CC=gcc
CFLAGS=-Wall -Wextra -g

all: tetris

tetris: main.o
	$(CC) $(CFLAGS) main.h include/main.o core.h -o bin/$@ -lncurses
	@echo "Build process finished. Run with ./bin/tetris"

main.o: 
	$(CC) $(CFLAGS) -c main.c -o include/$@ 

clean: 
	rm -rf ./*.o tetris ./bin/* ./include/*.o