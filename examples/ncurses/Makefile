CC=gcc
CFLAGS=-Wall -Wextra
CORE_PATH=../../src/include/core.o
LOCAL_BIN=./bin/tetris

all: check tetris

check:
	@[ -f $(CORE_PATH) ] || (echo "Core file not found. Make it on source directory!"; exit 1;)

tetris: main.o
	$(CC) $(CFLAGS) -o $(LOCAL_BIN) include/main.o $(CORE_PATH) -lncurses
	@echo "\n\033[1;92mBuild process finished. Run with \033[4;34m./bin/tetris\033[0m\n"

main.o: main.c main.h
	$(CC) -g -c $< -o include/$@

clean: 
	rm -rf ./*.o tetris ./bin/* ./include/*.o