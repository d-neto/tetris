#ifndef __TETRIS_CORE_H__
#define __TETRIS_CORE_H__

#include <time.h>
#include <stdlib.h>

#define BLOCK_SPEED 30
#define BLOCK_SPEED_UP 3
#define COLS 16
#define ROWS 32
#define MAX_BLOCK_GRID 5
#define NEXT_BLOCK_COUNT 3

#define BLOCK_TYPES 7

extern void Init(int **block_position, int **current_speed , int **next_block);

extern int MaxBits();
extern int CanDownRow(int row, int lower, int grid[ROWS]);
extern int LoweringBlock(int blockGrid[ROWS], int grid[ROWS], int (*kbhit)(), int (*getkey)(), int keys[4]);
extern int CanDownBlock(int blockGrid[ROWS], int grid[ROWS]);
extern int RotateBlock(int blockGrid[ROWS], int grid[ROWS]);
extern int VerifyRow(int row, int * grid);

extern void SetupBlocks();
extern void DownRow(int row, int * grid);
extern void TransposeGrid(int rotate[MAX_BLOCK_GRID]);
extern void SortNewBlock(int type, int * setIn);
extern void SpawnNewBlock(int * blockGrid);
extern void AddToGrid(int * blockGrid, int * grid);
extern void GetBlock(int type, int * setIn);


#endif