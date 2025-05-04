#include "./include/io.h"

void print_maze(FILE* fd, maze_optimized_t* maze) {
  size_t maze_rows = maze->rows;
  size_t maze_cols = maze->cols;
  fprintf(fd, "\n");
  for (size_t i = 0; i < maze_rows; i++) {
    for (size_t j = 0; j < maze_cols; j++) {
      int lb = get_maze_optimized_left_border(maze, i, j);
      int rb = get_maze_optimized_right_border(maze, i, j);
      int bb = get_maze_optimized_bottom_border(maze, i, j);
      int up = get_maze_optimized_upper_border(maze, i, j);

      size_t state = get_maze_optimized_valid_path_state(maze, i, j);

      size_t checked = get_maze_optimized_state(maze, i, j);
      fprintf(fd, "%c", '+');

      if (up == 1) {
        fprintf(fd, "%c", '-');
      } else {
        // if (state == 1) {
        //     fprintf(fd, GREEN "%c" RESET, '#');
        // } else if (checked == 1) {
        //     fprintf(fd, WHITE "%c" RESET, '#');
        // } else if (checked == 0) {
        //     fprintf(fd, BLU "%c" RESET, '#');
        // } else {
        //     fprintf(fd, RED "%c" RESET, '#');
        // }
        if (checked == 0 && state != 1) {
          fprintf(fd, BLU "%c" RESET, '#');
        } else {
          if (state == 1) {
            fprintf(fd, GREEN "%c" RESET, '#');
          } else {
            fprintf(fd, RED "%c" RESET, '#');
          }
        }
      }

      fprintf(fd, "%c", '+');
    }
    fprintf(fd, "\n");

    for (size_t j = 0; j < maze_cols; j++) {
      int lb = get_maze_optimized_left_border(maze, i, j);
      int rb = get_maze_optimized_right_border(maze, i, j);

      int state = get_maze_optimized_valid_path_state(maze, i, j);

      int checked = get_maze_optimized_state(maze, i, j);

      if (lb == 1) {
        fprintf(fd, "%c", '|');
      } else {
        // if (state == 1) {
        //     fprintf(fd, GREEN "%c" RESET, '#');
        // } else if (checked == 1) {
        //     fprintf(fd, WHITE "%c" RESET, '#');
        // } else if (checked == 0) {
        //     fprintf(fd, BLU "%c" RESET, '#');
        // } else {
        //     fprintf(fd, RED "%c" RESET, '#');
        // }
        if (checked == 0 && state != 1) {
          fprintf(fd, BLU "%c" RESET, '#');
        } else {
          if (state == 1) {
            fprintf(fd, GREEN "%c" RESET, '#');
          } else {
            fprintf(fd, RED "%c" RESET, '#');
          }
        }
      }

      // if (state == 1) {
      //     fprintf(fd, GREEN "%c" RESET, '#');
      // } else if (checked == 1) {
      //     fprintf(fd, WHITE "%c" RESET, '#');
      // } else if (checked == 0) {
      //     fprintf(fd, BLU "%c" RESET, '#');
      // } else {
      //     fprintf(fd, RED "%c" RESET, '#');
      // }
      if (checked == 0 && state != 1) {
        fprintf(fd, BLU "%c" RESET, '#');
      } else {
        if (state == 1) {
          fprintf(fd, GREEN "%c" RESET, '#');
        } else {
          fprintf(fd, RED "%c" RESET, '#');
        }
      }

      if (rb == 1) {
        fprintf(fd, "%c", '|');
      } else {
        // if (state == 1) {
        //     fprintf(fd, GREEN "%c" RESET, '#');
        // } else if (checked == 1) {
        //     fprintf(fd, WHITE "%c" RESET, '#');
        // } else if (checked == 0) {
        //     fprintf(fd, BLU "%c" RESET, '#');
        // } else {
        //     fprintf(fd, RED "%c" RESET, '#');
        // }
        if (checked == 0 && state != 1) {
          fprintf(fd, BLU "%c" RESET, '#');
        } else {
          if (state == 1) {
            fprintf(fd, GREEN "%c" RESET, '#');
          } else {
            fprintf(fd, RED "%c" RESET, '#');
          }
        }
      }
    }
    fprintf(fd, "\n");
  }

  for (size_t j = 0; j < maze_cols; j++) {
    int lb = get_maze_optimized_left_border(maze, maze->rows - 1, j);
    int rb = get_maze_optimized_right_border(maze, maze->rows - 1, j);
    int bb = get_maze_optimized_bottom_border(maze, maze->rows - 1, j);
    int up = get_maze_optimized_upper_border(maze, maze->rows - 1, j);

    fprintf(fd, "%c", '+');

    if (bb == 1) {
      fprintf(fd, "%c", '-');
    } else {
      fprintf(fd, "%c", '#');
    }

    fprintf(fd, "%c", '+');
  }

  fprintf(fd, "\n\n");
}