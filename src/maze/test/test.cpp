#include <gtest/gtest.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
    #include "../include/maze.h"
    #include "../../io/include/io.h"
}

#define NULL nullptr

TEST(MAZE_UTILS_TEST, get_bit) {
    unsigned ch1 = 0;
    EXPECT_EQ(get_bit(ch1, 0), 0);
    EXPECT_EQ(get_bit(ch1, 1), 0);
    EXPECT_EQ(get_bit(ch1, 2), 0);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    ch1 = 1;
    EXPECT_EQ(get_bit(ch1, 0), 1);
    EXPECT_EQ(get_bit(ch1, 1), 0);
    EXPECT_EQ(get_bit(ch1, 2), 0);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    ch1 = 2;
    EXPECT_EQ(get_bit(ch1, 0), 0);
    EXPECT_EQ(get_bit(ch1, 1), 1);
    EXPECT_EQ(get_bit(ch1, 2), 0);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    ch1 = 3;
    EXPECT_EQ(get_bit(ch1, 0), 1);
    EXPECT_EQ(get_bit(ch1, 1), 1);
    EXPECT_EQ(get_bit(ch1, 2), 0);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    ch1 = 4;
    EXPECT_EQ(get_bit(ch1, 0), 0);
    EXPECT_EQ(get_bit(ch1, 1), 0);
    EXPECT_EQ(get_bit(ch1, 2), 1);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    ch1 = 5;
    EXPECT_EQ(get_bit(ch1, 0), 1);
    EXPECT_EQ(get_bit(ch1, 1), 0);
    EXPECT_EQ(get_bit(ch1, 2), 1);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    ch1 = 6;
    EXPECT_EQ(get_bit(ch1, 0), 0);
    EXPECT_EQ(get_bit(ch1, 1), 1);
    EXPECT_EQ(get_bit(ch1, 2), 1);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    ch1 = 7;
    EXPECT_EQ(get_bit(ch1, 0), 1);
    EXPECT_EQ(get_bit(ch1, 1), 1);
    EXPECT_EQ(get_bit(ch1, 2), 1);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    ch1 = 8;
    EXPECT_EQ(get_bit(ch1, 0), 0);
    EXPECT_EQ(get_bit(ch1, 1), 0);
    EXPECT_EQ(get_bit(ch1, 2), 0);
    EXPECT_EQ(get_bit(ch1, 3), 1);
}


TEST(MAZE_UTILS_TEST, set_bit) {
    unsigned char ch1 = 0;

    unsigned char ch1_res = 1;

    set_bit(&ch1, 0, 1);
    EXPECT_EQ(ch1, ch1_res);
    set_bit(&ch1, 1, 1);
    EXPECT_EQ(ch1, 3);

    ch1 = 0;
    EXPECT_EQ(get_bit(ch1, 0), 0);
    EXPECT_EQ(get_bit(ch1, 1), 0);
    EXPECT_EQ(get_bit(ch1, 2), 0);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    set_bit(&ch1, 0, 1);
    EXPECT_EQ(get_bit(ch1, 0), 1);
    EXPECT_EQ(get_bit(ch1, 1), 0);
    EXPECT_EQ(get_bit(ch1, 2), 0);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    set_bit(&ch1, 1, 1);
    EXPECT_EQ(get_bit(ch1, 0), 1);
    EXPECT_EQ(get_bit(ch1, 1), 1);
    EXPECT_EQ(get_bit(ch1, 2), 0);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    set_bit(&ch1, 2, 1);
    EXPECT_EQ(get_bit(ch1, 0), 1);
    EXPECT_EQ(get_bit(ch1, 1), 1);
    EXPECT_EQ(get_bit(ch1, 2), 1);
    EXPECT_EQ(get_bit(ch1, 3), 0);

    set_bit(&ch1, 3, 1);
    EXPECT_EQ(get_bit(ch1, 0), 1);
    EXPECT_EQ(get_bit(ch1, 1), 1);
    EXPECT_EQ(get_bit(ch1, 2), 1);
    EXPECT_EQ(get_bit(ch1, 3), 1);

    set_bit(&ch1, 3, 0);
    EXPECT_EQ(get_bit(ch1, 0), 1);
    EXPECT_EQ(get_bit(ch1, 1), 1);
    EXPECT_EQ(get_bit(ch1, 2), 1);
    EXPECT_EQ(get_bit(ch1, 3), 0);
}

