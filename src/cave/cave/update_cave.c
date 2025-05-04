#include <stdlib.h>

#include "./include/cave.h"

int cave_get_bit(unsigned char ch, int index) {
  if (index < 0 || index >= 6) return 0;
  return (ch >> index) & 1;
}

void cave_set_bit(unsigned char* ch, int index, int bit) {
  if (index < 0 || index >= 6) return;
  if (bit == 1) {
    *ch |= 1 << index;
  } else {
    *ch &= ~(1 << index);
  }
}

int is_alive(unsigned char ch) { return cave_get_bit(ch, 0); }

void set_alive(unsigned char* ch) { cave_set_bit(ch, 0, 1); }

void set_alive_next_step(unsigned char* ch) { cave_set_bit(ch, 3, 1); }

void set_dead_next_step(unsigned char* ch) { cave_set_bit(ch, 3, 0); }

void set_state_next_step(unsigned char* ch) {
  int next_state = cave_get_bit(*ch, 3);
  *ch = 0;
  cave_set_bit(ch, 0, next_state);
}

// size_t get_alive_neighbors(cave* cave_ptr, size_t i, size_t j) {
//     size_t counter = 0;
//     size_t    max_row = cave_ptr->rows - 1;
//     size_t    max_col = cave_ptr->cols - 1;
//     if ((i == 0 && j == 0) || is_alive(cave_ptr->map[i - 1][j - 1]))
//         counter++;
//     if (i == 0 || is_alive(cave_ptr->map[i - 1][j]))
//         counter++;
//     if ((i == 0 && j + 1 > max_col) || is_alive(cave_ptr->map[i - 1][j + 1]))
//         counter++;
//     if (j + 1 > max_col || is_alive(cave_ptr->map[i][j + 1]))
//         counter++;
//     if ((i + 1 > max_row && j + 1 > max_col) || is_alive(cave_ptr->map[i +
//     1][j + 1]))
//         counter++;
//     if (i + 1 > max_row || is_alive(cave_ptr->map[i + 1][j]))
//         counter++;
//     if ((i + 1 > max_row && j == 0) || is_alive(cave_ptr->map[i + 1][j - 1]))
//         counter++;
//     if (j == 0 || is_alive(cave_ptr->map[i][j - 1]))
//         counter++;

//     return counter;
// }

size_t get_alive_neighbors(cave* cave_ptr, size_t i, size_t j) {
  size_t counter = 0;
  size_t max_row = cave_ptr->rows - 1;
  size_t max_col = cave_ptr->cols - 1;

  // Top-left neighbor
  if (i > 0 && j > 0) {
    if (is_alive(cave_ptr->map[i - 1][j - 1])) {
      counter++;
    }
  }

  // Top neighbor
  if (i > 0) {
    if (is_alive(cave_ptr->map[i - 1][j])) {
      counter++;
    }
  }

  // Top-right neighbor
  if (i > 0 && j < max_col) {
    if (is_alive(cave_ptr->map[i - 1][j + 1])) {
      counter++;
    }
  }

  // Right neighbor
  if (j < max_col) {
    if (is_alive(cave_ptr->map[i][j + 1])) {
      counter++;
    }
  }

  // Bottom-right neighbor
  if (i < max_row && j < max_col) {
    if (is_alive(cave_ptr->map[i + 1][j + 1])) {
      counter++;
    }
  }

  // Bottom neighbor
  if (i < max_row) {
    if (is_alive(cave_ptr->map[i + 1][j])) {
      counter++;
    }
  }

  // Bottom-left neighbor
  if (i < max_row && j > 0) {
    if (is_alive(cave_ptr->map[i + 1][j - 1])) {
      counter++;
    }
  }

  // Left neighbor
  if (j > 0) {
    if (is_alive(cave_ptr->map[i][j - 1])) {
      counter++;
    }
  }

  return counter;
}

int update_cave(cave* cave_ptr, size_t birth_limit, size_t death_limit) {
  if (!cave_ptr || !cave_ptr->map) return 66;
  int res = 0;

  size_t rows = cave_ptr->rows;
  size_t cols = cave_ptr->cols;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      size_t alive_neighbors = get_alive_neighbors(cave_ptr, i, j);
      if (is_alive(cave_ptr->map[i][j])) {
        if (alive_neighbors < death_limit) {
          set_dead_next_step(&cave_ptr->map[i][j]);
          res = 1;
        } else {
          set_alive_next_step(&cave_ptr->map[i][j]);
        }
      } else {
        if (alive_neighbors > birth_limit) {
          set_alive_next_step(&cave_ptr->map[i][j]);
          res = 1;
        } else {
          set_dead_next_step(&cave_ptr->map[i][j]);
        }
      }
    }
  }

  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      set_state_next_step(&cave_ptr->map[i][j]);
    }
  }
  return res;
}
