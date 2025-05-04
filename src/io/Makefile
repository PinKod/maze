CC = gcc
FLAGS = -Wall -Wextra -std=c11 -g -O3 
LINUX_FLAGS = -lsubunit -lrt -lpthread -lm -static
COVERAGE_FLAGS = --coverage -fprofile-arcs -ftest-coverage
ADDRESS_SANITIZER_FLAGS = -fsanitize=address -fsanitize=leak
LD_ADDRESS_SANITIZER_FLAGS = -static-libasan
OBJ_DIR = ./bin/obj/
BIN_DIR = ./bin/
H_LIB = io_h_lib.a
MKDIR_EXISTING = mkdir -p

OS := $(shell uname -s)
ifeq ($(OS),Linux)
	OPEN_CMD = xdg-open
endif

ifeq ($(OS),Darwin)
	OPEN_CMD = open
endif

test_lib : clean
	@$(MKDIR_EXISTING) $(BIN_DIR)
	@$(MKDIR_EXISTING) $(OBJ_DIR)
	$(CC) $(FLAGS) $(LINUX_FLAGS) -c *.c
	@mv *.o $(OBJ_DIR)
	ar rc $(H_LIB) $(OBJ_DIR)*.o
	ranlib $(H_LIB)
	@mv $(H_LIB) $(BIN_DIR)

test_lib_coverage : clean
	@$(MKDIR_EXISTING) $(BIN_DIR)
	@$(MKDIR_EXISTING) $(OBJ_DIR)
	$(CC) $(FLAGS) $(LINUX_FLAGS) -c *.c $(COVERAGE_FLAGS)
	@mv *.o $(OBJ_DIR)
	ar rc $(H_LIB) $(OBJ_DIR)*.o
	ranlib $(H_LIB)
	@mv $(H_LIB) $(BIN_DIR)

test_lib_address_sanitizer : clean
	@$(MKDIR_EXISTING) $(BIN_DIR)
	@$(MKDIR_EXISTING) $(OBJ_DIR)
	$(CC) $(FLAGS) $(LINUX_FLAGS) -c *.c $(ADDRESS_SANITIZER_FLAGS)
	@mv *.o $(OBJ_DIR)
	ar rc $(H_LIB) $(OBJ_DIR)*.o
	ranlib $(H_LIB)
	@mv $(H_LIB) $(BIN_DIR)

CXX = g++
CPPFLAGS = -isystem ./include
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread -lgtest -lpthread -O3
LDFLAGS = -pthread
EXECUTE_FLAGS = --gtest_repeat=1 --gtest_break_on_failure=0
SUPPRESS_TEST_WARNINGS = -w 
TEST_SRC = ./test/
TEST_TARGET = test_io_h   ##target name change for another dir
OBJ_DIR_TEST = ./bin/obj/
BIN_DIR_TEST = ./bin/
TEMP_FILE = tmp.txt
NULL_OUTPUT_STREAM = > /dev/null
MAZE_H_LIB = maze_h_lib.a

