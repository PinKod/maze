#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/maze.h"

int compare_maze_matrix(maze_t* maze1, maze_t* maze2) {
  if (maze1 == NULL || maze2 == NULL) return NOT_EQUAL;
  if (maze1->map1 == NULL || maze1->map2 == NULL || maze2->map1 == NULL ||
      maze2->map2 == NULL)
    return NOT_EQUAL;
  if (maze1->rows != maze2->rows) return NOT_EQUAL;
  if (maze1->cols != maze2->cols) return NOT_EQUAL;
  int equal = EQUAL;
  for (size_t i = 0; i < maze1->rows && equal == EQUAL; i++) {
    for (size_t j = 0; j < maze1->cols; j++) {
      if (maze1->map1[i][j] != maze2->map1[i][j] ||
          maze1->map2[i][j] != maze2->map2[i][j]) {
        equal = NOT_EQUAL;
        break;
      }
    }
  }
  return equal;
}

int compare_maze_in_files(char* file_name_1, char* file_name_2) {
  if (file_name_1 == NULL || file_name_2 == NULL) return NOT_EQUAL;
  FILE* file1 = fopen(file_name_1, "r");
  FILE* file2 = fopen(file_name_2, "r");
  if (file1 == NULL || file2 == NULL) {
    if (file1 != NULL) fclose(file1);
    if (file2 != NULL) fclose(file2);
    return NOT_EQUAL;
  }
  int equal = EQUAL;
  char* line1 = NULL;
  char* line2 = NULL;
#ifdef _GNU_SOURCE
  size_t size = MAX_SIZE * 2 + 10;
  size_t* size_ptr = &size;
  while (getline(&line1, size_ptr, file1) != EOF &&
         getline(&line2, size_ptr, file2) != EOF) {
    if (strcmp(line1, line2) != 0) {
      equal = NOT_EQUAL;
      break;
    }
    if (line1 != NULL) free(line1);
    if (line2 != NULL) free(line2);
  }
  if (line1 != NULL) free(line1);
  if (line2 != NULL) free(line2);

#endif
  return equal;
}