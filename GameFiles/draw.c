#include <stdlib.h>
#include <string.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "draw.h"
#include "music.h"

/**
 * Initialises all the SDL Windows, Renderers and loads the textures from the
 * game folder
 */
void initialiseSDL() {

  //Checks in case the device does not have a video card
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDLs:"
        " %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  //Creates the window and checks if creation is successful
  window = SDL_CreateWindow("QTClimb", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            SCREEN_WIDTH + SCREEN_SIDEBAR_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL window:"
        " %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  //Creates the renderer
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL "
        "renderer:" " %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }


  loadTexture("../Images/blank_image.bmp", &blank_texture);
  loadTexture("../Images/floor_image.bmp", &floor_texture);
  loadTexture("../Images/cracked_image.bmp", &cracked_texture);
  loadTexture("../Images/player_image.bmp", &player_texture);
  loadTexture("../Images/end_pos_image.bmp", &endpos_texture);
  loadTexture("../Images/splash_screen_image.bmp", &splash_screen);
  loadTexture("../Images/menu_background_image.bmp", &main_menu_background);
  loadTexture("../Images/level_menu_background_image.bmp", &level_menu_background);
  loadTexture("../Images/options_menu_background_image.bmp", &options_menu_background);
  loadTexture("../Images/end_level_menu_background_image.bmp", &end_level_menu_background);
  loadTexture("../Images/queens_tower.bmp", &queen_texture);
  loadTexture("../Images/credits_menu_background_image.bmp", &credit_screen);
  loadTexture("../Images/end_screen_image.bmp", &end_screen);
  loadTexture("../Images/intro_screen_image.bmp", &intro_screen);
  loadTexture("../Images/queens_tower_imperial.bmp", &queen_texture_imperial);
  loadTexture("../Images/tony_image.bmp", &tony_texture);
  loadTexture("../Images/side_helper_image.bmp", &side_helper_texture);


  TTF_Init();
  infinityFont = TTF_OpenFont("../Fonts/Infinity.ttf", 30);

  surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0,
                                 0);
}

/**
 * Loads a texture into a saved texture file
 * @param fileName is the name of the file to load
 * @param loadTexture is the name of the
 */
void loadTexture(char *fileName, SDL_Texture **loadTexture) {

  SDL_Surface *loadingSurface = NULL;
  loadingSurface = SDL_LoadBMP(fileName);
  if (loadingSurface == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't initialize SDL Texture: %s", SDL_GetError());
    printf("%s\n", fileName);
    exit(EXIT_FAILURE);
  }
  *loadTexture = SDL_CreateTextureFromSurface(renderer, loadingSurface);
  SDL_FreeSurface(loadingSurface);

}


/**
 * Renders the tiles for the floor map
 * @param map is the array of enums of the tile
 * @param endPos is the position of the target location
 * @param rows is the number rows in the map
 * @param cols is the number cols in the map
 */
void renderStage(squareState **map, playerPosition endPos, int rows, int cols) {

  int x_disp = (SCREEN_WIDTH - cols * SQUARE_SIZE) / 2;
  int y_disp = (SCREEN_HEIGHT - rows * SQUARE_SIZE) / 2;

  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      switch (map[r][c]) {
        case STEPPED:
          renderImage(c * SQUARE_SIZE + x_disp, r * SQUARE_SIZE + y_disp,
                      SQUARE_SIZE, SQUARE_SIZE, cracked_texture);
          break;
        case BLACK_SPACE:
          renderImage(c * SQUARE_SIZE + x_disp, r * SQUARE_SIZE + y_disp,
                      SQUARE_SIZE, SQUARE_SIZE, blank_texture);

          break;
        default:
          renderImage(c * SQUARE_SIZE + x_disp, r * SQUARE_SIZE + y_disp,
                      SQUARE_SIZE, SQUARE_SIZE, floor_texture);
      }
    }
  }

  //draw end position
  renderImage(endPos.c * SQUARE_SIZE + x_disp,
              endPos.r * SQUARE_SIZE + y_disp,
              SQUARE_SIZE, SQUARE_SIZE, endpos_texture);
}

/**
 * Renders only the changed tiles during the level loop
 * @param newPosition is the newPosition of the player
 * @param firstTime is a boolean variable to see if the function has been
 * called before
 * @param rows is the number rows in the map
 * @param cols is the number cols in the map
 * @return the gameIsRunning variable which represents the termination
 * condition for the game loop
 */
