#include <stdlib.h>

#include "./include/maze.h"

// extern declaration for inline functions
extern inline int get_bit(unsigned char ch, int index);
extern inline void set_bit(unsigned char* ch, int index, int bit);

int validate_input(maze_optimized_t const* maze, size_t i, size_t j) {
  return (maze == NULL || maze->map == NULL || i >= maze->rows ||
          j >= maze->cols)
             ? 0
             : 1;
}

inline int get_bit(unsigned char ch, int index) {
  if (index < 0 || index >= 6) return 0;
  return (ch >> index) & 1;
}

inline void set_bit(unsigned char* ch, int index, int bit) {
  if (index < 0 || index >= 6) return;
  if (bit == 1) {
    *ch |= 1 << index;
  } else {
    *ch &= ~(1 << index);
  }
}

void set_maze_optimized_left_border(maze_optimized_t* maze, size_t i, size_t j,
                                    bool border) {
  if (validate_input(maze, i, j) == 0) return;
  if (border == true)
    set_bit(&maze->map[i][j], 0, 1);
  else
    set_bit(&maze->map[i][j], 0, 0);
}

void set_maze_optimized_upper_border(maze_optimized_t* maze, size_t i, size_t j,
                                     bool border) {
  if (validate_input(maze, i, j) == 0) return;
  if (border == true)
    set_bit(&maze->map[i][j], 1, 1);
  else
    set_bit(&maze->map[i][j], 1, 0);
}

void set_maze_optimized_right_border(maze_optimized_t* maze, size_t i, size_t j,
                                     bool border) {
  if (validate_input(maze, i, j) == 0) return;
  if (border == true)
    set_bit(&maze->map[i][j], 2, 1);
  else
    set_bit(&maze->map[i][j], 2, 0);
}

void set_maze_optimized_bottom_border(maze_optimized_t* maze, size_t i,
                                      size_t j, bool border) {
  if (validate_input(maze, i, j) == 0) return;
  if (border == true)
    set_bit(&maze->map[i][j], 3, 1);
  else
    set_bit(&maze->map[i][j], 3, 0);
}

int get_maze_optimized_left_border(maze_optimized_t* maze, size_t i, size_t j) {
  if (validate_input(maze, i, j) == 0) return 1;
  return get_bit(maze->map[i][j], 0);
}

int get_maze_optimized_upper_border(maze_optimized_t* maze, size_t i,
                                    size_t j) {
  if (validate_input(maze, i, j) == 0) return 1;
  return get_bit(maze->map[i][j], 1);
}

int get_maze_optimized_right_border(maze_optimized_t* maze, size_t i,
                                    size_t j) {
  if (validate_input(maze, i, j) == 0) return 1;
  return get_bit(maze->map[i][j], 2);
}

int get_maze_optimized_bottom_border(maze_optimized_t* maze, size_t i,
                                     size_t j) {
  if (validate_input(maze, i, j) == 0) return 1;
  return get_bit(maze->map[i][j], 3);
}

void set_maze_optimized_not_checked(maze_optimized_t* maze, size_t i,
                                    size_t j) {
  if (validate_input(maze, i, j) != 0) set_bit(&maze->map[i][j], 4, 0);
}

void set_maze_optimized_checked(maze_optimized_t* maze, size_t i, size_t j) {
  if (validate_input(maze, i, j) != 0) set_bit(&maze->map[i][j], 4, 1);
}

int get_maze_optimized_state(maze_optimized_t* maze, size_t i, size_t j) {
  if (validate_input(maze, i, j) == 0) return 0;
  return get_bit(maze->map[i][j], 4);
}

void set_maze_optimized_valid_path(maze_optimized_t* maze, size_t i, size_t j) {
  if (validate_input(maze, i, j) != 0) set_bit(&maze->map[i][j], 5, 1);
}

void set_maze_optimized_not_valid_path(maze_optimized_t* maze, size_t i,
                                       size_t j) {
  if (validate_input(maze, i, j) != 0) set_bit(&maze->map[i][j], 5, 0);
}
int get_maze_optimized_valid_path_state(maze_optimized_t* maze, size_t i,
                                        size_t j) {
  if (validate_input(maze, i, j) == 0) return 0;
  return get_bit(maze->map[i][j], 5);
}