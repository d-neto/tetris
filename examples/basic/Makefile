CC=gcc
CFLAGS= -Wall -Wextra
INC_PATH=./include
CORE_PATH=../../src/include/core.o
LOCAL_BIN=./bin/tetris

all: check tetris

check:
	@[ -f $(CORE_PATH) ] || (echo "Core file not found. Make it on source directory!"; exit 1;)


tetris: main.o input.o
	$(CC) $(CFLAGS) -o $(LOCAL_BIN) $(INC_PATH)/main.o $(INC_PATH)/input.o $(CORE_PATH)
	@echo "\n\033[1;92mBuild process finished. Run with \033[4;34m./bin/tetris\033[0m\n"

input.o: input.c input.h
	$(CC) -g -c $< -o $(INC_PATH)/$@

main.o: main.c main.h
	$(CC) -g -c $< -o $(INC_PATH)/$@

clean: 
	rm -rf $(LOCAL_BIN) $(INC_PATH)/*.o