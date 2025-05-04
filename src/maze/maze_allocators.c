#include <limits.h>
#include <stdlib.h>

#include "./include/maze.h"

maze_optimized_t* maze_optimized_alloc(size_t rows, size_t cols);
int optimized_map_maze_alloc(maze_optimized_t* maze_optimized);
void init_maze_optimized_boarders(maze_optimized_t* maze_optimized);
void set_state_not_valid(maze_optimized_t* maze_optimized);

maze_optimized_t* new_maze_optimized(size_t rows, size_t cols) {
  if (rows == 0 || cols == 0) return NULL;
  maze_optimized_t* maze_optimized = maze_optimized_alloc(rows, cols);
  if (maze_optimized == NULL) return NULL;
  if (optimized_map_maze_alloc(maze_optimized) == BAD_ALLOCATION) {
    free(maze_optimized);
    return NULL;
  }
  init_maze_optimized_boarders(maze_optimized);
  set_state_not_valid(maze_optimized);
  return maze_optimized;
}

maze_optimized_t* maze_optimized_alloc(size_t rows, size_t cols) {
  maze_optimized_t* maze_optimized = malloc(sizeof(maze_optimized_t));
  if (maze_optimized == NULL) return NULL;

  maze_optimized->rows = rows;
  maze_optimized->cols = cols;
  maze_optimized->map = NULL;
  return maze_optimized;
}

int optimized_map_maze_alloc(maze_optimized_t* maze_optimized) {
  unsigned char** optimized_map =
      calloc(maze_optimized->rows, sizeof(unsigned char*));
  if (optimized_map == NULL) return BAD_ALLOCATION;

  int return_code = SUCCESS;
  for (size_t i = 0; i < maze_optimized->rows && return_code == SUCCESS; i++) {
#ifdef ALLOC_LIMIT
    optimized_map[i] = NULL;
#else
    optimized_map[i] = calloc(maze_optimized->cols, sizeof(unsigned char));
#endif
    if (optimized_map[i] == NULL) {
      for (size_t j = i - 1; j != ULLONG_MAX; j--) {
        if (optimized_map[j] != NULL) free(optimized_map[j]);
      }
      free(optimized_map);
      return_code = BAD_ALLOCATION;
      break;
    }
  }
  if (return_code == SUCCESS) {
    maze_optimized->map = optimized_map;
  }
  return return_code;
}

void init_maze_optimized_boarders(maze_optimized_t* maze_optimized) {
  size_t rows = maze_optimized->rows;
  size_t cols = maze_optimized->cols;
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      if (i == 0) {
        set_maze_optimized_upper_border(maze_optimized, i, j, true);
      } else {
        set_maze_optimized_upper_border(maze_optimized, i, j, false);
      }

      if (i == rows - 1) {
        set_maze_optimized_bottom_border(maze_optimized, i, j, true);
      } else {
        set_maze_optimized_bottom_border(maze_optimized, i, j, false);
      }

      if (j == 0) {
        set_maze_optimized_left_border(maze_optimized, i, j, true);
      } else {
        set_maze_optimized_left_border(maze_optimized, i, j, false);
      }

      if (j == cols - 1) {
        set_maze_optimized_right_border(maze_optimized, i, j, true);
      } else {
        set_maze_optimized_right_border(maze_optimized, i, j, false);
      }
    }
  }
}

void set_state_not_valid(maze_optimized_t* maze_optimized) {
  for (size_t i = 0; i < maze_optimized->rows; i++) {
    for (size_t j = 0; j < maze_optimized->cols; j++) {
      set_maze_optimized_not_valid_path(maze_optimized, i, j);
    }
  }
}

