#include <limits.h>
#include <stdlib.h>
#include <time.h>

#include "./include/maze.h"

void random_generator_init() {
  time_t seed = time(NULL);
  srand(seed);
}

int random_generator(int min, int max) {
  if (max == min) return max;
  if (max < 0) return 0;
  int rd_num = rand() % (max - min + 1) + min;
  return rd_num;
}

void generate_walls(struct maze_optimized_t* maze, size_t row, size_t* sets) {
  size_t prev_set = sets[0];
  for (size_t j = 0; j < maze->cols - 1; j++) {
    if (j != 0 && prev_set == sets[j]) {
      set_maze_optimized_left_border(maze, row, j + 1, true);
      set_maze_optimized_right_border(maze, row, j, true);
    } else if (random_generator(0, 1) == 0) {
      set_maze_optimized_left_border(maze, row, j + 1, true);
      set_maze_optimized_right_border(maze, row, j, true);
    }
  }
}

int compare_size_t(const void* a, const void* b) {
  size_t x = *(size_t*)a;
  size_t y = *(size_t*)b;
  return (x > y) - (x < y);
}

void refill_sets(maze_optimized_t* maze, size_t current_row, size_t* sets) {
  size_t cols = maze->cols;
  size_t counter = 0;
  for (size_t j = 0; j < cols; j++) {
    if (get_maze_optimized_upper_border(maze, current_row, j) == 0) {
      counter += 1;
    }
  }
  if (counter > 0) {
    size_t* no_border = malloc(sizeof(size_t) * counter);
    size_t i = 0;
    for (size_t j = 0; j < cols; j++) {
      if (get_maze_optimized_upper_border(maze, current_row, j) == 0) {
        no_border[i] = sets[j];
        i += 1;
      }
    }
    qsort(no_border, counter, sizeof(size_t), compare_size_t);
    size_t current_set = 1;
    i = 0;
    for (size_t j = 0; j < cols; j++) {
      if (get_maze_optimized_upper_border(maze, current_row, j) == 1) {
        if (current_set == no_border[i]) {
          while (current_set == no_border[i]) {
            if (i >= sizeof(no_border) / sizeof(no_border[0])) {
              break;
            }
            i += 1;
            current_set += 1;
          }
        }
        sets[j] = current_set;
        current_set += 1;
      }
    }
    free(no_border);
  } else {
    for (size_t j = 0; j < cols; j++) {
      sets[j] = j + 1;
    }
  }
}

void union_sets(maze_optimized_t* maze, size_t* sets, size_t current_row) {
  size_t current_set = sets[0];
  for (size_t j = 0; j < maze->cols; j++) {
    if (get_maze_optimized_left_border(maze, current_row, j) == 0) {
      size_t set_to_join = sets[j];
      for (size_t k = 0; k < maze->cols; k++) {
        if (sets[k] == set_to_join) {
          sets[k] = current_set;
        }
      }
    } else {
      current_set = sets[j];
    }
  }
}

void add_bottom_walls(maze_optimized_t* maze, size_t current_row) {
  size_t cols = maze->cols;
  size_t start_j_current_set = 0;
  for (size_t j = 0; j < cols; j++) {
    if (get_maze_optimized_right_border(maze, current_row, j) == 1) {
      size_t end_j_current_set = j;
      if (start_j_current_set != end_j_current_set) {
        int rand_int = random_generator(1, 2);
        // int rand_int = 1;
        if (rand_int == 1) {
          for (size_t k = start_j_current_set; k < end_j_current_set; k++) {
            set_maze_optimized_bottom_border(maze, current_row, k, true);
            set_maze_optimized_upper_border(maze, current_row + 1, k, true);
          }
        } else {
          for (size_t k = end_j_current_set; k > start_j_current_set; k--) {
            set_maze_optimized_bottom_border(maze, current_row, k, true);
            set_maze_optimized_upper_border(maze, current_row + 1, k, true);
          }
        }
      }
      start_j_current_set = end_j_current_set + 1;
    }
  }
}

void last_row(maze_optimized_t* maze, size_t* array) {
  size_t cols = maze->cols;
  size_t rows = maze->rows;
  size_t prev_set = array[0];
  for (size_t j = 1; j < cols; j++) {
    size_t current_set = array[j];
    if (prev_set != current_set) {
      set_maze_optimized_left_border(maze, rows - 1, j, false);
      set_maze_optimized_right_border(maze, rows - 1, j - 1, false);
    }
    prev_set = current_set;
  }
}

maze_optimized_t* generate_maze(size_t rows, size_t cols) {
  maze_optimized_t* maze = new_maze_optimized(rows, cols);
  size_t* sets = malloc(sizeof(size_t) * cols);

  if (!maze || !sets) {
    if (maze) free_maze_optimized(maze);
    if (sets) free(sets);
  }

  random_generator_init();

  for (size_t i = 0; i < rows - 1; i++) {
    refill_sets(maze, i, sets);
    generate_walls(maze, i, sets);
    union_sets(maze, sets, i);
    add_bottom_walls(maze, i);
  }
  last_row(maze, sets);
  free(sets);

  return maze;
}