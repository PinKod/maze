#define _GNU_SOURCE
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "./include/maze.h"

typedef enum { UP, RIGHT, DOWN, LEFT, UNVALID } direction;

typedef enum { VALID, INVALID } path_validity;

void* walk_maze(void* args);
node* new_node(size_t i, size_t j, node* prev_node, volatile bool* flag,
               size_t i_end, size_t j_end, maze_optimized_t* maze,
               pthread_mutex_t* mutex, volatile long long int* thread_counter);
unsigned char possible_dirs_walk(node* current_node, maze_optimized_t* maze);
direction possible_dir(node* current_node, maze_optimized_t* maze);
int thread_create(pthread_t* thread, const pthread_attr_t* attr,
                  void* (*start_routine)(void*), void* arg,
                  pthread_mutex_t* mutex,
                  volatile long long int* thread_counter);
void thread_exit(void* retval, pthread_mutex_t* mutex,
                 volatile long long int* thread_counter);
void thread_detach_exit(void* retval, maze_optimized_t* maze,
                        node* current_node, node* start_node);
node* one_dir_walk(node* current_node, volatile bool* flag,
                   maze_optimized_t* maze, pthread_mutex_t* mutex,
                   volatile long long int* thread_counter);
direction* possible_dirs_multiply(node* current_node, maze_optimized_t* maze,
                                  direction* dirs);
bool check_flag(volatile bool* flag, pthread_mutex_t* mutex);
long long int check_thread_counter(volatile long long int* thread_counter,
                                   pthread_mutex_t* mutex);

void wait();

node* solve_maze(maze_optimized_t* maze, size_t i_start, size_t j_start,
                 size_t i_end, size_t j_end) {
  if (!maze) return NULL;

  volatile bool* flag = malloc(sizeof(bool));
  *flag = true;
  pthread_mutex_t* mutex = malloc(sizeof(pthread_mutex_t));
  volatile long long int* thread_counter =
      malloc(sizeof(unsigned long long int));
  *thread_counter = 0;
  node* start_node = new_node(i_start, j_start, NULL, flag, i_end, j_end, maze,
                              mutex, thread_counter);
  pthread_t* thread = malloc(sizeof(pthread_t));
  if (!start_node || !thread || !flag || !mutex || !thread_counter) {
    if (start_node) free(start_node);
    if (thread) free(thread);
    if (flag) free((void*)flag);
    if (mutex) free(mutex);
    if (thread_counter) free((void*)thread_counter);
    return NULL;
  }
  pthread_mutex_init(mutex, NULL);
  thread_create(thread, NULL, walk_maze, (void*)start_node, mutex,
                thread_counter);

  node* res_node = NULL;
  pthread_join(*thread, NULL);
  while (check_thread_counter(thread_counter, mutex) > 0) {
    wait();
  }

  free(thread);
  free((void*)flag);
  pthread_mutex_destroy(mutex);
  free(mutex);
  free((void*)thread_counter);
  return res_node;
}

void wait() {}

int thread_create(pthread_t* thread, const pthread_attr_t* attr,
                  void* (*start_routine)(void*), void* arg,
                  pthread_mutex_t* mutex,
                  volatile long long int* thread_counter) {
  pthread_mutex_lock(mutex);
  *thread_counter += 1;
  // fprintf(stderr, "create thread_counter: %lld\n", *thread_counter);
  int res = pthread_create(thread, attr, start_routine, arg);
  pthread_mutex_unlock(mutex);
  return res;
}

void thread_exit(void* retval, pthread_mutex_t* mutex,
                 volatile long long int* thread_counter) {
  pthread_mutex_lock(mutex);
  *thread_counter -= 1;
  // fprintf(stderr, "kill thread_counter: %lld\n", *thread_counter);
  pthread_mutex_unlock(mutex);
  pthread_exit(retval);
}

void thread_detach_exit(void* retval, maze_optimized_t* maze,
                        node* current_node, node* start_node) {
  pthread_detach(pthread_self());
  pthread_mutex_t* mutex = start_node->mutex;
  volatile long long int* thread_counter = start_node->thread_counter;

  while (current_node != start_node) {
    node* prev_node = current_node->prev_node;
    free(current_node);
    current_node = prev_node;
  }
  // node* prev_node = start_node->prev_node;
  // if (prev_node->i < start_node->i) {
  //     prev_node->next_bottom_node = NULL;
  // } else if (prev_node->i > start_node->i) {
  //     prev_node->next_upper_node = NULL;
  // } else if (prev_node->j < start_node->j) {
  //     prev_node->next_right_node = NULL;
  // } else if (prev_node->j > start_node->j) {
  //     prev_node->next_left_node = NULL;
  // }
  free(start_node);
  thread_exit(NULL, mutex, thread_counter);
}