int renderUpdateStage(playerPosition newPosition, int firstTime, int rows,
                      int cols, SDL_Texture *curr_texture) {
  static playerPosition previousPos;
  int x_disp = (SCREEN_WIDTH - cols * SQUARE_SIZE) / 2;
  int y_disp = (SCREEN_HEIGHT - rows * SQUARE_SIZE) / 2;
  if (firstTime) {
    previousPos = newPosition;
    return 1;
  }
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT) {
    return 0;
  }

  renderImage(previousPos.c * SQUARE_SIZE + x_disp, previousPos.r *
                                                    SQUARE_SIZE + y_disp,
              SQUARE_SIZE, SQUARE_SIZE, cracked_texture);
  renderImage(newPosition.c * SQUARE_SIZE + x_disp, newPosition.r *
                                                    SQUARE_SIZE +
                                                    y_disp,
              SQUARE_SIZE, SQUARE_SIZE, curr_texture);

  previousPos = newPosition;
  return 1;
}

/**
 * Frees all the SDL components used in the function
 */
void clearUpSDL() {
  SDL_FreeSurface(surface);
  surface = NULL;
  SDL_DestroyRenderer(renderer);
  renderer = NULL;
  SDL_DestroyWindow(window);
  window = NULL;
  SDL_DestroyTexture(blank_texture);
  blank_texture = NULL;
  SDL_DestroyTexture(floor_texture);
  floor_texture = NULL;
  SDL_DestroyTexture(cracked_texture);
  cracked_texture = NULL;
  SDL_DestroyTexture(player_texture);
  player_texture = NULL;
  SDL_DestroyTexture(splash_screen);
  splash_screen = NULL;
  SDL_DestroyTexture(main_menu_background);
  main_menu_background = NULL;
  SDL_DestroyTexture(level_menu_background);
  level_menu_background = NULL;
  SDL_DestroyTexture(end_level_menu_background);
  end_level_menu_background = NULL;
  SDL_DestroyTexture(options_menu_background);
  options_menu_background = NULL;
  SDL_DestroyTexture(queen_texture);
  queen_texture = NULL;
  SDL_DestroyTexture(endpos_texture);
  endpos_texture = NULL;
  SDL_DestroyTexture(credit_screen);
  credit_screen = NULL;
  SDL_DestroyTexture(end_screen);
  end_screen = NULL;
  SDL_DestroyTexture(intro_screen);
  intro_screen = NULL;
  SDL_DestroyTexture(queen_texture_imperial);
  queen_texture_imperial = NULL;
  SDL_DestroyTexture(tony_texture);
  tony_texture = NULL;
  SDL_DestroyTexture(side_helper_texture);
  side_helper_texture = NULL;
  SDL_Quit();
  TTF_Quit();
}

/**
 * Updates the timer count during the level loop
 * @param currGame is the gameState struct
 * @return the string of the elasped time
 */
char *renderUpdateTime(gameState_t *currGame) {
  static char str[10];
  currGame->timer_count = SDL_GetTicks() - currGame->start_time
                          - currGame->level_menu_timer;

  strcpy(str, convertTimetoStr(currGame->timer_count));
  renderRect(renderer, SCREEN_WIDTH, SCREEN_HEIGHT - 100,
             SCREEN_SIDEBAR_WIDTH, 100,
             0, 0, 0);
  return str;
}

char *convertTimetoStr(uint32_t time) {
  static char str[10];
  int milliseconds = time % 1000;
  int seconds = time / 1000;
  char token[] = ":";
  sprintf(str, "%d%s%d%s", seconds, token, milliseconds, "s");
  return str;
}


/**
 * Renders a rectangle in a given position
 * @param renderer is the renderer to use
 * @param x is the x coordinate of the top left corner
 * @param y is the y coordinate of the top left corner
 * @param width is the width of the rectangle
 * @param height is the height of the rectangle
 * @param r is the r component of Colour
 * @param g is the g component of Colour
 * @param b is the b component of Colour
 */
void renderRect(SDL_Renderer *renderer, int x, int y, int width, int height,
                int r, int g, int b) {
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_Rect testRect;
  testRect.x = x;
  testRect.y = y;
  testRect.w = width;
  testRect.h = height;
  SDL_RenderFillRect(renderer, &testRect);
}

/**
 * Renders the Level counter based on the number of levels
 * @param currGame is the gameState struct
 */
void renderLevelCounter(gameState_t *currGame) {

  int displacement = (SCREEN_HEIGHT - 100) / (LEVEL_CAP);


  SDL_Rect destRect;
  destRect.x = SCREEN_WIDTH;
  destRect.y = (SCREEN_HEIGHT - 100) - ((displacement * currGame->levelCount)
                                        * 5) / 6;
  destRect.w = SCREEN_SIDEBAR_WIDTH;
  destRect.h = (displacement * currGame->levelCount * 5) / 6;

  SDL_Rect srcRect;
  srcRect.x = 0;
  srcRect.y = (SCREEN_HEIGHT) - (displacement * currGame->levelCount);
  srcRect.w = SCREEN_SIDEBAR_WIDTH;
  srcRect.h = displacement * (currGame->levelCount);

  SDL_RenderCopy(renderer, queen_texture_imperial, &srcRect, &destRect);
  // SDL_RenderPresent(renderer);
}


