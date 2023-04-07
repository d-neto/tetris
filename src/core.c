#include "core.h"

int block_position = COLS/2;
int current_speed = BLOCK_SPEED;
int speed_delay = 0;
int next_block[NEXT_BLOCK_COUNT] = {0};

void Init(int ** bp, int ** cs, int ** nxb){
    srand(time(NULL));
    *bp = &block_position;
    *cs = &current_speed;
    *nxb = next_block;
}

int MaxBits(){
    int max_bits = 0b1;
    for(int i = 0; i < COLS; i++)
        max_bits = max_bits << 1;
    max_bits -= 1;

    return max_bits;
}

void TransposeGrid(int rotate[MAX_BLOCK_GRID]){
    int newGrid[MAX_BLOCK_GRID] = {0};
    int comp = 0b1;

    int j = MAX_BLOCK_GRID - 1;
    for(j = MAX_BLOCK_GRID - 1; j >= 0; j--){
        int i = MAX_BLOCK_GRID - 1;
        for(i = MAX_BLOCK_GRID - 1; i >= 0; i--){
            if(rotate[i] & comp){
                newGrid[j] = newGrid[j] | (0b1 << i);
            }
        }
        comp = comp << 1;
    }

    for(int k = 0; k < MAX_BLOCK_GRID; k++){
        rotate[k] = newGrid[k];
    }
}
int RotateBlock(int blockGrid[ROWS], int grid[ROWS]){

    if(!CanDownBlock(blockGrid, grid)) return 0;


    for(int i = ROWS-MAX_BLOCK_GRID-1; i < ROWS; i++)
        if(blockGrid[i] != 0) return 0;

    int first_row = -1;
    int last_row = 0;

    int rotate[MAX_BLOCK_GRID] = {0};

    for(int i = 0; i < ROWS; i++){
        if(blockGrid[i] != 0 && first_row == -1){
            first_row = i;
        }

        blockGrid[i] = blockGrid[i] >> block_position;
    }

    last_row = first_row+MAX_BLOCK_GRID;

    int index = 0;
    for(int i = first_row; i <= last_row; i++){
        rotate[index] = blockGrid[i];
        index++;
    }

    for(int i = 0; i < ROWS; i++)
        blockGrid[i] = 0;

    int grid_first_row = first_row;

    TransposeGrid(rotate);

    while(rotate[0] == 0)
        for(int i = 0; i < MAX_BLOCK_GRID; i++){
            if(i != MAX_BLOCK_GRID-1)
                rotate[i] = rotate[i+1];
            else rotate[i] = 0;
        }

    int border_right = (0b1 << COLS) - 1;
    for(int k = 0; k < MAX_BLOCK_GRID; k++){
        if(((rotate[k] << block_position) > border_right)){
            block_position--;
            k--;
            continue;
        }
    }
    for(int k = 0; k < MAX_BLOCK_GRID; k++)
        blockGrid[grid_first_row+k] = rotate[k] << block_position;

    return 1;
}

int CanDownRow(int row, int lower, int grid[ROWS]){
    if((row+lower) >= ROWS) return 0;
    return !(grid[row] & grid[row+lower]);
}
void DownRow(int row, int * grid){
    int lower = 0;
    for(int i = row; i < ROWS; i++){
        if(CanDownRow(row, lower+1, grid)){
            lower++;
        }
    }
    if(lower > 0){
        grid[row+lower] = grid[row] | grid[row+lower];
        grid[row] = 0;
    }
}
int VerifyRow(int row, int * grid){
    int max_bits = MaxBits();
    if(
        (grid[row] >= max_bits) && 
        !CanDownRow(row, 2, grid)
    ){
        grid[row] = 0;
        return 1;
    }
    return 0;
}

