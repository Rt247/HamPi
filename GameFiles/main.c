#include "draw.h"
#include <time.h>
#include "music.h"

#undef main

int main(int argc, char *argv[]) {

  //Set a random seed based on the system time
  srand((unsigned int) time(NULL));

  //Instantiate the single gameState struct
  gameState_t currGameState;
  gameState_t *currGame = &currGameState;

  //The default states at start up
  currGame->currGameMode = RANDOM;
  currGame->difficultyGameMode = EASY;
  currGame->levelCount = 0;

  //Initialise the variables in the gameState and leader board
  initialiseGameState(currGame);
  initialiseLeaderboard(currGame);

  //Initialise all the SDL components including loading the textures
  initialiseSDL();
  initialiseSDLMixer();

  //Render the splash screen for a preset time while playing the intro music
  playMusic(introMusic);
  renderSplashScreen(SPLASH_SCREEN_TIME);

  //Render the intro screen as well implement the interface for that screen
  renderIntroScreen();
  creditMenuLogic(currGame);

  if (currGame->gameIsRunning) {
    //Render the main menu screen as well implement the interface for that
    //screen
    renderSideMenu();
    renderMenu(main_menu_background);
    mainMenuLogic(currGame);
    stopMusic(10);
  }

  //Bypass the main game loop if user quits the game from main menu interface
  if (!(currGame->gameIsRunning)) {
    return 0;
  }

  playLevelMusic();

  //The main game loop which represents the overall game and levels
  while (currGame->gameIsRunning) {

    if (!Mix_PlayingMusic()) {
      playLevelMusic();
    }

    //Get the time before starting the level
    currGame->start_time = SDL_GetTicks();


    //The level game loop which represents the individual levels
    //Exit conditions are if the user quits in one of the submenus or the
    // player reaches the end of the level
    while (currGame->nMoves < currGame->count && currGame->gameIsRunning) {

      //TODO GET NES INPUT  HERE

      int lastTicks = SDL_GetTicks();

      if (!Mix_PlayingMusic()) {
        playLevelMusic();
      }

      //If the user presses select then reset the level
      //If the user presses start then load up the level menu
      //Else just calculate the new position of the player
      if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {

        resetLevel(currGame);

      } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym ==
                                              SDLK_l) {

        //Render the Level Menu as well as loading the level menu
        // interface
        clearScreen();
        if (currGame->currGameMode == RANDOM) {
          renderSideMenuTimer();
          renderLevelCounter(currGame);
        } else if (currGame->currGameMode == SAVED) {
          renderLeaderboard(currGame);
        }
        renderTimer(convertTimetoStr(currGame->timer_count));
        renderMenu(level_menu_background);
        levelMenuLogic(currGame);

        //Pause the timer while the player is in the level menu
        currGame->level_menu_timer = SDL_GetTicks() -
                                     currGame->timer_count -
                                     currGame->start_time;
      } else {

        //Update the moves
        //TODO pass in gamestate struct
        currGame->nMoves += movePlayer(currGame->level, &(currGame->playerPos),
                                       currGame->rows, currGame->cols);
      }

      //Bypass the level game loop if user quits the game from level menu
      // interface
      if (currGame->gameIsRunning) {

        //Render only the changed tiles to improve efficiency
        currGame->gameIsRunning = renderUpdateStage
            (currGame->playerPos, 0, currGame->rows,
             currGame->cols, player_texture);

        //Holds the new current time as a string
        char *str = renderUpdateTime(currGame);

        //Display the sidemenu depending on which gamemode is set
        if (currGame->currGameMode == SAVED) {
          renderTimer(str);
          renderLeaderboard(currGame);
        } else if (currGame->currGameMode == RANDOM) {
          renderTimer(str);
          renderLevelCounter(currGame);
        }

        //If the player collides with the endPosition, reset the level
        if (currGame->playerPos.c == currGame->endPosLevel.c &&
            currGame->playerPos.r == currGame->endPosLevel.r &&
            currGame->nMoves < currGame->count) {
          resetLevel(currGame);
        }

        //Render everything to the screen as well delay
        SDL_RenderPresent(renderer);

        int delay = 1000 / MENU_FPS_LIMIT - SDL_GetTicks() + lastTicks;
        if (delay > 0) {
          SDL_Delay(delay);
        }
      }

    }

    //Bypass the end  if user quits the game from main menu
    // interface
    if (currGame->gameIsRunning) {
      //Created an endgame according to the difficulty level set
      if (currGame->levelCount >= LEVEL_CAP) {

        //Render the end screen as well implement the interface
        // for that screen
        stopMusic(100);
        playMusic(endMusic);
        clearScreen();
        renderEndScreen();
        creditMenuLogic(currGame);
        currGame->levelCount = 0;
        stopMusic(1000);

        //Render the main menu screen as well implement the interface
        // for that screen
        playMusic(introMusic);
        clearScreen();
        renderSideMenu();
        renderMenu(main_menu_background);
        mainMenuLogic(currGame);

      } else if (currGame->currGameMode == SAVED) {
        clearScreen();

        char str[10];
        sprintf(str, "%d", currGame->timer_count);
        renderLeaderboard(currGame);
        currGame->levelCount = 0;
        renderTimer(str);
        renderMenu(end_level_menu_background);
        endlevelMenuLogic(currGame);

      } else {

        clearScreen();

        //Render the time taken to complete the level at the end
        // screen menu
        //Holds the new current time as a string
        char *str = renderUpdateTime(currGame);
        renderLevelCounter(currGame);

        //Render the end menu screen as well implement the interface
        // for that screen
        renderSideMenuTimer();
        renderLevelCounter(currGame);
        renderTimer(str);
        renderMenu(end_level_menu_background);
        endlevelMenuLogic(currGame);
      }

    }
  }

  if (Mix_PlayingMusic()) {
    stopMusic(0);
    SDL_Delay(100);
  }


  //Free all the resources and memory used in the game
  freeLeaderBoard(currGame->head);
  currGame->head = NULL;
  freeCurrGame(currGame);
  clearUpSDL();
  clearUpSDLMusic();
  return EXIT_SUCCESS;
}



