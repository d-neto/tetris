
#include "main.h"

int score_color[3] = {255, 255, 180};
int next_color[3] = {255, 255, 100};

int main(int argc, char *argv[])
{

    if(!Initialize()) return -1;

    SetUpAll();
    SetIcon();
    LoadTextures();
    LoadSFX();
    InitFonts();

    CreateText("Score", &score_txt, &score_rect, 0, 10, score_color);
    CreateText("0", &scoreValue_txt, &scoreValue_rect, 72, 40, score_color);
    CreateText("Next", &nextBlock_txt, &nextBlock_rect, 40, 90, next_color);

    int close = 0;
    while (!close){

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT: close = 1; break;
                case SDL_KEYDOWN:
                    pressed_key = 1;
                    key_code = event.key.keysym.sym;
                    break;
            }
        }

        SDL_RenderClear(renderer);
        Update();
        SDL_RenderPresent(renderer);

        SDL_Delay(1000 / 60);
        if(pressed_key) pressed_key = 0;
    }

    Kill();
 
    return 0;
}



int Initialize(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 0;
    }
    if(TTF_Init() != 0){
        printf("error initializing TTF: %s\n", TTF_GetError());
        return 0;
    }

    deviceId_score = SDL_OpenAudioDevice(NULL, 0, &score_wavSpec, NULL, 0);
    deviceId_block = SDL_OpenAudioDevice(NULL, 0, &block_wavSpec, NULL, 0);

    Init(&blockposition, &currentspeed, &nextblock);
    return 1;
}