TEST(MAZE_UTILS_TEST, get_maze_borders_unvalid_args) {
    maze_optimized_t *maze_null = NULL;
    EXPECT_EQ(get_maze_optimized_left_border(maze_null, 0, 0), 1);
    EXPECT_EQ(get_maze_optimized_left_border(NULL, 0, 0), 1);
    maze_optimized_t maze_optimized_test_case = {0};
    maze_optimized_test_case.map = NULL;
    EXPECT_EQ(get_maze_optimized_upper_border(&maze_optimized_test_case, 0, 0), 1);
    EXPECT_EQ(get_maze_optimized_bottom_border(&maze_optimized_test_case, -1, -1), 1);
    unsigned char ch = ' ';
    unsigned char *ch_ptr = &ch;
    unsigned char **char_ptr_ptr = &ch_ptr;
    maze_optimized_test_case.map = char_ptr_ptr;
    maze_optimized_test_case.rows = 1;
    maze_optimized_test_case.cols = 1;
    EXPECT_EQ(get_maze_optimized_left_border(&maze_optimized_test_case, 1, 0), 1);
    EXPECT_EQ(get_maze_optimized_left_border(&maze_optimized_test_case, 0, 1), 1);
    EXPECT_EQ(get_maze_optimized_left_border(&maze_optimized_test_case, 2, 2), 1);
}

TEST(MAZE_ALLOCATORS, make_optimized_maze) {
    maze_t* maze = read_maze_from_file("./test/test_cases/maze_1.txt");
    EXPECT_NE(maze, NULL);
    maze_optimized_t* maze_optimazed = make_maze_optimized_from_maze(maze);
    EXPECT_NE(maze_optimazed, NULL);
    EXPECT_EQ(get_maze_optimized_left_border(maze_optimazed, 0, 0), 1);
    EXPECT_EQ(get_maze_optimized_bottom_border(maze_optimazed, 3, 3), 1);
    EXPECT_EQ(get_maze_optimized_right_border(maze_optimazed, 3, 3), 1);
    EXPECT_EQ(get_maze_optimized_upper_border(maze_optimazed, 2, 2), 1);
    EXPECT_EQ(get_maze_optimized_right_border(maze_optimazed, 1, 1), 0);
    maze_free(maze);
    free_maze_optimized(maze_optimazed);
}

TEST(MAZE_GENERATION, refill_sets_1) {
    size_t rows = 10;
    size_t cols = 10;
    maze_optimized_t* maze = new_maze_optimized(rows, cols);
    size_t* sets = (size_t*)malloc(sizeof(size_t) * rows * cols);
    EXPECT_NE(maze, NULL);
    EXPECT_NE(sets, NULL);
    refill_sets(maze, 0, sets);
    EXPECT_EQ(sets[0], 1);
    EXPECT_EQ(sets[1], 2);
    EXPECT_EQ(sets[2], 3);
    EXPECT_EQ(sets[3], 4);
    EXPECT_EQ(sets[4], 5);
    EXPECT_EQ(sets[5], 6);
    EXPECT_EQ(sets[6], 7);
    EXPECT_EQ(sets[7], 8);
    EXPECT_EQ(sets[8], 9);
    EXPECT_EQ(sets[9], 10);

    free(sets);
    free_maze_optimized(maze);
}

// TEST(MAZE_GENERATION, refill_sets_2) {
//     size_t rows = 10;
//     size_t cols = 8;
//     maze_optimized_t* maze = new_maze_optimized(rows, cols);
//     size_t* sets = (size_t*)malloc(sizeof(size_t) * rows * cols);
//     EXPECT_NE(maze, NULL);
//     EXPECT_NE(sets, NULL);
//     set_maze_optimized_upper_border(maze, 0, 2, false);
//     set_maze_optimized_upper_border(maze, 0, 5, false);
//     set_maze_optimized_upper_border(maze, 0, 7, false);
//     sets[0] = 4;
//     sets[1] = 4;
//     sets[2] = 1;
//     sets[3] = 4;
//     sets[4] = 3;
//     sets[5] = 5;
//     sets[6] = 2;
//     sets[7] = 9;
//     refill_sets(maze, 0, sets);
//     EXPECT_EQ(sets[0], 2);
//     EXPECT_EQ(sets[1], 3);
//     EXPECT_EQ(sets[2], 1);
//     EXPECT_EQ(sets[3], 4);
//     EXPECT_EQ(sets[4], 6);
//     EXPECT_EQ(sets[5], 5);
//     EXPECT_EQ(sets[6], 7);
//     EXPECT_EQ(sets[7], 9);

