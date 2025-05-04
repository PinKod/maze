CC = gcc
FLAGS = -Wall -Wextra -std=c11 -g -O3 
LINUX_FLAGS = -lsubunit -lrt -lpthread -lm -lncurses
COVERAGE_FLAGS = --coverage -fprofile-arcs -ftest-coverage
ADDRESS_SANITIZER_FLAGS = -fsanitize=address -fsanitize=leak
LD_ADDRESS_SANITIZER_FLAGS = -static-libasan
OBJ_DIR = ./bin/obj/
BIN_DIR = ./bin/
SERVER_TARGET = web_server
MKDIR_EXISTING = mkdir -p

OS := $(shell uname -s)
ifeq ($(OS),Linux)
	OPEN_CMD = xdg-open
endif

ifeq ($(OS),Darwin)
	OPEN_CMD = open
endif

web_server : clean maze_test_lib_build io_test_lib_build
	@$(MKDIR_EXISTING) $(BIN_DIR)
	@$(MKDIR_EXISTING) $(OBJ_DIR)
	$(CC) $(FLAGS) *.c -o $(SERVER_TARGET)  ./$(BIN_DIR)/*.a $(LINUX_FLAGS)
	@mv $(SERVER_TARGET) $(BIN_DIR)

maze_test_lib_build :
	@$(MKDIR_EXISTING) $(BIN_DIR)
	make -C ./../maze test_lib
	@mv ./../maze/bin/*.a ./$(BIN_DIR)

io_test_lib_build :
	@$(MKDIR_EXISTING) $(BIN_DIR)
	make -C ./../io test_lib
	mv ./../io/bin/*.a ./$(BIN_DIR)


clean :
	@echo "Deleting unnecessary files..."
	@rm -rf massif* *.o *.out *.a *.dSYM **/*.dSYM *.log **/*.log *.gc* *.html $(BIN_DIR)/$(H_LIB) $(OBJ_DIR)*.o $(TEST_SRC)$(OBJ_DIR_TEST)*.o $(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET)
