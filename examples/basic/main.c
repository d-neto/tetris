#include "./main.h"


int main(){

    Init(&blockposition, &currentspeed, &nextblock);

    hide_cursor();

    int end = 0;
    
    SetupBlocks();
    SpawnNewBlock(BLOCK_GRID);

    int keys[4] = {RIGHT_KEY, LEFT_KEY, DOWN_KEY, ROTATION_KEY};

    clear();

    DrawScore();
    DrawNextBlock();

    while(!end){

        DrawBinaryGrid();

        if(!LoweringBlock(BLOCK_GRID, GRID, &kbhit, &getch, keys)){
            Beep();
            AddToGrid(BLOCK_GRID, GRID);
            AddScore(5);
            DrawScore();
            DrawBinaryGrid();

            usleep(800 * 1000L);
            SpawnNewBlock(BLOCK_GRID);
            DrawNextBlock();
        }

        for(int i = ROWS-1; i >= 0; i--){
            DownRow(i, GRID);
            if(VerifyRow(i, GRID)){
                Beep();
                usleep(100 * 1000L);
                DrawBinaryGrid();
                AddScore(10);
                DrawScore();
            }
        }

        if(GRID[0] != 0){
            end = 1;
            continue;
        }

        usleep(10 * 1000L);
    }

    clear();
    DrawBinaryGrid();
    DrawScore();

    gotoxy(COLS-5, ROWS/2-1);
    printf(WHTHB"            "COLOR_RESET);

    gotoxy(COLS-5, ROWS/2+1);
    printf(WHTHB"            "COLOR_RESET);

    gotoxy(COLS-5, ROWS/2);
    printf(WHTHB BRED" GAME OVER! "COLOR_RESET);
    gotoxy(0, ROWS+2);

    show_cursor();
}


void DrawBinaryGrid(){
    for(int i = 0; i < ROWS; i++){
        int compare = 0b1;
        for(int j = 0; j < COLS; j++){

            gotoxy(j*2+1, i+1);

            if(GRID[i] & compare){
                printf(BYEL"1 "COLOR_RESET);
            }else if(BLOCK_GRID[i] & compare){
                printf(BGRN"1 "COLOR_RESET);
            }else{
                printf(WHT"0 "COLOR_RESET);
            }


            compare = compare << 1;
        }
    }
}

void DrawScore(){
    gotoxy(COLS*2+2, 1);
    printf(WHTHB BMAG" | Score: %d "COLOR_RESET, SCORE);
}

void DrawNextBlock(){
    gotoxy(COLS*2+2, 2);
    printf(WHTHB BBLU" +==== Next Blocks ====+ "COLOR_RESET);

    for(int i = 1; i < NEXT_BLOCK_COUNT; i++){

        int temp_grid[MAX_BLOCK_GRID] = {0};
        GetBlock(nextblock[i], temp_grid);

        for(int x = 0; x < MAX_BLOCK_GRID; x++){
            int compare = 0b1;
            for(int y = 0; y < MAX_BLOCK_GRID; y++){
                gotoxy(COLS*2+10+y*2, 3*(i)+x+1);
                if(temp_grid[x] & compare){
                    printf(BGRN"1 "COLOR_RESET);
                }else printf("  ");

                compare = compare << 1;
            }
        }
    }
}

void AddScore(int value){
    SCORE += value;
}

int Beep(){
    printf("\a");
    return 0;
}