maze_test_lib_build :
	make -C ./../maze test_lib
	@mv ./../maze/bin/*.a ./$(BIN_DIR)/$(MAZE_H_LIB)

maze_test_lib_build_coverage : 
	make -C ./../maze test_lib_coverage
	@mv ./../maze/bin/*.a ./$(BIN_DIR)/$(MAZE_H_LIB)

maze_test_lib_build_address_sanitizer :
	make -C ./../maze test_lib_address_sanitizer
	@mv ./../maze/bin/*.a ./$(BIN_DIR)/$(MAZE_H_LIB)

test_build : clean test_build_echo test_lib maze_test_lib_build
	@$(MKDIR_EXISTING) $(TEST_SRC)$(BIN_DIR_TEST)
	@$(MKDIR_EXISTING) $(TEST_SRC)$(OBJ_DIR_TEST)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(TEST_SRC)*.cpp $(SUPPRESS_TEST_WARNINGS)
	@mv *.o $(TEST_SRC)$(OBJ_DIR_TEST)
	$(CXX) $(CPPFLAGS)  $(TEST_SRC)$(OBJ_DIR_TEST)*.o $(BIN_DIR)$(H_LIB) $(BIN_DIR)/$(MAZE_H_LIB) -o $(TEST_TARGET) $(CXXFLAGS) $(SUPPRESS_TEST_WARNINGS)
	@mv $(TEST_TARGET) $(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET)

unit_tests : unit_tests_echo test_build 
	./$(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET) 

coverage : clean coverage_echo test_lib_coverage maze_test_lib_build_coverage
	@$(MKDIR_EXISTING) $(TEST_SRC)$(BIN_DIR_TEST)
	@$(MKDIR_EXISTING) $(TEST_SRC)$(OBJ_DIR_TEST)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(TEST_SRC)*.cpp $(COVERAGE_FLAGS) $(SUPPRESS_TEST_WARNINGS)
	@mv *.o $(TEST_SRC)$(OBJ_DIR_TEST)
	$(CXX) $(CPPFLAGS)  $(TEST_SRC)$(OBJ_DIR_TEST)*.o $(BIN_DIR)$(H_LIB) $(BIN_DIR)/$(MAZE_H_LIB) -o $(TEST_TARGET) $(CXXFLAGS) $(COVERAGE_FLAGS) $(SUPPRESS_TEST_WARNINGS)
	@mv $(TEST_TARGET) $(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET)
	./$(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET) $(NULL_OUTPUT_STREAM)
	@gcovr --html --html-details -o coverage_report.html
	@$(OPEN_CMD) coverage_report.html
	@rm -rf  **/*.log *.gc*

address_sanitizer : clean address_sanitizer_echo test_lib_address_sanitizer maze_test_lib_build_address_sanitizer
	@$(MKDIR_EXISTING) $(TEST_SRC)$(BIN_DIR_TEST)
	@$(MKDIR_EXISTING) $(TEST_SRC)$(OBJ_DIR_TEST)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(TEST_SRC)*.cpp $(ADDRESS_SANITIZER_FLAGS) $(SUPPRESS_TEST_WARNINGS)
	@mv *.o $(TEST_SRC)$(OBJ_DIR_TEST)
	$(CXX) $(CPPFLAGS)  $(TEST_SRC)$(OBJ_DIR_TEST)*.o $(BIN_DIR)$(H_LIB) $(BIN_DIR)/$(MAZE_H_LIB) -o $(TEST_TARGET) $(CXXFLAGS) $(ADDRESS_SANITIZER_FLAGS) $(LD_ADDRESS_SANITIZER_FLAGS) $(SUPPRESS_TEST_WARNINGS)
	@mv $(TEST_TARGET) $(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET)
	./$(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET) $(NULL_OUTPUT_STREAM)

valgrind : test_build valgrind_echo
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET) $(NULL_OUTPUT_STREAM)

profile : test_build profile_echo
	valgrind --tool=massif --stacks=yes ./$(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET) $(NULL_OUTPUT_STREAM)



cppcheck : 
	cppcheck --enable=all --suppress=missingIncludeSystem ./*.c

test : unit_tests valgrind coverage address_sanitizer 

clean :
	@echo "Deleting unnecessary files..."
	rm -rf massif* *.o *.out *.a *.dSYM **/*.dSYM *.log **/*.log *.gc* *.html $(BIN_DIR)*.a $(OBJ_DIR)*.o $(TEST_SRC)$(OBJ_DIR_TEST)*.o $(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET)

profile_echo :
	@echo "┏=========================================┓"
	@echo "┃                profiling                ┃"
	@echo "┗=========================================┛"

test_build_echo : 
	@echo "┏=========================================┓"
	@echo "┃         test files compilation          ┃"
	@echo "┗=========================================┛"

valgrind_echo : 
	@echo "┏=========================================┓"
	@echo "┃             valgrind test               ┃"
	@echo "┗=========================================┛"

address_sanitizer_echo : 
	@echo "┏=========================================┓"
	@echo "┃         address sanitizer test          ┃"
	@echo "┗=========================================┛"

coverage_echo : 
	@echo "┏=========================================┓"
	@echo "┃       coverage report generation        ┃"
	@echo "┗=========================================┛"

unit_tests_echo : 
	@echo "┏=========================================┓"
	@echo "┃                  TESTS                  ┃"
	@echo "┗=========================================┛"
