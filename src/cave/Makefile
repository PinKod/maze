CC = gcc
FLAGS = -Wall -Wextra -std=c11 -g
LINUX_FLAGS = -lsubunit -lrt -lpthread -lm -static
COVERAGE_FLAGS = --coverage -fprofile-arcs -ftest-coverage
ADDRESS_SANITIZER_FLAGS = -fsanitize=address -fsanitize=leak
LD_ADDRESS_SANITIZER_FLAGS = -static-libasan
OBJ_DIR = ./bin/obj/
BIN_DIR = ./bin/
H_LIB = io_cave_h_lib.a                   ##target name change for another dir
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
	$(CC) $(FLAGS) $(LINUX_FLAGS) -c ./cave/*.c ./io/*.c
	@mv *.o $(OBJ_DIR)
	ar rc $(H_LIB) $(OBJ_DIR)*.o
	ranlib $(H_LIB)
	@mv $(H_LIB) $(BIN_DIR)

test_lib_coverage : clean
	@$(MKDIR_EXISTING) $(BIN_DIR)
	@$(MKDIR_EXISTING) $(OBJ_DIR)
	$(CC) $(FLAGS) $(LINUX_FLAGS) -c ./cave/*.c ./io/*.c $(COVERAGE_FLAGS)
	@mv *.o $(OBJ_DIR)
	ar rc $(H_LIB) $(OBJ_DIR)*.o
	ranlib $(H_LIB)
	@mv $(H_LIB) $(BIN_DIR)

test_lib_address_sanitizer : clean
	@$(MKDIR_EXISTING) $(BIN_DIR)
	@$(MKDIR_EXISTING) $(OBJ_DIR)
	$(CC) $(FLAGS) $(LINUX_FLAGS) -c ./cave/*.c ./io/*.c $(ADDRESS_SANITIZER_FLAGS)
	@mv *.o $(OBJ_DIR)
	ar rc $(H_LIB) $(OBJ_DIR)*.o
	ranlib $(H_LIB)
	@mv $(H_LIB) $(BIN_DIR)








clean :
	@echo "Deleting unnecessary files..."
	@rm -rf massif* *.o *.out *.a *.dSYM **/*.dSYM *.log **/*.log *.gc* *.html $(BIN_DIR)/$(H_LIB) $(OBJ_DIR)*.o $(TEST_SRC)$(OBJ_DIR_TEST)*.o $(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET)