/**
 * Renders a rectangle outline in a given position
 * @param renderer is the renderer to use
 * @param x is the x coordinate of the top left corner
 * @param y is the y coordinate of the top left corner
 * @param width is the width of the rectangle
 * @param height is the height of the rectangle
 * @param r is the r component of Colour
 * @param g is the g component of Colour
 * @param b is the b component of Colour
 */
void renderRectOutline(SDL_Renderer *renderer, int x, int y, int width, int
height, int r, int g, int b) {
  SDL_SetRenderDrawColor(renderer, r, g, b, 255);
  SDL_Rect testRect;
  testRect.x = x;
  testRect.y = y;
  testRect.w = width;
  testRect.h = height;
  SDL_RenderDrawRect(renderer, &testRect);
}

/**
 * Renders the image in a target location
 * @param x is the x coordinate of the top left corner
 * @param y is the y coordinate of the top left corner
 * @param width is the width of the rectangle
 * @param height is the height of the rectangle
 * @param image_texture is the image texture to draw to screen
 */
void renderImage(int x, int y, int width, int height,
                 SDL_Texture *image_texture) {

  SDL_Rect destRect;
  destRect.x = x;
  destRect.y = y;
  destRect.w = width;
  destRect.h = height;
  SDL_RenderCopy(renderer, image_texture, NULL, &destRect);
}

/**
 * Draws the splash screen to the screen
 * @param time is the time for the splash screen to display
 */
void renderSplashScreen(uint32_t time) {
  SDL_Rect destRect;
  destRect.x = 0;
  destRect.y = 0;
  destRect.w = SCREEN_WIDTH + SCREEN_SIDEBAR_WIDTH;
  destRect.h = SCREEN_HEIGHT;
  SDL_RenderCopy(renderer, splash_screen, NULL, &destRect);
  SDL_RenderPresent(renderer);
  SDL_Delay(time);
  clearScreen();
}

/**
 * Renders the end screen
 */
void renderEndScreen() {
  SDL_Rect destRect;
  destRect.x = 0;
  destRect.y = 0;
  destRect.w = SCREEN_WIDTH;
  destRect.h = SCREEN_HEIGHT;

  SDL_RenderCopy(renderer, end_screen, NULL, &destRect);
  renderEndSideMenu();

  // SDL_RenderPresent(renderer);
}

/**
 * Renders the Intro Screen
 */
void renderIntroScreen() {
  SDL_Rect destRect;
  destRect.x = 0;
  destRect.y = 0;
  destRect.w = SCREEN_WIDTH;
  destRect.h = SCREEN_HEIGHT;
  renderSideMenu();
  SDL_RenderCopy(renderer, intro_screen, NULL, &destRect);
  // SDL_RenderPresent(renderer);
}

/**
 * Renders the Menu  background
 * @param menu_background
 */
void renderMenu(SDL_Texture *menu_background) {
  //Draw Menu background
  SDL_Rect destRect;
  destRect.x = 0;
  destRect.y = 0;
  destRect.w = SCREEN_WIDTH;
  destRect.h = SCREEN_HEIGHT;
  SDL_RenderCopy(renderer, menu_background, NULL, &destRect);
  // SDL_RenderPresent(renderer);
}

/**
 * Renders the side menu timer
 * @param str is the str of the time to render
 */
void renderSideMenuTimer() {
  SDL_Rect destRect;
  destRect.x = SCREEN_WIDTH;
  destRect.y = 0;
  destRect.w = SCREEN_SIDEBAR_WIDTH;
  destRect.h = SCREEN_HEIGHT - 100;

  SDL_RenderCopy(renderer, queen_texture, NULL, &destRect);
  // SDL_RenderPresent(renderer);
}

/**
 * Renders the leaderboard to the sidemenu
 * @param currGame is the gameState struct
 * @param str is the str of the time to render
 */
