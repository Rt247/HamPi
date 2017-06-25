#ifndef GAME_GAME_H
#define GAME_GAME_H

#include "gameUtils.h"
#include "SDL2/SDL.h"

#define SPLASH_SCREEN_TIME 2000
#define LEVEL_CAP 5
#define NUM_CHOICES 4
#define X_DISP 200
#define Y_DISP 180
#define X_WIDTH_SIZE 180
#define Y_WIDTH_SIZE 74
#define SELECTION_DISPLACEMENT 3
#define X_DISP_FIRST 160
#define Y_DISP_FIRST 190
#define X_DISP_SECOND 304
#define Y_DISP_SECOND 190
#define X_DISP_THIRD 304
#define Y_DISP_THIRD 300
#define Y_WIDTH_SIZE_OPT 55
#define X_WIDTH_SIZE_OPT 144

typedef enum gameMode{
    RANDOM,
    SAVED
}gameMode_t;

typedef enum difficultyMode{
    EASY = 4,
    MEDIUM = 7,
    HARD = 10
}difficultyMode_t;

typedef struct gameState{
    int nMoves;
    int rows;
    int cols;
    squareState **level;
    squareState **levelMap;
    playerPosition playerPos;
    playerPosition initPosLevel;
    int count;
    int gameIsRunning;
    gameMode_t currGameMode;
    difficultyMode_t difficultyGameMode;
    uint32_t timer_count;
    uint32_t start_time;
    uint32_t level_menu_timer;
    playerPosition *solArray[144];
    leaderBoard *head;
    playerPosition endPosLevel;
    int levelCount;

} gameState_t;



typedef enum menuOptions{
    START,
    OPTIONS,
    CREDITS,
    EXIT,
    NOTHING,
    RESUME,
    MAIN_MENU,
    SAVE_LEVEL,
    NEXT_LEVEL,
    GAME_MODE,
    DIFFICULTY,
    GIVE_UP
} menuOptions_t;

void loadRandomLevel(gameState_t *currGame);
void loadSavedLevel(gameState_t *currGame);
void mainMenuLogic(gameState_t *currGame);
void levelMenuLogic(gameState_t *currGame);
void creditMenuLogic(gameState_t *currGame);
void endlevelMenuLogic(gameState_t *currGame);
void initialiseGameState(gameState_t *currGame);
void initialiseLeaderboard(gameState_t *currGame);
menuOptions_t getMenuInput(menuOptions_t choices[], int num_options,
                               SDL_Texture *menu_background, int isEndLevel,
                               gameState_t *currGame);
menuOptions_t getOptMenuInput(gameState_t *currGame);
void resetLevel(gameState_t *currGame);
void findPlayerPos(gameState_t *currGame);
void saveLevel(gameState_t *currGame);
void incrementLevel(gameState_t *currGame);
void freeCurrGame(gameState_t *currGame);


#endif //GAME_GAME_H
