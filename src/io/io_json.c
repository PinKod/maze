#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <valgrind/memcheck.h>

#include "./include/io.h"

int count_digits(unsigned long long n) {
  char buffer[21];
  sprintf(buffer, "%llu", n);
  return strlen(buffer);
}

size_t json_len_counter(maze_optimized_t* maze) {
  size_t json_len = 24;
  int rows_len = count_digits(maze->rows);
  int cols_len = count_digits(maze->cols);
  size_t cells = maze->rows * maze->cols;
  size_t cells_info_size = cells * 4;
  size_t cells_quotes = cells * 2;
  size_t cells_commas = cells - 1;
  json_len = json_len + rows_len + cols_len + cells_info_size + cells_quotes +
             cells_commas;
  return json_len;
}

int ull_to_str(char* ptr, unsigned long long num) {
  int len = 0;
  unsigned long long temp = num;

  do {
    len++;
    temp /= 10;
  } while (temp > 0);

  ptr[len] = '\0';
  for (int i = len - 1; i >= 0; i--) {
    ptr[i] = '0' + (num % 10);
    num /= 10;
  }
  return len;
}

char* map_to_json(maze_optimized_t* maze, char* ptr) {
  char quote[] = "\"";
  char comma[] = ",";
  for (size_t i = 0; i < maze->rows; i++) {
    for (size_t j = 0; j < maze->cols; j++) {
      strcpy(ptr, quote);
      ptr += strlen(quote);

      sprintf(ptr, "%01d", get_maze_optimized_upper_border(maze, i, j));
      ptr += 1;
      sprintf(ptr, "%01d", get_maze_optimized_right_border(maze, i, j));
      ptr += 1;
      sprintf(ptr, "%01d", get_maze_optimized_bottom_border(maze, i, j));
      ptr += 1;
      sprintf(ptr, "%01d", get_maze_optimized_left_border(maze, i, j));
      ptr += 1;

      strcpy(ptr, quote);
      ptr += strlen(quote);
      strcpy(ptr, comma);
      ptr += strlen(comma);
    }
  }
  ptr -= 1;
  return ptr;
}

char* maze_to_json(maze_optimized_t* maze) {
  if (maze == NULL || maze->map == NULL || maze->rows == 0 || maze->cols == 0)
    return NULL;
  size_t json_len = json_len_counter(maze);
  char* res = malloc(json_len * sizeof(char) + 1);
  if (res == NULL) return NULL;
  char* ptr = res;
  char header_1[] = "{\"r\":\"";
  char header_2[] = "\",\"c\":\"";
  char header_3[] = "\",\"map\":[";
  char quote[] = "\"";
  char comma[] = ",";

  strcpy(ptr, header_1);
  ptr += strlen(header_1);

  ptr += ull_to_str(ptr, maze->rows);

  strcpy(ptr, header_2);
  ptr += strlen(header_2);

  ptr += ull_to_str(ptr, maze->cols);

  strcpy(ptr, header_3);
  ptr += strlen(header_3);

  ptr = map_to_json(maze, ptr);

  char header_4[] = "]}";
  strcpy(ptr, header_4);

  return res;
}