#include <stdlib.h>

#include "./include/io.h"

FILE* open_file_r(char* filename) {
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "cant find file: %s\n", filename);
  }
  return file;
}

FILE* open_file_a(char* filename) {
  FILE* file = fopen(filename, "a");
  if (file == NULL) {
    fprintf(stderr, "cant find file and cant create new: %s\n", filename);
  }
  return file;
}

int read_maze_size(FILE* file, maze_t* maze) {
  size_t rows = 0;
  size_t cols = 0;
  char ch1 = '.';
  char ch2 = '.';
  int return_code;
  if (fscanf(file, "%lu%c%lu%c", &rows, &ch1, &cols, &ch2) == 4 && ch1 == ' ' &&
      ch2 == '\n' && rows > 0 && cols > 0) {
    maze->rows = rows;
    maze->cols = cols;
    return_code = 1;
  } else {
    maze->rows = 0;
    maze->cols = 0;
    return_code = 0;
  }
  return return_code;
}

void set_maze_map(maze_t* maze, size_t i, size_t j, char ch, int maze_num) {
  if (maze_num == 1) {
    maze->map1[i][j] = ch;
  } else {
    maze->map2[i][j] = ch;
  }
}

int read_maze_map(maze_t* maze, FILE* file, int maze_num) {
  char ch = '.';
  int val = 0;
  int return_code = SUCCESS;
  for (size_t i = 0; i < maze->rows && return_code == SUCCESS; i++) {
    for (size_t j = 0; j < maze->cols && return_code == SUCCESS; j++) {
      int ret = fscanf(file, "%d%c", &val, &ch);
      if (ret == 2 && (j + 1 == maze->cols && (ch == '\0' || ch == '\n'))) {
        if (val == 1)
          set_maze_map(maze, i, j, '1', maze_num);
        else
          set_maze_map(maze, i, j, '0', maze_num);
      } else if (ret == 2 && ch == ' ') {
        if (val == 1)
          set_maze_map(maze, i, j, '1', maze_num);
        else
          set_maze_map(maze, i, j, '0', maze_num);
      } else {
        perror("invalid maze input\n");
        return_code = INVALID_INPUT_MAZE;
      }
    }
  }
  return return_code;
}

maze_t* read_maze_from_file(char* file_name) {
  if (file_name == NULL) {
    perror("no file to open\n");
    return NULL;
  }
  FILE* file = open_file_r(file_name);
  if (file == NULL) return NULL;
  maze_t* maze = malloc(sizeof(maze_t));
  if (maze == NULL) {
    fclose(file);
    return NULL;
  }
  char ch = '\n';
  int fail = 0;
  if (!(read_maze_size(file, maze) == 1 && map_maze_alloc(maze) == 1 &&
        read_maze_map(maze, file, 1) == SUCCESS &&
        fscanf(file, "%c", &ch) == 1 && ch == '\n' &&
        read_maze_map(maze, file, 2) == SUCCESS)) {
    fail = 1;
  }
  if (fail == 1 || maze->rows == 0 || maze->cols == 0) {
    maze_free(maze);
  }
  fclose(file);
  return maze;
}

int write_maze_to_file(maze_optimized_t* maze_optimized, char* file_name) {
  if (file_name == NULL || maze_optimized == NULL ||
      maze_optimized->map == NULL) {
    perror("maze wrong or no file to open\n");
    return WRITE_FILE_ERROR;
  }
  FILE* file = open_file_a(file_name);
  if (file == NULL) return WRITE_FILE_ERROR;
  fprintf(file, "%lu%c%lu\n", maze_optimized->rows, ' ', maze_optimized->cols);
  for (size_t i = 0; i < maze_optimized->rows; i++) {
    for (size_t j = 0; j < maze_optimized->cols; j++) {
      if (get_maze_optimized_right_border(maze_optimized, i, j) == 1) {
        fprintf(file, "%c", '1');
      } else {
        fprintf(file, "%c", '0');
      }
      if (j < maze_optimized->cols - 1) {
        fprintf(file, "%c", ' ');
      } else {
        fprintf(file, "\n");
      }
    }
  }

  fprintf(file, "\n");

  for (size_t i = 0; i < maze_optimized->rows; i++) {
    for (size_t j = 0; j < maze_optimized->cols; j++) {
      if (get_maze_optimized_bottom_border(maze_optimized, i, j) == 1) {
        fprintf(file, "%c", '1');
      } else {
        fprintf(file, "%c", '0');
      }
      if (j < maze_optimized->cols - 1) {
        fprintf(file, "%c", ' ');
      } else {
        fprintf(file, "\n");
      }
    }
  }
  fclose(file);
  return SUCCESS;
}