bool check_flag(volatile bool* flag, pthread_mutex_t* mutex) {
  pthread_mutex_lock(mutex);
  bool result = *flag;
  pthread_mutex_unlock(mutex);
  return result;
}

long long int check_thread_counter(volatile long long int* thread_counter,
                                   pthread_mutex_t* mutex) {
  pthread_mutex_lock(mutex);
  long long int result = *thread_counter;
  pthread_mutex_unlock(mutex);
  return result;
}

void* walk_maze(void* args) {
  node* start_node = (node*)args;
  volatile bool* flag = start_node->flag;

  maze_optimized_t* maze = start_node->maze;
  node* current_node = start_node;
  int iteration_main = 0;
  while (check_flag(flag, start_node->mutex) == true) {
    //// fprintf(stderr, "main loop      %lu %lu\n", current_node->i,
    ///current_node->j);
    // check if node is end(target) node? if so:
    // 1)mutex(lock) all treads,
    // 2)retrun to global start node & setting valid path,
    // 3)change flag
    // 4)unlock mutex
    if (current_node->i == current_node->i_end &&
        current_node->j == current_node->j_end) {
      int res = pthread_mutex_lock(start_node->mutex);
      // fprintf(stderr, "mutex lock res=%d\n", res);
      set_maze_optimized_valid_path(maze, start_node->i, start_node->j);
      // fprintf(stderr, "main loop, target found, back trace all nodes\n");
      node* local_current_node = current_node;

      while (current_node) {
        // fprintf(stderr, "%lu    %lu\n", current_node->i, current_node->j);
        set_maze_optimized_valid_path(maze, current_node->i, current_node->j);

        current_node = current_node->prev_node;
      }
      while (local_current_node != start_node) {
        node* to_free = local_current_node;
        local_current_node = local_current_node->prev_node;
        free(to_free);
      }
      pthread_mutex_t* mutex = start_node->mutex;
      volatile long long int* thread_counter = start_node->thread_counter;
      free(start_node);
      // fprintf(stderr, "flag before: %d\n", *flag);
      *flag = false;
      // fprintf(stderr, "flag after: %d\n", *flag);
      pthread_mutex_unlock(mutex);
      thread_exit(NULL, mutex, thread_counter);
    }

    unsigned char possible_dirs = possible_dirs_walk(current_node, maze);
    if (possible_dirs == 0) {
      while (current_node != start_node) {
        // set_maze_optimized_not_valid_path(maze, current_node->i,
        // current_node->j);
        node* prev = current_node->prev_node;
        free(current_node);
        current_node = prev;
      }
      pthread_mutex_t* mutex = start_node->mutex;
      volatile long long int* thread_counter = start_node->thread_counter;
      free(start_node);
      thread_exit(NULL, mutex, thread_counter);
    } else if (possible_dirs == 1) {
      current_node = one_dir_walk(current_node, flag, maze, start_node->mutex,
                                  start_node->thread_counter);
    } else {
      pthread_t* thread_upper = NULL;
      pthread_t* thread_right = NULL;
      pthread_t* thread_bottom = NULL;
      pthread_t* thread_left = NULL;
      bool flag_upper = false;
      bool flag_right = false;
      bool flag_bottom = false;
      bool flag_left = false;
      direction dirs_initial[4];
      direction* dirs =
          possible_dirs_multiply(current_node, maze, &dirs_initial[0]);
      // print all dirs
      for (int i = 0; i < 4; i++) {
        // fprintf(stderr, "dirs[%d]=%d ", i, dirs[i]);
      }
      if (dirs[0] == UP) {
        thread_upper = malloc(sizeof(pthread_t));
        node* next_upper_node =
            new_node(current_node->i - 1, current_node->j, current_node, flag,
                     current_node->i_end, current_node->j_end, maze,
                     current_node->mutex, current_node->thread_counter);
        thread_create(thread_upper, NULL, walk_maze, (void*)next_upper_node,
                      current_node->mutex, current_node->thread_counter);
        flag_upper = true;
      }
      if (dirs[1] == RIGHT) {
        thread_right = malloc(sizeof(pthread_t));
        node* next_right_node =
            new_node(current_node->i, current_node->j + 1, current_node, flag,
                     current_node->i_end, current_node->j_end, maze,
                     current_node->mutex, current_node->thread_counter);
        thread_create(thread_right, NULL, walk_maze, (void*)next_right_node,
                      current_node->mutex, current_node->thread_counter);
        flag_right = true;
      }
      if (dirs[2] == DOWN) {
        thread_bottom = malloc(sizeof(pthread_t));
        node* next_bottom_node =
            new_node(current_node->i + 1, current_node->j, current_node, flag,
                     current_node->i_end, current_node->j_end, maze,
                     current_node->mutex, current_node->thread_counter);
        thread_create(thread_bottom, NULL, walk_maze, (void*)next_bottom_node,
                      current_node->mutex, current_node->thread_counter);
        flag_bottom = true;
      }
      if (dirs[3] == LEFT) {
        thread_left = malloc(sizeof(pthread_t));
        node* next_left_node =
            new_node(current_node->i, current_node->j - 1, current_node, flag,
                     current_node->i_end, current_node->j_end, maze,
                     current_node->mutex, current_node->thread_counter);
        thread_create(thread_left, NULL, walk_maze, (void*)next_left_node,
                      current_node->mutex, current_node->thread_counter);
        flag_left = true;
      }
      unsigned char iteration = 0;
      while (check_flag(flag, start_node->mutex) == true &&
             (dirs[0] == UP || dirs[1] == RIGHT || dirs[2] == DOWN ||
              dirs[3] == LEFT)) {
        // fprintf(stderr, "mul while loop\nflag=%d\ndirs[0]=%d    dirs[1]=%d
        // dirs[2]=%d    dirs[3]=%d\n", *flag, dirs[0], dirs[1], dirs[2],
        // dirs[3]);
        if (dirs[0] == UP) {
          int res_thread_upper = pthread_tryjoin_np(*thread_upper, NULL);
          if (res_thread_upper == 0) {
            dirs[0] = UNVALID;
            free(thread_upper);
            flag_upper = false;
          }
        }
        if (dirs[1] == RIGHT) {
          int res_thread_right = pthread_tryjoin_np(*thread_right, NULL);
          if (res_thread_right == 0) {
            dirs[1] = UNVALID;
            free(thread_right);
            flag_right = false;
          }
        }
        if (dirs[2] == DOWN) {
          int res_thread_bottom = pthread_tryjoin_np(*thread_bottom, NULL);
          if (res_thread_bottom == 0) {
            dirs[2] = UNVALID;
            free(thread_bottom);
            flag_bottom = false;
          }
        }
        if (dirs[3] == LEFT) {
          int res_thread_left = pthread_tryjoin_np(*thread_left, NULL);
          if (res_thread_left == 0) {
            dirs[3] = UNVALID;
            free(thread_left);
            flag_left = false;
          }
        }
      }
      if (flag_upper) free(thread_upper);
      if (flag_right) free(thread_right);
      if (flag_bottom) free(thread_bottom);
      if (flag_left) free(thread_left);
      // // fprintf(stderr, "after mul while loop\n");
      // if we here??? means that flag was switched somewhere(target found) or
      // all subtreads finished with no result

      // if (get_maze_optimized_valid_path_state(maze, current_node->i,
      // current_node->j) == 0) {
      //     pthread_detach(pthread_self());
      //     while (current_node != start_node) {
      //         // set_maze_optimized_not_valid_path(maze, current_node->i,
      //         current_node->j); node* prev = current_node->prev_node;
      //         // fprintf(stderr, "free node i=%lu, j=%lu\n", current_node->i,
      //         current_node->j); free(current_node); current_node = prev;
      //     }
      //     // // fprintf(stderr, "after free while loop\n");
      //     pthread_mutex_t*        mutex = start_node->mutex;
      //     volatile long long int* thread_counter =
      //     start_node->thread_counter;
      //     // set_maze_optimized_not_valid_path(maze, start_node->i,
      //     start_node->j); free(start_node); start_node = NULL;
      //     thread_exit(NULL, mutex, thread_counter);
      // }
      if (*flag == false) {
        thread_detach_exit(NULL, maze, current_node, start_node);
      } else {
        while (current_node != start_node) {
          // set_maze_optimized_not_valid_path(maze, current_node->i,
          // current_node->j);
          node* prev = current_node->prev_node;
          // fprintf(stderr, "free node i=%lu, j=%lu\n", current_node->i,
          // current_node->j);
          free(current_node);
          current_node = prev;
        }
        // // fprintf(stderr, "after free while loop\n");
        pthread_mutex_t* mutex = start_node->mutex;
        volatile long long int* thread_counter = start_node->thread_counter;
        // set_maze_optimized_not_valid_path(maze, start_node->i,
        // start_node->j);
        free(start_node);
        start_node = NULL;
        thread_exit(NULL, mutex, thread_counter);
      }
    }
  }
  // flag was switched somewhere? target was found. Need to clear up resourses
  // 1)check if nodes in valid path
  // 2)if yes, do nothing
  // 3)if no, detach -> clear up all nodes
  thread_detach_exit(NULL, maze, current_node, start_node);
}

