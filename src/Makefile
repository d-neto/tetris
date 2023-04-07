CC=gcc
CFLAGS=-Wall -Wextra
INC_PATH=./include
LIB_PATH=./lib

all: core.o

core.o: core.c core.h
	$(CC) -c -g $< -o $(INC_PATH)/core.o

clean:
	rm -rf ./*.o $(INC_PATH)/*.o