int CanDownBlock(int blockGrid[ROWS], int grid[ROWS]){
    if(blockGrid[ROWS-1] != 0) return 0;

    for(int i = 0; i < ROWS; i++)
        if(i+1 < ROWS && grid[i+1] & blockGrid[i]){
            return 0;
        }

    return 1;
}
int LoweringBlock(int blockGrid[ROWS], int grid[ROWS], int (*kbhit)(), int (*getkey)(), int keys[4]){
    if(!CanDownBlock(blockGrid, grid)){
        current_speed = BLOCK_SPEED;
        return 0;
    }
    speed_delay++;

    if(speed_delay >= current_speed){
        for(int i = ROWS-1; i > 0; i--){
            if(i == ROWS-1 && blockGrid[i] != 0) break;
            blockGrid[i] = blockGrid[i-1];
            if(i == 1) blockGrid[0] = 0;
        }
        speed_delay = 0;
    }

    if((*kbhit)()){
        char ch = (*getkey)();
        char canMoveLeft = 1;
        char canMoveRight = 1;

        int max_right = 0b1;
        for(int i = 0; i < COLS-1; i++)
            max_right = max_right << 1;

        for(int i = ROWS-1; i >= 0; i--){
            if(blockGrid[i] != 0 && (blockGrid[i] >> 1) & grid[i]) canMoveLeft = 0;
            if(blockGrid[i] != 0 && (blockGrid[i] << 1) & grid[i]) canMoveRight = 0;

            if(blockGrid[i] & 0b1)
                canMoveLeft = 0;
            if(blockGrid[i] & max_right)
                canMoveRight = 0;
        }


        if(ch == keys[0] && canMoveRight){
            block_position++;
            for(int i = ROWS-1; i >= 0; i--){
                blockGrid[i] = blockGrid[i] << 1;
            }
        }else if(ch == keys[1] && canMoveLeft){
            block_position--;
            for(int i = ROWS-1; i >= 0; i--){
                blockGrid[i] = blockGrid[i] >> 1;
            }
        }else if(ch == keys[2]){
                current_speed = BLOCK_SPEED_UP;
        }else if(ch == keys[3]){
                RotateBlock(blockGrid, grid);
        }
    }


    return 1;
}
void AddToGrid(int * blockGrid, int * grid){
    for(int i = 0; i < ROWS; i++){
        grid[i] = grid[i] | blockGrid[i];
        blockGrid[i] = 0;
    }
}

void GetBlock(int type, int * setIn){

    if(
        COLS < 4 && 
        (type == 0 || type == 2 || type == 3 || type == 5 || type == 6)
    ) type = 1;

    switch(type){
        case 0:
            setIn[0] = 0b001111;
            break;
        case 1:
            setIn[0] = 0b000011;
            setIn[1] = 0b000011;
            break;
        case 2:
            setIn[0] = 0b000011;
            setIn[1] = 0b000110;
            break;
        case 3: 
            setIn[0] = 0b000001;
            setIn[1] = 0b000111;
            break;
        case 4: 
            setIn[0] = 0b000010;
            setIn[1] = 0b000111;
            break;
        case 5: 
            setIn[0] = 0b000100;
            setIn[1] = 0b000111;
            break;
        case 6: 
            setIn[0] = 0b000110;
            setIn[1] = 0b000011;
            break;
    }
}
void SortNewBlock(int type, int * setIn){
    GetBlock(type, setIn);
    setIn[0] = setIn[0] << block_position;
    setIn[1] = setIn[1] << block_position;
}

void SpawnNewBlock(int * blockGrid){

    for(int i = 0; i < NEXT_BLOCK_COUNT; i++){
        if(i != NEXT_BLOCK_COUNT-1)
            next_block[i] = next_block[i+1];
    }
    next_block[NEXT_BLOCK_COUNT-1] = rand() % BLOCK_TYPES;

    block_position = COLS/2;
    if(COLS < 5) block_position = 0;

    SortNewBlock(next_block[0], blockGrid);
}
void SetupBlocks(){
    for(int i = 0; i < 3; i++){
        next_block[i] = rand() % 7;
    }
}