CC = gcc
CFLAGS = -g -Wall
SDL_INCLUDES = -lSDL2_image -lSDL2_ttf `sdl2-config --cflags --libs`
INC_PATH=include
CORE_PATH=../../src/include/core.o
LOCAL_BIN=./bin/tetris


all: check tetris

check:
	@[ -f $(CORE_PATH) ] || (echo "Core file not found. Make it on source directory!"; exit 1;)

tetris: main.o
	$(CC) $(CFLAGS) -o $(LOCAL_BIN) $(INC_PATH)/main.o $(CORE_PATH) $(SDL_INCLUDES)
	@echo "\n\033[1;92mBuild process finished. Run with \033[4;34m./bin/tetris\033[0m\n"

main.o: main.c main.h
	$(CC) -g -c $< -o $(INC_PATH)/$@ $(SDL_INCLUDES)

clean:
	rm -rf ./bin/tetris ./include/*.o