void SetIcon(){
    SDL_Surface * icon = IMG_Load(ICON_PATH);
    SDL_SetWindowIcon(window, icon);
}
void SetUpAll(){
    window = SDL_CreateWindow("TETRIS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SetupBlocks();
    SpawnNewBlock(block_grid);
}
void LoadTextures(){
    block_txt = IMG_LoadTexture(renderer, TXT_BLOCK_PATH);
    bg_grid_txt = IMG_LoadTexture(renderer, TXT_GRID_BG_PATH);
    bg_txt = IMG_LoadTexture(renderer, TXT_BG_PATH);
}
void LoadSFX(){
    SDL_LoadWAV(SCORE_AUDIO_PATH, &score_wavSpec, &score_wavBuffer, &score_wavLength);
    SDL_LoadWAV(BLOCK_AUDIO_PATH, &block_wavSpec, &block_wavBuffer, &block_wavLength);
}

void Kill(){

    SDL_CloseAudioDevice(deviceId_score);
    SDL_CloseAudioDevice(deviceId_block);
    
    SDL_FreeWAV(score_wavBuffer);
    SDL_FreeWAV(block_wavBuffer);

    SDL_DestroyTexture(bg_txt); 
    SDL_DestroyTexture(bg_grid_txt); 
    SDL_DestroyTexture(block_txt);
    SDL_DestroyTexture(score_txt);
    SDL_DestroyTexture(scoreValue_txt);

    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Update(){

    DrawGrid();
    DrawBlocks();
    DrawNextBlocks();
    if(!game_over){

        for(int i = 0; i < HEIGHT/CELL_SIZE; i++){
            DownRow(i, grid);
            if(VerifyRow(i, grid)){
                score+=15;
                CreateText(IntegerToString(score), &scoreValue_txt, &scoreValue_rect, 72 - GetLengthInt(score) * 4, 40, score_color);

                int success = SDL_QueueAudio(deviceId_score, score_wavBuffer, score_wavLength);
                SDL_PauseAudioDevice(deviceId_score, 0);

                DeleteRow(i);
            }
        }

        if(!LoweringBlock(block_grid, grid, &kbhit, &getkey, keys)){

            int success = SDL_QueueAudio(deviceId_block, block_wavBuffer, block_wavLength);
            SDL_PauseAudioDevice(deviceId_block, 0);

            score += 5;
            AddToGrid(block_grid, grid);
            SpawnNewBlock(block_grid);
            CreateText(IntegerToString(score), &scoreValue_txt, &scoreValue_rect, 72 - GetLengthInt(score) * 4, 40, score_color);
        }

    }

    if(grid[0] != 0) game_over = 1;

    SDL_RenderCopy(renderer, score_txt, NULL, &score_rect);
    SDL_RenderCopy(renderer, scoreValue_txt, NULL, &scoreValue_rect);
    SDL_RenderCopy(renderer, nextBlock_txt, NULL, &nextBlock_rect);
}

void InitFonts(){
    textFont = TTF_OpenFont(FONT_NAME, FONT_SIZE);
}
void CreateText(char * textPrint, SDL_Texture ** txt, SDL_Rect * rect, int x, int y, int color[]){
    // textColor.r = 255;
    // textColor.g = 255;
    // textColor.b = 180;
    textColor.r = color[0];
    textColor.g = color[1];
    textColor.b = color[2];

    textBgColor.r = 0;
    textBgColor.g = 0;
    textBgColor.b = 0;

    textSurface = TTF_RenderText_Solid(textFont, textPrint, textColor); 
    *txt = SDL_CreateTextureFromSurface(renderer, textSurface);
    rect->x = x == 0 ? GRID_WIDTH + textSurface->w/2 : GRID_WIDTH + x;
    rect->y = y;
    rect->w = textSurface->w;
    rect->h = textSurface->h;

    SDL_FreeSurface(textSurface);
}

void PaintRow(int row, int paint){
    int w, h;
    SDL_QueryTexture(block_txt, NULL, NULL, &w, &h);
    SDL_Rect txtdraw;
    txtdraw.x = 0;
    txtdraw.y = 0;
    txtdraw.w = w*SCALE;
    txtdraw.h = h*SCALE;

    txtdraw.y = row*h*SCALE;

    SDL_RenderClear(renderer);
    DrawGrid();
    DrawBlocks();
    DrawNextBlocks();
    SDL_RenderCopy(renderer, score_txt, NULL, &score_rect);
    SDL_RenderCopy(renderer, scoreValue_txt, NULL, &scoreValue_rect);
    SDL_RenderCopy(renderer, nextBlock_txt, NULL, &nextBlock_rect);

    for(int j = 0; j < GRID_WIDTH/CELL_SIZE; j++){
        txtdraw.x = j*w*SCALE;
        if(!paint) SDL_SetTextureColorMod(block_txt, 255, 130, 0);
        else SDL_SetTextureColorMod(block_txt, 255, 60, 60);
        SDL_RenderCopy(renderer, block_txt, NULL, &txtdraw);
    }

    SDL_RenderPresent(renderer);
}

void DeleteRow(int row){
    SDL_Delay(100);
    PaintRow(row, 0);
    SDL_Delay(100);
    PaintRow(row, 1);
    SDL_Delay(100);
    PaintRow(row, 0);
    SDL_Delay(100);
    PaintRow(row, 0);
    SDL_Delay(100);
    PaintRow(row, 1);
    SDL_Delay(100);
    PaintRow(row, 0);
    grid[row] = 0;
}

int kbhit(){
    return pressed_key;
}
int getkey(){
    return key_code;
}

void DrawGrid(){
    int w, h;
    SDL_QueryTexture(bg_grid_txt, NULL, NULL, &w, &h);

    SDL_Rect txt;
    txt.x = 0;
    txt.y = 0;
    txt.w = w*SCALE;
    txt.h = h*SCALE;

    SDL_SetTextureColorMod(bg_grid_txt, 100, 100, 130);
    for(int i = 0; i < GRID_WIDTH; i+=w*SCALE){
        txt.x = i;
        for(int j = 0; j < GRID_HEIGHT; j+=w*SCALE){
            txt.y = j;
            SDL_RenderCopy(renderer, bg_grid_txt, NULL, &txt);
        }
    }

    SDL_QueryTexture(bg_txt, NULL, NULL, &w, &h);
    txt.w = w*SCALE;
    txt.h = h*SCALE;

    SDL_SetTextureColorMod(bg_txt, 150, 150, 170);
    for(int i = GRID_WIDTH; i < WIDTH; i+=w*SCALE){
        txt.x = i;
        for(int j = 0; j < GRID_HEIGHT; j+=w*SCALE){
            txt.y = j;
            SDL_RenderCopyEx(renderer, bg_txt, NULL, &txt, 0, NULL, 0);
        }
    }
}

void DrawBlocks(){
    int w, h;
    SDL_QueryTexture(block_txt, NULL, NULL, &w, &h);
    SDL_Rect txtdraw;
    txtdraw.x = 0;
    txtdraw.y = 0;
    txtdraw.w = w*SCALE;
    txtdraw.h = h*SCALE;
    for(int i = 0; i < GRID_HEIGHT/CELL_SIZE; i++){
        txtdraw.y = i*h*SCALE;
        int compare = 0b1;
        for(int j = 0; j < GRID_WIDTH/CELL_SIZE; j++){
            txtdraw.x = j*w*SCALE;
            if(grid[i] & compare){
                if(!game_over)
                    SDL_SetTextureColorMod(block_txt, 255, 130, 0);
                else
                    SDL_SetTextureColorMod(block_txt, 105, 30, 30);
                SDL_RenderCopy(renderer, block_txt, NULL, &txtdraw);
            }
            if(block_grid[i] & compare){
                if(!game_over)
                    SDL_SetTextureColorMod(block_txt, 0, 255, 130);
                else
                    SDL_SetTextureColorMod(block_txt, 0, 120, 100);
                SDL_RenderCopy(renderer, block_txt, NULL, &txtdraw);
            }
            compare = compare << 1;
        }
    }
}

void DrawNextBlocks(){
    int w, h;
    SDL_QueryTexture(block_txt, NULL, NULL, &w, &h);
    SDL_Rect txtdraw;
    txtdraw.x = 0;
    txtdraw.y = 0;
    txtdraw.w = w*SCALE;
    txtdraw.h = h*SCALE;




    for(int k = 1; k < NEXT_BLOCK_COUNT; k++){

        int temp_grid[MAX_BLOCK_GRID] = {0};
        GetBlock(nextblock[k], temp_grid);

        for(int i = 0; i < MAX_BLOCK_GRID; i++){
            txtdraw.y = (110*k)+(i+1)*h*SCALE;
            int compare = 0b1;
            for(int j = 0; j < MAX_BLOCK_GRID; j++){
                txtdraw.x = GRID_WIDTH+ (MAX_BLOCK_GRID/2+1)*w*SCALE + j*w*SCALE;
                if(temp_grid[i] & (1 << j)){
                    SDL_SetTextureColorMod(block_txt, 0, 255, 130);
                    SDL_RenderCopy(renderer, block_txt, NULL, &txtdraw);
                }
            }
        }
    }
}

int GetLengthInt(int integer){
    int length = 0;
    while(integer >= 10){
        length++;
        integer /= 10;
    }
    return length;
}
char * IntegerToString(int integer){
    char * string = (char *) malloc(sizeof(int));
    sprintf(string, "%d", integer);
    return string;
}