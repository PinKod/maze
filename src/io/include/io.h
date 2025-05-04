#ifndef IO_H
#define IO_H

#include <stdio.h>

#include "./../../includes/defines.h"
#include "./../../maze/include/maze.h"

// file io functions
maze_t* read_maze_from_file(char* file_name);
int write_maze_to_file(maze_optimized_t* maze_optimized, char* file_name);
void print_maze(FILE* fd, maze_optimized_t* maze_optimized);

// json io functions
char* maze_to_json(maze_optimized_t* maze);

#endif  // IO_H