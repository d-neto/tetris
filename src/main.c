#include "main.h"

void SetUpColors(){
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_YELLOW);
    init_pair(6, COLOR_WHITE, COLOR_BLUE);
}


void RenderGridInBinary(){
    for(int i = 0; i < ROWS; i++){
        int compare = 0b1;
        for(int j = 0; j < COLS; j++){
            if(GRID[i] & compare || BLOCK_GRID[i] & compare){
                attron(A_BOLD);

                if(BLOCK_GRID[i] & compare) attron(COLOR_PAIR(2));
                else attron(COLOR_PAIR(1));

                mvprintw(i, j*2, "1");
                attroff(A_BOLD);

                if(BLOCK_GRID[i] & compare) attroff(COLOR_PAIR(2));
                else attroff(COLOR_PAIR(1));
            }else{
                attron(COLOR_PAIR(3));
                attron(A_DIM);
                mvprintw(i, j*2, "0");
                attroff(A_DIM);
                attroff(COLOR_PAIR(3));
            }

            if(j==COLS-1) mvprintw(i, j*2+1, " = %d\n", GRID[i]);

            compare = compare << 1;
        }
    }
}
void RenderNextBlocks(){

    attron(COLOR_PAIR(6));
    attron(A_STANDOUT);   
    attron(A_BOLD);
    mvprintw(1, COLS*2+6, " Next Blocks: ");
    attroff(A_STANDOUT);    
    attroff(A_BOLD);
    attroff(COLOR_PAIR(6));

    for(int k = 1; k < NEXT_BLOCK_COUNT; k++){
        int temp_grid[5] = {0};
        GetBlock(next_block[k], temp_grid);

        for(int i = 0; i < MAX_BLOCK_GRID; i++){
            int compare = 0b1;
            for(int j = 0; j < COLS; j++){
                mvprintw(i+3+(k-1)*5, COLS*2+7+j*2, "  ");
                if(temp_grid[i] & compare){
                    attron(COLOR_PAIR(2));
                    attron(A_BOLD);
                    mvprintw(i+3+(k-1)*5, COLS*2+7+j*2, BLOCK);
                    attroff(A_BOLD);
                    attroff(COLOR_PAIR(2));
                }
                compare = compare << 1;
            }
        }

    }
}
void RenderGridInBlocks(){
    int block_offset = strlen(BLOCK);
    int empty_space_offset = strlen(EMPTY_SPACE);
    for(int i = 0; i < ROWS; i++){
        int compare = 0b1;
        for(int j = 0; j < COLS; j++){
            if(GRID[i] & compare || BLOCK_GRID[i] & compare){
                attron(A_BOLD);

                if(BLOCK_GRID[i] & compare) attron(COLOR_PAIR(2));
                else attron(COLOR_PAIR(1));

                mvprintw(i+1, j*2+block_offset, BLOCK);
                attroff(A_BOLD);

                if(BLOCK_GRID[i] & compare) attroff(COLOR_PAIR(2));
                else attroff(COLOR_PAIR(1));
            }else{
                attron(COLOR_PAIR(3));
                attron(A_DIM);
                mvprintw(i+1, j*2+empty_space_offset, EMPTY_SPACE);
                attroff(A_DIM);
                attroff(COLOR_PAIR(3));
            }
            compare = compare << 1;
        }
    }

    RenderNextBlocks();
}

void ClearRow(int row, char block[], int color){
    int block_offset = strlen(BLOCK);

    for(int j = 0; j < COLS; j++){
        attron(COLOR_PAIR(color));
        mvprintw(row+1, j*2+block_offset, "%s", block);
        attroff(A_BOLD);
        attroff(COLOR_PAIR(color));
    }
}

void DrawBorders(){
    attron(COLOR_PAIR(1));
    attron(A_BOLD);
    for(int i = 0; i <= ROWS+1; i++){
        mvprintw(i, 1, "|");
        mvprintw(i, COLS*2+2, "|");

        if(i == 0 || i == ROWS+1){
            mvprintw(i, 1, "+");
            mvprintw(i, COLS*2+2, "+");

            for(int j = 2; j <= COLS*2+1; j++)
                mvprintw(i, j, "-");
        }
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(1));

}

void AddScore(int add){
    SCORE+=add;

    attron(COLOR_PAIR(6));
    attron(A_BOLD);
    mvprintw(0, COLS-4, " SCORE: %d ", SCORE);
    attroff(COLOR_PAIR(6));
    attroff(A_BOLD);
}

int main(){

    srand(time(NULL));

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);

    SetUpColors();
    SetupBlocks();
    DrawBorders();

    SpawnNewBlock(BLOCK_GRID);
    AddScore(0);

    int keys[4] = {RIGHT_KEY, LEFT_KEY, DOWN_KEY, ROTATION_KEY};

    do{
        if(!LoweringBlock(BLOCK_GRID, GRID, &kbhit, &getch, keys)){
            beep();
            AddToGrid(BLOCK_GRID, GRID);
            RenderGridInBlocks();
            AddScore(5);
            refresh();
            usleep(1000 * 1000L);
            SpawnNewBlock(BLOCK_GRID);
        }

        for(int i = ROWS-1; i >= 0; i--){
            DownRow(i, GRID);
            if(VerifyRow(i, GRID)){
                ClearRow(i, "**", 4);
                refresh();
                beep();
                usleep(100 * 1000L);
                ClearRow(i, "--", 4);
                refresh();
                usleep(100 * 1000L);
                ClearRow(i, "__", 4);
                refresh();
                beep();
                usleep(200 * 1000L);
                AddScore(30);
            }
        }

        usleep(10 * 1000L);
        RenderGridInBlocks();
        refresh();

        if(GRID[0] != 0){
            GAME_OVER = 1;
        }
    }while(!GAME_OVER);


    attron(COLOR_PAIR(5));
    attron(A_BOLD);
    mvprintw(ROWS/2-1, 10, "              ");
    mvprintw(ROWS/2, 10, "  GAME OVER!  ");
    mvprintw(ROWS/2+1, 10, "              ");
    attroff(A_BOLD);
    attroff(COLOR_PAIR(5));
    refresh();

    while(getch() != 10);

    endwin();
    return 0;
}