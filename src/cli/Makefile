CC = gcc
FLAGS = -Wall -Wextra -std=c11 -g -O3 
LINUX_FLAGS = -lrt -lpthread -lm
LINKING_FLAGS = -lrt -lpthread -lm
OBJ_DIR = ./bin/obj/
BIN_DIR = ./bin/
MKDIR_EXISTING = mkdir -p
CLI = maze_cli


cli : make_bin_dir libs *.c
	@$(MKDIR_EXISTING) $(OBJ_DIR)
	$(CC) $(FLAGS) $(LINUX_FLAGS) -c *.c 
	@mv *.o $(OBJ_DIR)
	$(CC) $(OBJ_DIR)*.o $(BIN_DIR)*.a $(LINKING_FLAGS) -o $(BIN_DIR)$(CLI) 

libs : make_bin_dir
	make -C ./../maze clean
	make -C ./../maze test_lib
	cp ./../maze/$(BIN_DIR)*.a $(BIN_DIR)
	make -C ./../cave clean
	make -C ./../cave test_lib
	cp ./../cave/$(BIN_DIR)*.a $(BIN_DIR)
	make -C ./../io clean 
	make -C ./../io test_lib
	cp ./../io/$(BIN_DIR)*.a $(BIN_DIR)
	

make_bin_dir : 
	$(MKDIR_EXISTING) $(BIN_DIR)

clean :
	@echo "Deleting unnecessary files..."
	@rm -rf massif* *.o *.out *.a *.dSYM **/*.dSYM *.log **/*.log *.gc* *.html $(BIN_DIR)/*.a $(OBJ_DIR)*.o $(TEST_SRC)$(OBJ_DIR_TEST)*.o $(TEST_SRC)$(BIN_DIR_TEST)$(TEST_TARGET)
