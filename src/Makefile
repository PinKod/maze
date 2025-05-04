install_dir ?= /usr/local/bin

all : cli test install clean

cli :
	make -C ./cli cli

install: cli
	install -d $(install_dir)
	install cli/bin/maze_cli $(install_dir)

uninstall:
	rm -f $(install_dir)/bin/cli

clean:
	make -C ./io clean
	make -C ./maze clean
	make -C ./cave clean
	rm -rf *.o *.a *.dSYM **/*.dSYM *.log **/*.log *.gc*
	
test :
	@echo "┏=========================================┓"
	@echo "┃                 MAZE                    ┃"
	@echo "┗=========================================┛"
	make -C ./maze test
	@echo "┏=========================================┓"
	@echo "┃                 IO TESTS                ┃"
	@echo "┗=========================================┛"
	make -C ./io test

style_check:
	@echo "┏=========================================┓"
	@echo "┃  Checking your code for Adecvatnost     ┃"
	@echo "┗=========================================┛"
	@cp ./linters/.clang-format .
	@find . -name "*.c" -o -name "*.h" | xargs clang-format -n
	@rm ./.clang-format

format_style:
	@echo "┏=========================================┓"
	@echo "┃  Formatting your code for Adecvatnost   ┃"
	@echo "┗=========================================┛"
	@cp ./linters/.clang-format .
	@find . -name "*.c" -o -name "*.h" | xargs clang-format -i
	@rm .clang-format

style_check_cheloveka_cotorogo_po_golove_bum_bum_bum:
	@echo "┏=========================================┓"
	@echo "┃  Checking your code for Google Style    ┃"
	@echo "┗=========================================┛"
	@find . -name "*.c" -o -name "*.h" | xargs clang-format -n -style="{BasedOnStyle: Google}"

format_style_cheloveka_cotorogo_po_golove_bum_bum_bum:
	@echo "┏=========================================┓"
	@echo "┃  Formatting your code for Google Style  ┃"
	@echo "┗=========================================┛"
	@find . -name "*.c" -o -name "*.h" | xargs clang-format -i -style="{BasedOnStyle: Google}"

full_clean: clean
	rm -rf *.html

.PHONY: all clean test style_check format_style style_check_cheloveka_cotorogo_po_golove_bum_bum_bum format_style_cheloveka_cotorogo_po_golove_bum_bum_bum full_clean uninstall install cli