//     free(sets);
//     free_maze_optimized(maze);
// }

// TEST(MAZE_GENERATION, refill_sets_3) {
//     size_t rows = 10;
//     size_t cols = 8;
//     maze_optimized_t* maze = new_maze_optimized(rows, cols);
//     size_t* sets = (size_t*)malloc(sizeof(size_t) * rows * cols);
//     EXPECT_NE(maze, NULL);
//     EXPECT_NE(sets, NULL);
//     set_maze_optimized_upper_border(maze, 0, 0, false);
//     set_maze_optimized_upper_border(maze, 0, 3, false);
//     set_maze_optimized_upper_border(maze, 0, 5, false);
//     set_maze_optimized_upper_border(maze, 0, 6, false);
//     sets[0] = 1;
//     sets[1] = 1;
//     sets[2] = 1;
//     sets[3] = 4;
//     sets[4] = 4;
//     sets[5] = 6;
//     sets[6] = 6;
//     sets[7] = 6;
//     refill_sets(maze, 0, sets);
//     EXPECT_EQ(sets[0], 1);
//     EXPECT_EQ(sets[1], 2);
//     EXPECT_EQ(sets[2], 3);
//     EXPECT_EQ(sets[3], 4);
//     EXPECT_EQ(sets[4], 5);
//     EXPECT_EQ(sets[5], 6);
//     EXPECT_EQ(sets[6], 6);
//     EXPECT_EQ(sets[7], 7);

//     free(sets);
//     free_maze_optimized(maze);
// }

// TEST(MAZE_GENERATION, union_sets) {
//     size_t rows = 8;
//     size_t cols = 8;
//     maze_optimized_t* maze = new_maze_optimized(rows, cols);
//     size_t* sets = (size_t*)malloc(sizeof(size_t) * rows * cols);
//     EXPECT_NE(maze, NULL);
//     EXPECT_NE(sets, NULL);
//     refill_sets(maze, 0, sets);
    
//     set_maze_optimized_right_border(maze, 0, 2, true);
//     set_maze_optimized_left_border(maze, 0, 3, true);
//     set_maze_optimized_right_border(maze, 0, 4, true);
//     set_maze_optimized_left_border(maze, 0, 5, true);

//     add_bottom_walls(maze, 0);

//     EXPECT_EQ(get_maze_optimized_bottom_border(maze, 0, 0), true);
//     EXPECT_EQ(get_maze_optimized_bottom_border(maze, 0, 1), true);
//     EXPECT_EQ(get_maze_optimized_bottom_border(maze, 0, 2), true);
//     EXPECT_EQ(get_maze_optimized_bottom_border(maze, 0, 3), true);
//     EXPECT_EQ(get_maze_optimized_bottom_border(maze, 0, 4), true);
//     EXPECT_EQ(get_maze_optimized_bottom_border(maze, 0, 5), true);
//     EXPECT_EQ(get_maze_optimized_bottom_border(maze, 0, 6), true);
//     EXPECT_EQ(get_maze_optimized_bottom_border(maze, 0, 7), true);

//     free_maze_optimized(maze);
//     free(sets);
    
// }

// TEST(MAZE_GENERATION, add_bottom_walls) {
//     size_t rows = 6;
//     size_t cols = 6;
//     maze_optimized_t* maze = new_maze_optimized(rows, cols);
//     size_t* sets = (size_t*)malloc(sizeof(size_t) * rows * cols);
//     EXPECT_NE(maze, NULL);
//     EXPECT_NE(sets, NULL);
//     refill_sets(maze, 0, sets);
    
