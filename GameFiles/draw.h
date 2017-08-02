#ifndef ARM11_31_DRAW_H
#define ARM11_31_DRAW_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "gameUtils.h"
#include "game.h"

#define SCREEN_WIDTH   600
#define SCREEN_HEIGHT  600
#define SQUARE_SIZE 50
#define SCREEN_SIDEBAR_WIDTH 158

//
//typedef struct sdl_Objects {
//
//  SDL_Window *window;
//  SDL_Renderer *renderer;
//  SDL_Surface *surface;
//  SDL_Event event;
//
//} sdl_Objects_t;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_Event event;


SDL_Texture *blank_texture;
SDL_Texture *floor_texture;
SDL_Texture *cracked_texture;
SDL_Texture *player_texture;
SDL_Texture *endpos_texture;
SDL_Texture *queen_texture;
SDL_Texture *tony_texture;
SDL_Texture *side_helper_texture;

SDL_Texture *splash_screen;
SDL_Texture *credit_screen;
SDL_Texture *end_screen;
SDL_Texture *intro_screen;
SDL_Texture *queen_texture_imperial;

SDL_Texture *main_menu_background;
SDL_Texture *end_level_menu_background;
SDL_Texture *options_menu_background;
SDL_Texture *level_menu_background;

TTF_Font *infinityFont;


void renderRect(SDL_Renderer *renderer, int x, int y, int width, int height,
                int r, int g, int b);

void initialiseSDL();

void renderStage(squareState **map, playerPosition endPos, int rows, int cols);

void clearUpSDL();

void renderImage(int x, int y, int width, int height,
                 SDL_Texture *image_texture);

int renderUpdateStage(playerPosition newPosition, int firstTime, int rows, int
cols, SDL_Texture *curr_texture);

void renderRectOutline(SDL_Renderer *renderer, int x, int y, int width, int
height, int r, int g, int b);

void renderSplashScreen(uint32_t time);

void renderSideMenu();

void renderSideMenuTimer();

void renderMenu(SDL_Texture *menu);

void renderEndScreen();

void renderTimer(char displayText[]);

void renderLevelCounter(gameState_t *currGame);

void renderSideHelper(int i, int displacement);

void draw_solution(gameState_t *currGame);

void clearScreen();

void renderIntroScreen();

void renderLeaderboard(gameState_t *currGame);

void renderCredits();

void renderEndSideMenu();

char *renderUpdateTime(gameState_t *currGame);

char *convertTimetoStr(uint32_t time);

void loadTexture(char *fileName, SDL_Texture **loadTexture);

#endif //ARM11_31_DRAW_H