void renderLeaderboard(gameState_t *currGame) {

  SDL_Color color = {0, 255, 255};
  SDL_Surface *leaderBoard_surface = TTF_RenderText_Solid(infinityFont,
                                                          "Leaderboard", color);

  SDL_Texture *leaderboardTexture = SDL_CreateTextureFromSurface(renderer,
                                                                 leaderBoard_surface);
  SDL_Rect leaderboard;

  leaderboard.x = 600;
  leaderboard.y = 0;
  leaderboard.w = SCREEN_SIDEBAR_WIDTH;
  leaderboard.h = 100;
  SDL_RenderCopy(renderer, leaderboardTexture, NULL, &leaderboard);
  SDL_FreeSurface(leaderBoard_surface);


  leaderboard.h = (400 / 3);
  int i = 0;
  for (leaderBoard *current = currGame->head; current != NULL; current =
                                                                   current->next) {

    char str[11];
    if (current->time != (uint32_t) -1) {
      strcpy(str, convertTimetoStr(current->time));
    } else {
      strcpy(str, "-");
    }
    leaderBoard_surface = TTF_RenderText_Solid(infinityFont, str, color);
    leaderboardTexture = SDL_CreateTextureFromSurface(renderer,
                                                      leaderBoard_surface);

    leaderboard.y = 100 + (i * (400 / 3));

    SDL_RenderCopy(renderer, leaderboardTexture, NULL, &leaderboard);
    SDL_FreeSurface(leaderBoard_surface);
    i++;
  }
}

/**
 * Clears the screen and fills it with black
 */
void clearScreen() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}

/**
 * Renders the side menu
 */
void renderSideMenu() {
  SDL_Rect destRect;
  destRect.x = 600;
  destRect.y = 0;
  destRect.w = SCREEN_SIDEBAR_WIDTH;
  destRect.h = SCREEN_HEIGHT;
  SDL_RenderCopy(renderer, queen_texture, NULL, &destRect);
  // SDL_RenderPresent(renderer);
}

/**
 * Renders the timer
 * @param displayText is the text to display
 */
void renderTimer(char *displayText) {
  SDL_Color color = {0, 255, 255};
  SDL_Surface *surfaceTimer = TTF_RenderText_Solid(infinityFont,
                                                   displayText, color);

  SDL_Texture *timerTexture = SDL_CreateTextureFromSurface(renderer,
                                                           surfaceTimer);
  SDL_Rect timerLocation;

  timerLocation.x = 600;
  timerLocation.y = 500;
  timerLocation.w = SCREEN_SIDEBAR_WIDTH;
  timerLocation.h = 100;
  SDL_RenderCopy(renderer, timerTexture, NULL, &timerLocation);
  // SDL_RenderPresent(renderer);
  SDL_FreeSurface(surfaceTimer);
}

/**
 * Draws the solution step by step
 * @param currGame is the gameState struct
 */
void draw_solution(gameState_t *currGame) {
  resetLevel(currGame);
  renderRect(renderer, SCREEN_WIDTH, 0, SCREEN_SIDEBAR_WIDTH, SCREEN_HEIGHT, 0, 0,
             0);
  SDL_RenderPresent(renderer);
  playMusic(helperMusic);

  for (int i = 0; i < currGame->count; i++) {
    int lastTicks = SDL_GetTicks();
    renderUpdateStage(*(currGame->solArray[i]), 0, currGame->rows,
                      currGame->cols, tony_texture);
    int displacement = SCREEN_HEIGHT / (currGame->count - 1);
    renderSideHelper(i + 1, displacement);
    SDL_RenderPresent(renderer);
    int delay = 1000 / GIVE_UP_FPS_LIMIT - SDL_GetTicks() + lastTicks;
    if (delay > 0) {
      SDL_Delay(delay);
    }
  }
  stopMusic(2000);
}

void renderSideHelper(int i, int displacement) {

  SDL_Rect destRect;
  destRect.x = SCREEN_WIDTH;
  destRect.y = SCREEN_HEIGHT - (displacement * i);
  destRect.w = SCREEN_SIDEBAR_WIDTH;
  destRect.h = displacement * i;

  SDL_Rect srcRect;
  srcRect.x = 0;
  srcRect.y = SCREEN_HEIGHT - (displacement * i);
  srcRect.w = SCREEN_SIDEBAR_WIDTH;
  srcRect.h = displacement * i;

  SDL_RenderCopy(renderer, side_helper_texture, &srcRect, &destRect);
  // SDL_RenderPresent(renderer);
}


/**
 * Draws the credit screen
 */
void renderCredits() {
  SDL_Rect destRect;
  destRect.x = 0;
  destRect.y = 0;
  destRect.w = SCREEN_WIDTH;
  destRect.h = SCREEN_HEIGHT;
  renderSideMenu();
  SDL_RenderCopy(renderer, credit_screen, NULL, &destRect);
  // SDL_RenderPresent(renderer);
}

void renderEndSideMenu() {

  SDL_Rect destRect;
  destRect.x = 600;
  destRect.y = 0;
  destRect.w = SCREEN_SIDEBAR_WIDTH;
  destRect.h = SCREEN_HEIGHT;

  SDL_RenderCopy(renderer, queen_texture_imperial, NULL, &destRect);
  // SDL_RenderPresent(renderer);

}