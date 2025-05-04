#include <stdio.h>

#include "./include/io_cave.h"

FILE* open_file_r_cave(char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "cant find file: %s\n", filename);
  }
  return file;
}

// FILE* open_file_a_cave(char* filename) {
//     FILE* file = fopen(filename, "a");
//     if (file == NULL) {
//         fprintf(stderr, "cant find file and cant create new: %s\n",
//         filename);
//     }
//     return file;
// }

int read_cave_map(cave* maze, FILE* file) {
  char ch = '.';
  int val = 0;
  int return_code = 1;
  for (size_t i = 0; i < maze->rows && return_code == 1; i++) {
    for (size_t j = 0; j < maze->cols && return_code == 1; j++) {
      int ret = fscanf(file, "%d%c", &val, &ch);
      if (ret == 2 && (j + 1 == maze->cols && (ch == '\0' || ch == '\n'))) {
        if (val == 1) maze->map[i][j] = 1;
        // set_maze_map(maze, i, j, '1', maze_num);
        else
          maze->map[i][j] = 0;
        // set_maze_map(maze, i, j, '0', maze_num);
      } else if (ret == 2 && ch == ' ') {
        if (val == 1) maze->map[i][j] = 1;
        // set_maze_map(maze, i, j, '1', maze_num);
        else
          maze->map[i][j] = 0;
        // set_maze_map(maze, i, j, '0', maze_num);
      } else {
        perror("invalid maze input\n");
        return_code = 0;
      }
    }
  }
  return return_code;
}

cave* read_cave_from_file(char* file_name) {
  if (file_name == NULL) {
    perror("no file to open\n");
    return NULL;
  }
  FILE* file = open_file_r_cave(file_name);
  if (file == NULL) return NULL;

  size_t rows = 0;
  size_t cols = 0;
  cave* cave_ptr = NULL;
  int size_res = fscanf(file, "%zu %zu\n", &rows, &cols);
  if (size_res == 2) {
    cave_ptr = new_cave(rows, cols);
    if (cave_ptr) {
      int res = read_cave_map(cave_ptr, file);
      if (res != 1) {
        free_cave(cave_ptr);
        cave_ptr = NULL;
      }
    }
  }
  return cave_ptr;
}