node* one_dir_walk(node* current_node, volatile bool* flag,
                   maze_optimized_t* maze, pthread_mutex_t* mutex,
                   volatile long long int* thread_counter) {
  node* next_node = new_node(0, 0, current_node, flag, current_node->i_end,
                             current_node->j_end, maze, mutex, thread_counter);
  direction dir = possible_dir(current_node, maze);
  switch (dir) {
    case UP:
      next_node->i = current_node->i - 1;
      next_node->j = current_node->j;
      current_node->next_upper_node = next_node;
      break;
    case RIGHT:
      next_node->i = current_node->i;
      next_node->j = current_node->j + 1;
      current_node->next_right_node = next_node;
      break;
    case DOWN:
      next_node->i = current_node->i + 1;
      next_node->j = current_node->j;
      current_node->next_bottom_node = next_node;
      break;
    case LEFT:
      next_node->i = current_node->i;
      next_node->j = current_node->j - 1;
      current_node->next_left_node = next_node;
      break;
    default:
      next_node->i = current_node->i;
      next_node->j = current_node->j;
      current_node->next_left_node = next_node;
      break;
  }
  current_node = next_node;

  return current_node;
}

direction* possible_dirs_multiply(node* current_node, maze_optimized_t* maze,
                                  direction* dirs) {
  dirs[0] = UP;
  dirs[1] = RIGHT;
  dirs[2] = DOWN;
  dirs[3] = LEFT;

  // clang-format off
    if(get_maze_optimized_upper_border (maze, current_node->i, current_node->j) == 1) dirs[0] = UNVALID;
    if(get_maze_optimized_right_border (maze, current_node->i, current_node->j) == 1) dirs[1] = UNVALID;
    if(get_maze_optimized_bottom_border(maze, current_node->i, current_node->j) == 1) dirs[2] = UNVALID;
    if(get_maze_optimized_left_border  (maze, current_node->i, current_node->j) == 1) dirs[3] = UNVALID;
  // clang-format on

  // fprintf(stderr, "\n\nnode i=%lu, j=%lu\n", current_node->i,
  // current_node->j);
  if ((current_node->prev_node &&
       current_node->prev_node->i < current_node->i) ||
      dirs[0] == UNVALID) {
    dirs[0] = UNVALID;
    // fprintf(stderr, "UP invalid\n");
  }
  if ((current_node->prev_node &&
       current_node->prev_node->j > current_node->j) ||
      dirs[1] == UNVALID) {
    dirs[1] = UNVALID;
    // fprintf(stderr, "RIGHT invalid\n");
  }
  if ((current_node->prev_node &&
       current_node->prev_node->i > current_node->i) ||
      dirs[2] == UNVALID) {
    dirs[2] = UNVALID;
    // fprintf(stderr, "DOWN invalid\n");
  }
  if ((current_node->prev_node &&
       current_node->prev_node->j < current_node->j) ||
      dirs[3] == UNVALID) {
    dirs[3] = UNVALID;
    // fprintf(stderr, "LEFT invalid\n");
  }

  return dirs;
}

