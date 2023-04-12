#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <unistd.h>

#include "../../src/core.h"

#define ICON_PATH "./assets/icons/icon.png"

#define CELL_SIZE 16
#define SCALE 2

#define GRID_WIDTH COLS*CELL_SIZE
#define GRID_HEIGHT ROWS*CELL_SIZE

#define WIDTH GRID_WIDTH+16*10
#define HEIGHT GRID_HEIGHT

#define LEFT_KEY 97
#define RIGHT_KEY 100
#define DOWN_KEY 115
#define ROTATION_KEY 32

/* SDL Renderer */
SDL_Window * window;
SDL_Renderer * renderer;

/* Textures */
#define TXT_BLOCK_PATH "./assets/textures/block.png"
#define TXT_GRID_BG_PATH "./assets/textures/grid_bg.png"
#define TXT_BG_PATH "./assets/textures/bg.png"

SDL_Texture * block_txt;
SDL_Texture * bg_txt;
SDL_Texture * bg_grid_txt;

/* Text */
#define FONT_NAME "./assets/fonts/8-bit.ttf"
#define FONT_SIZE 32

SDL_Texture * score_txt;
SDL_Rect score_rect;

SDL_Texture * scoreValue_txt;
SDL_Rect scoreValue_rect;

SDL_Texture * nextBlock_txt;
SDL_Rect nextBlock_rect;

SDL_Surface * textSurface;
TTF_Font * textFont;
SDL_Color textColor;
SDL_Color textBgColor;

/* Audio */
#define SCORE_AUDIO_PATH "./assets/sfx/score.wav"
#define BLOCK_AUDIO_PATH "./assets/sfx/block.wav"

SDL_AudioSpec score_wavSpec;
Uint32 score_wavLength;
Uint8 *score_wavBuffer;

SDL_AudioSpec block_wavSpec;
Uint32 block_wavLength;
Uint8 *block_wavBuffer;

SDL_AudioDeviceID deviceId_score;
SDL_AudioDeviceID deviceId_block;

/* Tetris Props. */
int grid[HEIGHT/CELL_SIZE];
int block_grid[HEIGHT/CELL_SIZE];
int score = 0;
int game_over = 0;

int pressed_key = 0;
int key_code = 0;

int keys[4] = {RIGHT_KEY, LEFT_KEY, DOWN_KEY, ROTATION_KEY};

int *blockposition = NULL;
int *currentspeed = NULL;
int *nextblock = NULL;

int Initialize();
int kbhit();
int getkey();
int GetLengthInt(int integer);
char * IntegerToString(int integer);
void SetUpAll();
void Update();
void Kill();
void DrawGrid();
void DrawBlocks();
void DrawNextBlocks();
void DrawGrid();
void CreateText(char * textPrint, SDL_Texture ** txt, SDL_Rect * rect, int x, int y, int color[]);
void DeleteRow(int row);
void LoadTextures();
void LoadSFX();
void InitFonts();
void SetIcon();