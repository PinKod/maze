#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define THREAD_POOL_SIZE 4
#define QUEUE_SIZE 100
#define BUFFER_SIZE 1024

typedef struct {
  int *client_sockets;
  int front;
  int rear;
  int count;
  pthread_mutex_t lock;
  pthread_cond_t cond;
} Queue;

typedef struct {
  Queue *queue;
  int server_fd;
} ThreadArgs;

Queue queue;
volatile sig_atomic_t shutdown_flag = 0;

void handle_signal(int sig) { shutdown_flag = 1; }

void queue_init(Queue *q) {
  q->client_sockets = malloc(QUEUE_SIZE * sizeof(int));
  q->front = q->rear = q->count = 0;
  pthread_mutex_init(&q->lock, NULL);
  pthread_cond_init(&q->cond, NULL);
}

void enqueue(Queue *q, int client_socket) {
  pthread_mutex_lock(&q->lock);
  if (q->count < QUEUE_SIZE) {
    q->client_sockets[q->rear] = client_socket;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->count++;
    pthread_cond_signal(&q->cond);
  } else {
    close(client_socket);
  }
  pthread_mutex_unlock(&q->lock);
}

int dequeue(Queue *q) {
  pthread_mutex_lock(&q->lock);
  while (q->count == 0 && !shutdown_flag) {
    pthread_cond_wait(&q->cond, &q->lock);
  }

  if (shutdown_flag) {
    pthread_mutex_unlock(&q->lock);
    return -1;
  }

  int client_socket = q->client_sockets[q->front];
  q->front = (q->front + 1) % QUEUE_SIZE;
  q->count--;
  pthread_mutex_unlock(&q->lock);
  return client_socket;
}

void handle_request(int client_socket) {
  char buffer[BUFFER_SIZE];
  ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);

  if (bytes_read > 0) {
    buffer[bytes_read] = '\0';
    printf("Received request:\n%s\n", buffer);

    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello from microservice thread!";
    write(client_socket, response, strlen(response));
  }

  close(client_socket);
}

void *worker_thread(void *arg) {
  while (!shutdown_flag) {
    int client_socket = dequeue(&queue);
    if (client_socket == -1) break;
    handle_request(client_socket);
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int port = atoi(argv[1]);
  struct sigaction sa;
  sa.sa_handler = handle_signal;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGTERM, &sa, NULL);

  int server_fd;
  struct sockaddr_in address;
  int opt = 1;

  // Create server socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set socket options
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  // Bind socket to port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Start listening
  if (listen(server_fd, SOMAXCONN) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Microservice listening on port %d\n", port);

  // Initialize thread pool
  queue_init(&queue);
  pthread_t threads[THREAD_POOL_SIZE];

  for (int i = 0; i < THREAD_POOL_SIZE; i++) {
    if (pthread_create(&threads[i], NULL, worker_thread, NULL) != 0) {
      perror("Failed to create thread");
      exit(EXIT_FAILURE);
    }
  }

  // Main server loop
  while (!shutdown_flag) {
    int client_socket;
    socklen_t addrlen = sizeof(address);

    if (client_socket =
            accept(server_fd, (struct sockaddr *)&address, &addrlen)) {
      if (client_socket < 0) {
        if (!shutdown_flag) perror("accept");
        continue;
      }
      enqueue(&queue, client_socket);
    }
  }

  // Cleanup
  printf("Shutting down microservice...\n");
  close(server_fd);

  // Signal worker threads to exit
  for (int i = 0; i < THREAD_POOL_SIZE; i++) {
    enqueue(&queue, -1);
  }

  // Wait for all threads to finish
  for (int i = 0; i < THREAD_POOL_SIZE; i++) {
    pthread_join(threads[i], NULL);
  }

  free(queue.client_sockets);
  printf("Microservice shutdown complete\n");
  return 0;
}