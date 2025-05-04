#define _GNU_SOURCE

#include <ctype.h>
#include <math.h>
#include <poll.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#include "./include/common.h"

typedef struct {
  pid_t pid;
  int port;
  int is_running;
  char path_to_service[256];
} Instance;

static Instance* instances = NULL;
static size_t instances_count = 0;
static size_t instances_capacity = 0;
static int next_port = 8080;

struct stdin_settings {
  struct pollfd new;
  struct termios prev;
};

enum State { STATE_NORMAL, STATE_STOPPING };
static enum State current_state = STATE_NORMAL;
static char port_buffer[32];
static int port_index = 0;

struct stdin_settings input_nb_init() {
  struct stdin_settings settings = {};
  tcgetattr(0, &settings.prev);

  struct termios tc;
  tcgetattr(0, &tc);
  tc.c_lflag &= ~(ICANON | ECHO);
  tc.c_cc[VMIN] = 0;
  tc.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &tc);

  struct pollfd pfd = {.fd = 0, .events = POLLIN};
  settings.new = pfd;
  return settings;
}

void input_restore(struct termios* orig) { tcsetattr(0, TCSANOW, orig); }

void list_of_instructions();
void list_all_instances();
void list_running_instances();
void add_instance(pid_t pid, int port, char* service);
void stop_instance(int port);

int main() {
  struct stdin_settings settings = input_nb_init();
  struct pollfd pfd = settings.new;
  list_of_instructions();

  while (1) {
    int ret = poll(&pfd, 1, 1000);

    // Check for exited children
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
      if (pid == -1) break;
      for (size_t i = 0; i < instances_count; i++) {
        if (instances[i].pid == pid) {
          instances[i].is_running = 0;
          break;
        }
      }
    }

    if (ret > 0 && (pfd.revents & POLLIN)) {
      char c;
      if (read(0, &c, 1) > 0) {
        if (current_state == STATE_NORMAL) {
          switch (c) {
            case 'q':
              goto cleanup;
            case 'h':
              list_of_instructions();
              break;
            case 'r': {
              input_restore(&settings.prev);
              static char service[256];
              memset(service, 0, 256);
              fprintf(stdout, "enter service to start\n");
              if (fscanf(stdin, "%s", service) != 1) {
                fprintf(stderr,
                        "Invalid input. Please enter a valid service name.\n");
                break;
              }
              pid_t child_pid = fork();
              if (child_pid == 0) {
                char port_str[16];
                snprintf(port_str, sizeof(port_str), "%d", next_port);
                char* args[] = {service, port_str, NULL};
                execve(args[0], args, NULL);
                perror("execve");
                exit(EXIT_FAILURE);
              } else if (child_pid > 0) {
                add_instance(child_pid, next_port, service);
                printf("Started instance on port %d\n", next_port);
                next_port++;
              } else {
                perror("fork");
              }
              break;
            }
            case 's': {
              current_state = STATE_STOPPING;
              port_index = 0;
              memset(port_buffer, 0, sizeof(port_buffer));
              printf("\nEnter port to stop: ");
              fflush(stdout);
              break;
            }
            case 'l':
              list_all_instances();
              break;
            case 'c':
              list_running_instances();
              break;
          }
        } else if (current_state == STATE_STOPPING) {
          if (c == '\r' || c == '\n') {
            current_state = STATE_NORMAL;
            int port = atoi(port_buffer);
            stop_instance(port);
            printf("\n");
          } else if (isdigit(c) && port_index < sizeof(port_buffer) - 1) {
            port_buffer[port_index++] = c;
            port_buffer[port_index] = '\0';
            write(STDOUT_FILENO, &c, 1);
          } else {
            printf("\nInvalid input. Aborting stop command.\n");
            current_state = STATE_NORMAL;
          }
        }
      }
    }
  }

cleanup:
  input_restore(&settings.prev);

  // Terminate all running instances
  for (size_t i = 0; i < instances_count; i++) {
    if (instances[i].is_running) {
      kill(instances[i].pid, SIGTERM);
    }
  }

  // Wait for all children to exit
  while (waitpid(-1, NULL, 0) > 0)
    ;

  free(instances);
  return 0;
}

void add_instance(pid_t pid, int port, char* service) {
  if (instances_count >= instances_capacity) {
    instances_capacity = instances_capacity ? 2 * instances_capacity : 4;
    instances = realloc(instances, instances_capacity * sizeof(Instance));
  }
  instances[instances_count++] = (Instance){pid, port, 1, ""};
  strcpy(instances[instances_count - 1].path_to_service, service);
}

void stop_instance(int port) {
  for (size_t i = 0; i < instances_count; i++) {
    if (instances[i].port == port && instances[i].is_running) {
      kill(instances[i].pid, SIGTERM);
      instances[i].is_running = 0;
      printf("Stopped instance on port %d", port);
      return;
    }
  }
  printf("No running instance found on port %d", port);
}

void list_all_instances() {
  printf("\nAll instances:\n");
  for (size_t i = 0; i < instances_count; i++) {
    printf("PID: %-6d Port: %-5d Status: %s    Service: %s\n", instances[i].pid,
           instances[i].port, instances[i].is_running ? "Running" : "Stopped",
           instances[i].path_to_service);
  }
}

void list_running_instances() {
  printf("\nRunning instances:\n");
  for (size_t i = 0; i < instances_count; i++) {
    if (instances[i].is_running) {
      printf("PID: %-6d Port: %-5d    Service: %s\n", instances[i].pid,
             instances[i].port, instances[i].path_to_service);
    }
  }
}

void list_of_instructions() {
  printf(
      "\n\nq - Stop all instances and exit\n"
      "r - Run new instance\n"
      "s - Stop existing instance\n"
      "l - List all instances\n"
      "c - List running instances\n"
      "h - Show this help\n\n");
}