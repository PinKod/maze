#include <stdio.h>

#include "./include/io_cave.h"

void print_cave(cave* cave_ptr) {
  if (cave_ptr == NULL || cave_ptr->map == NULL) return;
  fprintf(stdout, "\033[0d\033[2J");
  for (size_t i = 0; i < cave_ptr->rows; ++i) {
    for (size_t j = 0; j < cave_ptr->cols; ++j) {
      if (cave_ptr->map[i][j] == 1) {
        fprintf(stdout, " ");
      } else {
        fprintf(stdout, "#");
      }
    }
    fprintf(stdout, "\n");
  }
}