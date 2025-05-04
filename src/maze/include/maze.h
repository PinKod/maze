#ifndef MAZE_H
#define MAZE_H

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#include "./../../includes/defines.h"

typedef struct {
  size_t rows;
  size_t cols;
  char** map1;
  char** map2;
} maze_t;

typedef struct maze_optimized_t maze_optimized_t;
struct maze_optimized_t {
  size_t rows;
  size_t cols;
  unsigned char** map;
};

// maze_optimized_t allocation && deallocation functions
maze_optimized_t* make_maze_optimized_from_maze(maze_t* maze);
maze_optimized_t* new_maze_optimized(size_t rows, size_t cols);
void free_maze_optimized(maze_optimized_t* maze);
int map_maze_alloc(maze_t* maze);
void maze_free(maze_t* maze);

// functions to work with maze_optimazied_t
void set_maze_optimized_left_border(maze_optimized_t* maze, size_t i, size_t j,
                                    bool border);
void set_maze_optimized_upper_border(maze_optimized_t* maze, size_t i, size_t j,
                                     bool border);
void set_maze_optimized_right_border(maze_optimized_t* maze, size_t i, size_t j,
                                     bool border);
void set_maze_optimized_bottom_border(maze_optimized_t* maze, size_t i,
                                      size_t j, bool border);

int get_maze_optimized_left_border(maze_optimized_t* maze, size_t i, size_t j);
int get_maze_optimized_upper_border(maze_optimized_t* maze, size_t i, size_t j);
int get_maze_optimized_right_border(maze_optimized_t* maze, size_t i, size_t j);
int get_maze_optimized_bottom_border(maze_optimized_t* maze, size_t i,
                                     size_t j);

void set_maze_optimized_not_checked(maze_optimized_t* maze, size_t i, size_t j);
void set_maze_optimized_checked(maze_optimized_t* maze, size_t i, size_t j);
int get_maze_optimized_state(maze_optimized_t* maze, size_t i, size_t j);
void set_maze_optimized_valid_path(maze_optimized_t* maze, size_t i, size_t j);
void set_maze_optimized_not_valid_path(maze_optimized_t* maze, size_t i,
                                       size_t j);
int get_maze_optimized_valid_path_state(maze_optimized_t* maze, size_t i,
                                        size_t j);

// raw functions to manipulate with maze bits
inline int get_bit(unsigned char ch, int index);
inline void set_bit(unsigned char* ch, int index, int bit);

// maze comparison functions
int compare_maze_matrix(maze_t* maze1, maze_t* maze2);
int compare_maze_in_files(char* file_name_1, char* file_name_2);

// maze generation functions
maze_optimized_t* generate_maze(size_t rows, size_t cols);

// maze solving functions
typedef struct node node;
struct node {
  size_t i;
  size_t j;
  size_t i_end;
  size_t j_end;
  node* prev_node;
  node* next_upper_node;
  node* next_right_node;
  node* next_bottom_node;
  node* next_left_node;
  maze_optimized_t* maze;
  volatile bool* flag;
  pthread_mutex_t* mutex;
  volatile long long int* thread_counter;
  node* res;
};
node* solve_maze(maze_optimized_t* maze, size_t i_start, size_t j_start,
                 size_t i_end, size_t j_end);
void free_nodes(node* start_node);
void print_nodes(FILE* fd, node* start_node);

// for testing purposes
void refill_sets(maze_optimized_t* maze, size_t current_row, size_t* sets);
void union_sets(maze_optimized_t* maze, size_t* sets, size_t current_row);
void add_bottom_walls(maze_optimized_t* maze, size_t current_row);
void init_maze_optimized_boarders(maze_optimized_t* maze_optimized);
#endif