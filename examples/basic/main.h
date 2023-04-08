#include <stdio.h>
#include <unistd.h>

#include "input.h"
#include "../../src/core.h"

#define show_cursor() printf("\e[?25h")
#define hide_cursor() printf("\e[?25l")
#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

/*
    Text Colors
*/
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BWHT "\e[1;37m"
#define BMAG "\e[1;35m"
#define WHT "\e[0;37m"
#define COLOR_RESET "\e[0m"

/*
    Background Colors
*/
#define BLKHB "\e[0;100m"
#define REDHB "\e[0;101m"
#define GRNHB "\e[0;102m"
#define YELHB "\e[0;103m"
#define BLUHB "\e[0;104m"
#define MAGHB "\e[0;105m"
#define CYNHB "\e[0;106m"
#define WHTHB "\e[0;107m"

/*
    Keys
*/
#define LEFT_KEY 97
#define RIGHT_KEY 100
#define DOWN_KEY 115
#define ROTATION_KEY 32

int *blockposition = NULL;
int *nextblock = NULL;
int *currentspeed = NULL;

int GRID[ROWS] = {0};
int BLOCK_GRID[ROWS] = {0};
int SCORE = 0;

void DrawBinaryGrid();
void DrawScore();
void DrawNextBlock();
void AddScore(int value);
int Beep();