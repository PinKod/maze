#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../cave/io/include/io_cave.h"
#include "../includes/defines.h"
#include "../io/include/io.h"
#include "../maze/include/maze.h"

maze_optimized_t* handle_load_maze();
maze_optimized_t* handle_generate_maze();
void handle_solve_maze(maze_optimized_t* maze);
void handle_cave_generation();

int main() {
  int choice;
  maze_optimized_t* maze = NULL;

  int exit_flag = 1;
  while (exit_flag) {
    printf("\nMain Menu:\n");
    printf("1. Load maze from file\n");
    printf("2. Generate ideal maze\n");
    printf("3. Solve maze\n");
    printf("4. Generate cave using cellular automaton\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
      case 1:
        free_maze_optimized(maze);
        maze = handle_load_maze();
        break;
      case 2:
        free_maze_optimized(maze);
        maze = handle_generate_maze();
        break;
      case 3:
        handle_solve_maze(maze);
        break;
      case 4:
        handle_cave_generation();
        break;
      case 5:
        exit_flag = 0;
      default:
        printf("Invalid choice. Try again.\n");
    }
  }
  free_maze_optimized(maze);
  return 0;
}

maze_optimized_t* handle_load_maze() {
  char filename[256];
  printf("Enter maze filename: ");
  scanf("%255s", filename);  // Prevent buffer overflow
  maze_optimized_t* maze =
      make_maze_optimized_from_maze(read_maze_from_file(filename));
  if (maze) {
    print_maze(stdout, maze);
  } else {
    printf("Failed to load maze.\n");
  }
  return maze;
}

maze_optimized_t* handle_generate_maze() {
  int width, height;
  printf("Enter maze dimensions (width height): ");
  scanf("%d %d", &width, &height);
  maze_optimized_t* maze = generate_maze(height, width);
  if (maze) {
    print_maze(stdout, maze);
    char save_filename[256];
    printf("Save generated maze to file: ");
    scanf("%255s", save_filename);
    write_maze_to_file(maze, save_filename);
  }
  return maze;
}

void handle_solve_maze(maze_optimized_t* maze) {
  if (!maze) {
    printf("No maze loaded.\n");
    return;
  }
  int start_x, start_y, end_x, end_y;
  printf("Enter start coordinates (x y): ");
  scanf("%d %d", &start_x, &start_y);
  printf("Enter end coordinates (x y): ");
  scanf("%d %d", &end_x, &end_y);
  solve_maze(maze, start_x, start_y, end_x, end_y);
  print_maze(stdout, maze);
}

void handle_cave_generation() {
  char cave_filename[256];
  printf("Enter cave filename to load: ");
  scanf("%255s", cave_filename);
  cave* cave_ptr = read_cave_from_file(cave_filename);
  if (!cave_ptr) {
    printf("Failed to load cave.\n");
    return;
  }
  int birth, death;
  printf("Enter birth threshold (0-8): ");
  scanf("%d", &birth);
  printf("Enter death threshold (0-8): ");
  scanf("%d", &death);

  int mode;
  printf("Choose mode (1-step, 2-auto): ");
  scanf("%d", &mode);
  if (mode == 2) {
    int delay_ms;
    printf("Enter delay in milliseconds: ");
    scanf("%d", &delay_ms);
    int stop_flag = 1;
    while (stop_flag) {
      stop_flag = update_cave(cave_ptr, birth, death);
      print_cave(cave_ptr);
      usleep(delay_ms * 1000);
    }
  } else {
    int stop_flag = 1;
    while (1) {
      char input[2];
      printf("Press Enter to step or 'q' to quit: ");
      if (fgets(input, 2, stdin) == NULL) {
        break;
      }
      if (input[0] == 'q') {
        break;
      }
      stop_flag = update_cave(cave_ptr, birth, death);
      print_cave(cave_ptr);
    }
  }
  free_cave(cave_ptr);
}