maze_optimized_t* make_maze_optimized_from_maze(maze_t* maze) {
  if (maze == NULL) return NULL;
  if (maze->map1 == NULL || maze->map2 == NULL) return NULL;
  if (maze->rows <= 0 || maze->cols <= 0) return NULL;
  maze_optimized_t* maze_optimized =
      maze_optimized_alloc(maze->rows, maze->cols);
  if (maze_optimized == NULL) return NULL;
  if (optimized_map_maze_alloc(maze_optimized) == BAD_ALLOCATION) {
    free(maze_optimized);
    return NULL;
  }

  init_maze_optimized_boarders(maze_optimized);

  for (size_t i = 0; i < maze_optimized->rows; i++) {
    for (size_t j = 0; j < maze_optimized->cols; j++) {
      char ch = maze->map1[i][j];
      if (ch == '1') {
        set_maze_optimized_right_border(maze_optimized, i, j, true);
      } else {
        set_maze_optimized_right_border(maze_optimized, i, j, false);
      }

      if (j > 0) {
        if (get_maze_optimized_right_border(maze_optimized, i, j - 1) == 1) {
          set_maze_optimized_left_border(maze_optimized, i, j, true);
        } else {
          set_maze_optimized_left_border(maze_optimized, i, j, false);
        }
      }
    }
  }

  for (size_t i = 0; i < maze_optimized->rows; i++) {
    for (size_t j = 0; j < maze_optimized->cols; j++) {
      char ch = maze->map2[i][j];
      if (ch == '1') {
        set_maze_optimized_bottom_border(maze_optimized, i, j, true);
      } else {
        set_maze_optimized_bottom_border(maze_optimized, i, j, false);
      }

      if (i > 0) {
        if (get_maze_optimized_bottom_border(maze_optimized, i - 1, j) == 1) {
          set_maze_optimized_upper_border(maze_optimized, i, j, true);
        } else {
          set_maze_optimized_upper_border(maze_optimized, i, j, false);
        }
      }
    }
  }
  set_state_not_valid(maze_optimized);
  return maze_optimized;
}

void free_maze_optimized(maze_optimized_t* maze_optimized) {
  if (!maze_optimized) return;
  if (maze_optimized->map) {
    for (size_t i = 0; i < maze_optimized->rows; i++) {
      if (maze_optimized->map[i]) free(maze_optimized->map[i]);
    }
    free(maze_optimized->map);
  }
  free(maze_optimized);
}

// allocators for normal maze

int map_maze_alloc(maze_t* maze) {
  if (maze == NULL) return 0;
  char** map_maze1 = calloc(maze->rows, sizeof(char*));
  char** map_maze2 = calloc(maze->rows, sizeof(char*));
  if (map_maze1 == NULL || map_maze2 == NULL) {
    if (map_maze1 != NULL) free(map_maze1);
    if (map_maze2 != NULL) free(map_maze2);
    return 0;
  }

  int return_code = SUCCESS;
  for (size_t i = 0; i < maze->rows && return_code == SUCCESS; i++) {
#ifdef ALLOC_LIMIT
    map_maze1[i] = NULL;
    map_maze2[i] = NULL;
#else
    map_maze1[i] = calloc(maze->cols, sizeof(char));
    map_maze2[i] = calloc(maze->cols, sizeof(char));
#endif
    if (map_maze1[i] == NULL || map_maze2[i] == NULL) {
      for (size_t j = i - 1; j != ULLONG_MAX; j--) {
        if (map_maze1[j] != NULL) free(map_maze1[j]);
        if (map_maze2[j] != NULL) free(map_maze2[j]);
      }
      free(map_maze1);
      free(map_maze2);
      return_code = BAD_ALLOCATION;
      break;
    }
  }
  if (return_code == SUCCESS) {
    maze->map1 = map_maze1;
    maze->map2 = map_maze2;
  }
  return return_code;
}

void maze_free(maze_t* maze) {
  if (maze == NULL) return;
  for (size_t i = 0; i < maze->rows; i++) {
    if (maze->map1 != NULL && maze->map1[i] != NULL) free(maze->map1[i]);
    if (maze->map2 != NULL && maze->map2[i] != NULL) free(maze->map2[i]);
  }
  if (maze->map2 != NULL) free(maze->map1);
  if (maze->map2 != NULL) free(maze->map2);
  free(maze);
}