direction possible_dir(node* current_node, maze_optimized_t* maze) {
  direction dirs[] = {UP, RIGHT, DOWN, LEFT};
  // clang-format off
    if(get_maze_optimized_upper_border (maze, current_node->i, current_node->j) == 1) dirs[0] = UNVALID;
    if(get_maze_optimized_right_border (maze, current_node->i, current_node->j) == 1) dirs[1] = UNVALID;
    if(get_maze_optimized_bottom_border(maze, current_node->i, current_node->j) == 1) dirs[2] = UNVALID;
    if(get_maze_optimized_left_border  (maze, current_node->i, current_node->j) == 1) dirs[3] = UNVALID;
  // clang-format on

  unsigned char possible_dirs = 4;

  if ((current_node->prev_node &&
       current_node->prev_node->i < current_node->i) ||
      dirs[0] == UNVALID) {
    possible_dirs -= 1;
    dirs[0] = UNVALID;
  }
  if ((current_node->prev_node &&
       current_node->prev_node->j > current_node->j) ||
      dirs[1] == UNVALID) {
    possible_dirs -= 1;
    dirs[1] = UNVALID;
  }
  if ((current_node->prev_node &&
       current_node->prev_node->i > current_node->i) ||
      dirs[2] == UNVALID) {
    possible_dirs -= 1;
    dirs[2] = UNVALID;
  }
  if ((current_node->prev_node &&
       current_node->prev_node->j < current_node->j) ||
      dirs[3] == UNVALID) {
    possible_dirs -= 1;
    dirs[3] = UNVALID;
  }

  direction dir = UNVALID;

  if (dirs[0] != UNVALID) {
    dir = UP;
  }
  if (dirs[1] != UNVALID) {
    dir = RIGHT;
  }
  if (dirs[2] != UNVALID) {
    dir = DOWN;
  }
  if (dirs[3] != UNVALID) {
    dir = LEFT;
  }

  return dir;
}