//     set_maze_optimized_right_border(maze, 0, 1, true);
//     set_maze_optimized_left_border(maze, 0, 2, true);
//     set_maze_optimized_right_border(maze, 0, 4, true);
//     set_maze_optimized_left_border(maze, 0, 5, true);

//     union_sets(maze, sets, 0);


//     free_maze_optimized(maze);
//     free(sets);
// }

TEST(MAZE_GENERATION, maze_generation) {
    maze_optimized_t* maze = generate_maze(10, 10);
    EXPECT_NE(maze, NULL);
    free_maze_optimized(maze);
}

TEST(MAZE_SOLVING, maze_one_way_1){
    maze_t* maze = read_maze_from_file("./test/test_cases/maze_one_way_1.txt");
    maze_optimized_t* maze_opt = make_maze_optimized_from_maze(maze);
    EXPECT_NE(maze_opt, NULL);
    print_maze(stderr, maze_opt);
    node* start_node = solve_maze(maze_opt, 0, 3, 3, 3);
    EXPECT_EQ(start_node, NULL);
    print_maze(stderr, maze_opt);
    free_nodes(start_node);
    free_maze_optimized(maze_opt);
    maze_free(maze);
}

TEST(MAZE_SOLVING, maze_one_way_2){
    maze_t* maze = read_maze_from_file("./test/test_cases/maze_one_way_2.txt");
    maze_optimized_t* maze_opt = make_maze_optimized_from_maze(maze);
    EXPECT_NE(maze_opt, NULL);
    print_maze(stderr, maze_opt);
    node* start_node = solve_maze(maze_opt, 0, 3, 3, 0);
    print_maze(stderr, maze_opt);
    EXPECT_EQ(start_node, NULL);
    free_maze_optimized(maze_opt);
    maze_free(maze);
}

TEST(MAZE_SOLVING, maze_no_way_1){
    maze_t* maze = read_maze_from_file("./test/test_cases/maze_one_way_1.txt");
    maze_optimized_t* maze_opt = make_maze_optimized_from_maze(maze);
    EXPECT_NE(maze_opt, NULL);
    print_maze(stderr, maze_opt);
    node* start_node = solve_maze(maze_opt, 0, 3, 3, 2);
    print_nodes(stderr, start_node);
    EXPECT_EQ(start_node, NULL);
    print_maze(stderr, maze_opt);
    free_maze_optimized(maze_opt);
    maze_free(maze);
}

TEST(MAZE_SOLVING, maze_no_way_2){
    maze_t* maze = read_maze_from_file("./test/test_cases/maze_one_way_2.txt");
    maze_optimized_t* maze_opt = make_maze_optimized_from_maze(maze);
    EXPECT_NE(maze_opt, NULL);
    print_maze(stderr, maze_opt);
    node* start_node = solve_maze(maze_opt, 0, 3, 3, 2);
    print_maze(stderr, maze_opt);
    EXPECT_EQ(start_node, NULL);
    free_maze_optimized(maze_opt);
    maze_free(maze);
}

TEST(MAZE_SOLVING, brutal_1){
    maze_t* maze = read_maze_from_file("./test/test_cases/maze_1.txt");
    maze_optimized_t* maze_opt = make_maze_optimized_from_maze(maze);
    EXPECT_NE(maze_opt, NULL);
    print_maze(stderr, maze_opt);
    node* start_node = solve_maze(maze_opt, 0, 0, 3, 0);
    EXPECT_EQ(start_node, NULL);
    print_maze(stderr, maze_opt);
    free_maze_optimized(maze_opt);
    maze_free(maze);
}

// TEST(MAZE_SOLVING, brutal_5){
//     maze_t* maze = read_maze_from_file("./test/test_cases/maze_5.txt");
//     maze_optimized_t* maze_opt = make_maze_optimized_from_maze(maze);
//     EXPECT_NE(maze_opt, NULL);
//     print_maze(stderr, maze_opt);
//     node* start_node = solve_maze(maze_opt, 0, 0, 19, 19);
//     EXPECT_EQ(start_node, NULL);
//     print_maze(stderr, maze_opt);
//     free_maze_optimized(maze_opt);
//     maze_free(maze);
// }

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}