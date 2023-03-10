#ifndef _TETRIS_GAME_H_
#define _TETRIS_GAME_H_

#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>

#include "core.h"

#define LEFT_KEY 97
#define RIGHT_KEY 100
#define DOWN_KEY 115
#define ROTATION_KEY 32

int GRID[ROWS] = {0};
int BLOCK_GRID[ROWS] = {0};
int SCORE = 0;
int GAME_OVER = 0;

int kbhit(void)
{
    int ch = getch();
    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else return 0;
}


void SetUpColors();
void RenderGridInBinary();
void RenderGridInBlocks();
void RenderNextBlocks();
void ClearRow();
void DrawBorders();
void AddScore();

#endif