unsigned char possible_dirs_walk(node* current_node, maze_optimized_t* maze) {
  direction dirs[] = {UP, RIGHT, DOWN, LEFT};
  // clang-format off
    if(get_maze_optimized_upper_border (maze, current_node->i, current_node->j) == 1) dirs[0] = UNVALID;
    if(get_maze_optimized_right_border (maze, current_node->i, current_node->j) == 1) dirs[1] = UNVALID;
    if(get_maze_optimized_bottom_border(maze, current_node->i, current_node->j) == 1) dirs[2] = UNVALID;
    if(get_maze_optimized_left_border  (maze, current_node->i, current_node->j) == 1) dirs[3] = UNVALID;
  // clang-format on

  unsigned char possible_dirs = 4;

  if ((current_node->prev_node &&
       current_node->prev_node->i < current_node->i) ||
      dirs[0] == UNVALID) {
    possible_dirs -= 1;
    dirs[0] = UNVALID;
  }
  if ((current_node->prev_node &&
       current_node->prev_node->j > current_node->j) ||
      dirs[1] == UNVALID) {
    possible_dirs -= 1;
    dirs[1] = UNVALID;
  }
  if ((current_node->prev_node &&
       current_node->prev_node->i > current_node->i) ||
      dirs[2] == UNVALID) {
    possible_dirs -= 1;
    dirs[2] = UNVALID;
  }
  if ((current_node->prev_node &&
       current_node->prev_node->j < current_node->j) ||
      dirs[3] == UNVALID) {
    possible_dirs -= 1;
    dirs[3] = UNVALID;
  }

  return possible_dirs;
}

node* new_node(size_t i, size_t j, node* prev_node, volatile bool* flag,
               size_t i_end, size_t j_end, maze_optimized_t* maze,
               pthread_mutex_t* mutex, volatile long long int* thread_counter) {
  node* new_node = (node*)malloc(sizeof(node));

  new_node->i = i;
  new_node->j = j;
  new_node->i_end = i_end;
  new_node->j_end = j_end;

  new_node->prev_node = prev_node;
  new_node->next_upper_node = NULL;
  new_node->next_right_node = NULL;
  new_node->next_bottom_node = NULL;
  new_node->next_left_node = NULL;

  new_node->maze = maze;

  new_node->flag = flag;
  new_node->mutex = mutex;
  new_node->thread_counter = thread_counter;

  return new_node;
}

void free_nodes(node* start_node) {
  node* current_node = start_node;
  while (current_node) {
    node* next_node = NULL;
    if (current_node->next_upper_node) {
      next_node = current_node->next_upper_node;
    } else if (current_node->next_right_node) {
      next_node = current_node->next_right_node;
    } else if (current_node->next_bottom_node) {
      next_node = current_node->next_bottom_node;
    } else if (current_node->next_left_node) {
      next_node = current_node->next_left_node;
    }
    free(current_node);
    current_node = next_node;
  }
}

void print_nodes(FILE* fd, node* start_node) {
  if (!start_node) {
    fprintf(fd, "no nodes\n");
    return;
  }
  node* current_node = start_node;
  while (current_node) {
    node* next_node = NULL;
    if (current_node->next_upper_node) {
      next_node = current_node->next_upper_node;
    } else if (current_node->next_right_node) {
      next_node = current_node->next_right_node;
    } else if (current_node->next_bottom_node) {
      next_node = current_node->next_bottom_node;
    } else if (current_node->next_left_node) {
      next_node = current_node->next_left_node;
    }
    // fprintf(fd, "pn: %zu %zu\n", current_node->i, current_node->j);
    current_node = next_node;
  }
}