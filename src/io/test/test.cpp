#include <gtest/gtest.h>
#include <stdlib.h>

extern "C" {
    #include "./../include/io.h"
}


#define NULL nullptr

TEST(TEST_FILE_IO, read_maze_from_file) {
    EXPECT_EQ(read_maze_from_file(NULL), NULL);
    EXPECT_EQ(read_maze_from_file("no_file"), NULL);
    maze_t* maze = NULL;
    maze = read_maze_from_file("./test/test_cases/maze_1.txt");
    EXPECT_NE(maze, NULL);
    char    result_maze1[4][4] = {{'0', '0', '0', '1'}, {'1', '0', '1', '1'}, {'0', '1', '0', '1'}, {'0', '0', '0', '1'}};
    char    result_maze2[4][4] = {{'1', '0', '1', '0'}, {'0', '0', '1', '0'}, {'1', '1', '0', '1'}, {'1', '1', '1', '1'}};
    char**  ptr_result_maze1 = (char**)calloc(4, sizeof(char*));
    char**  ptr_result_maze2 = (char**)calloc(4, sizeof(char*));

    for (int i = 0; i < 4; i++) {
        ptr_result_maze1[i] = (char*)calloc(4, sizeof(char));
        ptr_result_maze2[i] = (char*)calloc(4, sizeof(char));
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ptr_result_maze1[i][j] = result_maze1[i][j];
            ptr_result_maze2[i][j] = result_maze2[i][j];
        }
    }

    maze_t result_maze = {
        .rows = 4,
        .cols = 4,
        .map1 = ptr_result_maze1,
        .map2 = ptr_result_maze2,
    };

    // maze_optimazed_t *maze_opt = make_maze_optimazed_from_maze(maze);
    // maze_optimazed_t *res_opt = make_maze_optimazed_from_maze(&result_maze);
    // EXPECT_EQ(maze_opt);
    // EXPECT_EQ(res_opt);
    // fprintf(stderr, "\n");
    // print_maze(stderr, maze_opt);
    // fprintf(stderr, "\n");
    // print_maze(stderr, res_opt);
    // fprintf(stderr, "\n");

    EXPECT_EQ(compare_maze_matrix(maze, &result_maze), EQUAL);

    maze_free(maze);
    for (int i = 0; i < 4; i++) {
        free(ptr_result_maze1[i]);
        free(ptr_result_maze2[i]);
    }
    free(ptr_result_maze1);
    free(ptr_result_maze2);
}

TEST(TEST_JSON_IO, maze_to_json) {
    maze_t* maze_n = read_maze_from_file("./test/test_cases/maze_1.txt");
    EXPECT_NE(maze_n, NULL);
    maze_optimized_t* maze = make_maze_optimized_from_maze(maze_n);
    
    EXPECT_NE(maze, NULL);
    char* json = maze_to_json(maze);
    EXPECT_NE(json, NULL);
    char res[] ="{\"r\":\"4\",\"c\":\"4\",\"map\":[\"1011\",\"1000\",\"1010\",\"1100\",\"1101\",\"0001\",\"1110\",\"0101\",\"0011\",\"0110\",\"1001\",\"0110\",\"1011\",\"1010\",\"0010\",\"1110\"]}";
    EXPECT_STREQ(json, res);
    // printf("%s\n", json);
    free(json);
    maze_free(maze_n);
    free_maze_optimized(maze);
}  

TEST(MAZE_GENERATION, init_maze_optimized_boarders) {
    maze_optimized_t* maze = new_maze_optimized(10, 10);
    init_maze_optimized_boarders(maze);
    
    size_t rows = maze->rows;
    size_t cols = maze->cols;
    for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < cols; j++) {
            if(i == 0) {
                EXPECT_EQ(get_maze_optimized_upper_border(maze, i, j), true);
            }
            else {
                EXPECT_EQ(get_maze_optimized_upper_border(maze, i, j), false);
            }
            
            
            if(i == rows - 1) {
                EXPECT_EQ(get_maze_optimized_bottom_border(maze, i, j), true);
            }
            else {
                EXPECT_EQ(get_maze_optimized_bottom_border(maze, i, j), false);
            }

            if(j == 0) {
                EXPECT_EQ(get_maze_optimized_left_border(maze, i, j), true);
            }
            else {
                EXPECT_EQ(get_maze_optimized_left_border(maze, i, j), false);
            }

            if(j == cols - 1) {
                EXPECT_EQ(get_maze_optimized_right_border(maze, i, j), true);
            }
            else {
                EXPECT_EQ(get_maze_optimized_right_border(maze, i, j), false);
            }
        }
    }

    free_maze_optimized(maze);
}

TEST(MAZE_GENERATION, maze_generation) {
    maze_optimized_t* maze = generate_maze(10, 10);
    print_maze(stderr, maze);
    free_maze_optimized(maze);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    return 0;
}