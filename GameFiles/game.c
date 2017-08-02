#include "game.h"
#include "gameUtils.h"
#include "randomGen.h"
#include "SDL2/SDL.h"
#include "draw.h"
#include "io.h"
#include "string.h"
#include "music.h"

/**
 * Initialises the game to empty values
 * @param currGame is the gameState struct
 */
void initialiseGameState(gameState_t *currGame) {
  currGame->nMoves = 0;
  currGame->rows = 0;
  currGame->cols = 0;
  currGame->level = NULL;
  currGame->levelMap = NULL;
  currGame->count = 0;
  currGame->gameIsRunning = 1;
  currGame->timer_count = 0;
  currGame->start_time = 0;
  currGame->level_menu_timer = 0;
}

void initialiseLeaderboard(gameState_t *currGame) {
  currGame->head = allocateLeaderBoard();
  currGame->head->time = (uint32_t) -1;
  leaderBoard *current = currGame->head;
  for (int i = 0; i < 2; ++i) {
    current->next = allocateLeaderBoard();
    current->next->time = (uint32_t) -1;
    current = current->next;
  }
  current->next = NULL;
}

/**
 * The interface for the main menu
 * @param currGame is the gameState struct
 */
void mainMenuLogic(gameState_t *currGame) {

  //Render the UCL side menu holder
  renderSideMenu();

  //Set the currChoice to Nothing which acts as a default value
  menuOptions_t currChoice = NOTHING;
  //The options for the main menu interface
  menuOptions_t choices[NUM_CHOICES] = {START, OPTIONS, CREDITS, EXIT};
  //Get the input from the user assuming its not the last level
  currChoice = getMenuInput(choices, NUM_CHOICES, main_menu_background,
                            0, currGame);

  //Switch based on the user input
  switch (currChoice) {
    case START:
      //Load a random level or a saved level
      if (currGame->currGameMode == RANDOM) {
        clearScreen();
        initialiseGameState(currGame);
        loadRandomLevel(currGame);

      } else if (currGame->currGameMode == SAVED) {
        clearScreen();
        initialiseGameState(currGame);
        loadSavedLevel(currGame);
      }
      break;
    case OPTIONS:

      //Render the options screen as well implement the interface
      // for that screen
      clearScreen();
      renderSideMenu();
      renderMenu(options_menu_background);

      //Get the input from the options menu
      currChoice = getOptMenuInput(currGame);

      if (currChoice == EXIT) {
        currGame->gameIsRunning = 0;
      } else if (currChoice == MAIN_MENU) {
        //Render the main menu screen as well implement the interface
        // for that screen
        clearScreen();
        renderMenu(main_menu_background);
        mainMenuLogic(currGame);
      }
      break;
    case CREDITS:
      //Render the credits screen as well implement the interface
      // for that screen
      clearScreen();
      renderCredits();
      creditMenuLogic(currGame);

      //Render the main menu screen as well implement the interface
      // for that screen
      clearScreen();
      renderSideMenu();
      renderMenu(main_menu_background);
      mainMenuLogic(currGame);
      break;
    case EXIT:
      currGame->gameIsRunning = 0;
      break;
    default:
      perror("Should not have reached the default case in main menu "
                 "logic");
      exit(EXIT_FAILURE);
  }
}

/**
 * The interface for the credit menu
 */
void creditMenuLogic(gameState_t *currGame) {

  //Draw the selection box for the sole selection option
  renderRectOutline(renderer, X_DISP, X_WIDTH_SIZE + Y_WIDTH_SIZE *
                                                     SELECTION_DISPLACEMENT
                                      + 5,
                    X_WIDTH_SIZE, Y_WIDTH_SIZE - 25, 0, 0, 255);
  // SDL_RenderPresent(renderer);

  int game_running = 1;
  //Loop for getting the user input to continue
  while (game_running) {
    int lastTicks = SDL_GetTicks();
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
      currGame->gameIsRunning = 0;
      game_running = 0;
      clearScreen();
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a) {
      game_running = 0;
    }
    int delay = 1000 / MENU_FPS_LIMIT - SDL_GetTicks() + lastTicks;
    if (delay > 0) {
      SDL_Delay(delay);
    }
    // SDL_Delay(120);
    SDL_RenderPresent(renderer);
  }
}

/**
 * The interface for the level menu
 * @param currGame is the gameState struct
 */
void levelMenuLogic(gameState_t *currGame) {

  //Set the currChoice to Nothing which acts as a default value
  menuOptions_t currChoice;
  //The options for the level menu interface
  menuOptions_t choices[4] = {RESUME, SAVE_LEVEL, GIVE_UP, MAIN_MENU};
  //Get the input from the user assuming its not the last level
  currChoice = getMenuInput(choices, 4, level_menu_background, 1, currGame);

  //Switch based on the user input
  switch (currChoice) {
    case RESUME:
      //Render the previous screen before the level menu
      clearScreen();
      if (currGame->currGameMode == RANDOM) {
        renderSideMenuTimer();
        renderLevelCounter(currGame);
      } else {
        renderLeaderboard(currGame);
      }

      renderStage(currGame->level, currGame->endPosLevel, currGame->rows,
                  currGame->cols);
      break;
    case MAIN_MENU:
      clearScreen();
      stopMusic(100);

      //Clear the current game parameters
      //freeCurrGame(currGame);
      //Render the main menu screen as well implement the interface
      // for that screen

      playMusic(introMusic);
      renderSideMenu();
      renderMenu(main_menu_background);
      mainMenuLogic(currGame);
      break;
    case SAVE_LEVEL:

      //Save the level and render the level menu screen as well
      // implement the interface for that screen
      saveLevel(currGame);
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
      break;
    case GIVE_UP:
      clearScreen();
      stopMusic(500);

      //Draw the screen frame by frame
      renderRect(renderer, SCREEN_WIDTH, 0, SCREEN_SIDEBAR_WIDTH, SCREEN_HEIGHT, 0, 0,
                 0);
      SDL_RenderPresent(renderer);
      draw_solution(currGame);
      clearScreen();

      //Clear the current game parameters
      //freeCurrGame(currGame);

      //Render the main menu screen as well implement the interface
      // for that screen
      if (currGame->currGameMode == RANDOM) {
        if (currGame->levelCount > 0) {
          currGame->levelCount--;
        }
        currGame->timer_count = 0;
        currGame->level_menu_timer = 0;
        currGame->start_time = SDL_GetTicks();
        loadRandomLevel(currGame);
      } else {
        renderSideMenu();
        renderMenu(main_menu_background);
        mainMenuLogic(currGame);
      }

      break;
  }
}

/**
 * Saves the necessary parameters within currGame to a the approriate files
 * @param currGame is the gameState struct
 */
void saveLevel(gameState_t *currGame) {
  writeMap("../Files/saved_level.txt", currGame->levelMap, currGame->rows,
           currGame->cols);
  writeSolution("../Files/saved_level_solution", currGame->solArray,
                currGame->count);
  writeLeaderBoard("../Files/leaderboard", currGame->head);
}

/**
 * Increments the level based on the which difficulty is set
 * @param currGame is the gameState struct
 */
void incrementLevel(gameState_t *currGame) {

  (currGame->levelCount)++;

//    switch(currGame->difficultyGameMode){
//        case EASY:
//            currGame->levelCount += 4;
//            break;
//        case MEDIUM:
//            currGame->levelCount += 2;
//            break;
//        case HARD:
//            currGame->levelCount += 1;
//            break;
//    }
}

/**
 * The interface for the end level menu
 * @param currGame is the gameState struct
 */
void endlevelMenuLogic(gameState_t *currGame) {

  //Set the currChoice to Nothing which acts as a default value
  menuOptions_t currChoice = NOTHING;
  //The options for the level menu interface
  menuOptions_t choices[4] = {NEXT_LEVEL, MAIN_MENU, SAVE_LEVEL, EXIT};
  //Get the input from the user assuming it is the last level
  currChoice = getMenuInput(choices, 4, end_level_menu_background, 1,
                            currGame);

  //Switch based on the user input
  switch (currChoice) {
    case NEXT_LEVEL:
      if (currGame->currGameMode == RANDOM) {

        currGame->timer_count = 0;
        currGame->level_menu_timer = 0;
        clearScreen();
        //Loads the next level as well as incrementing the level
        incrementLevel(currGame);
        loadRandomLevel(currGame);
      } else if (currGame->currGameMode == SAVED) {
        //Loads the saved level again to replay it
        clearScreen();
        currGame->head = updateLeaderBoard(currGame->head,
                                           currGame->timer_count);
        writeLeaderBoard("../Files/leaderboard", currGame->head);
        initialiseGameState(currGame);
        loadSavedLevel(currGame);
      }
      break;
    case MAIN_MENU:
      clearScreen();

      //Resets the level count
      currGame->levelCount = 0;
      currGame->timer_count = 0;
      currGame->level_menu_timer = 0;
      //Render the main menu screen as well implement the interface
      // for that screen
      renderSideMenu();
      renderMenu(main_menu_background);
      mainMenuLogic(currGame);
      break;
    case SAVE_LEVEL:
      clearScreen();
      if (currGame->currGameMode == RANDOM) {
        renderSideMenuTimer();
        renderLevelCounter(currGame);
      } else if (currGame->currGameMode == SAVED) {
        renderLeaderboard(currGame);
      }
      renderTimer(convertTimetoStr(currGame->timer_count));
      //Call the save level function to save the level
      saveLevel(currGame);
      currGame->head = updateLeaderBoard(currGame->head,
                                         currGame->timer_count);
      writeLeaderBoard("../Files/leaderboard", currGame->head);


      //Render the end menu screen as well implement the interface
      // for that screen
      renderMenu(end_level_menu_background);
      endlevelMenuLogic(currGame);
      break;
    case EXIT:
      currGame->timer_count = 0;
      currGame->level_menu_timer = 0;
      currGame->gameIsRunning = 0;
      break;
  }
}

/**
 * Loads a random level and assigns the variables to the gameStates
 * @param currGame is the gameState struct
 */
void loadRandomLevel(gameState_t *currGame) {

  freeCurrGame(currGame);
  currGame->start_time = SDL_GetTicks();
  resetLeaderBoard(currGame->head);
  currGame->rows = (rand() % 3) + currGame->difficultyGameMode;
  currGame->cols = (rand() % 3) + currGame->difficultyGameMode;
  currGame->level = getMapPtr(currGame->rows, currGame->cols);
  currGame->levelMap = generate(currGame->rows, currGame->cols, &
      (currGame->initPosLevel), currGame->solArray, &(currGame->endPosLevel));
  setLevel(currGame->level, currGame->levelMap, &(currGame->playerPos),
           &(currGame->initPosLevel), currGame->rows, currGame->cols);
  currGame->count = countUnstepped(currGame->levelMap, currGame->rows, currGame->cols);
  currGame->nMoves = 0;
  renderStage(currGame->level, currGame->endPosLevel, currGame->rows, currGame->cols);
  renderSideMenuTimer();
  renderUpdateStage(currGame->playerPos, 1, currGame->rows, currGame->cols,
                    player_texture);
}

/**
 * Loads a saved level from the contents of the input files to the game struct
 * @param currGame
 */
void loadSavedLevel(gameState_t *currGame) {

  freeCurrGame(currGame);
  currGame->start_time = SDL_GetTicks();
  currGame->levelMap = readMap("../Files/saved_level.txt", &(currGame->rows), &
      (currGame->cols));
  findPlayerPos(currGame);
  currGame->level = getMapPtr(currGame->rows, currGame->cols);
  setLevel(currGame->level, currGame->levelMap, &(currGame->playerPos),
           &(currGame->initPosLevel), currGame->rows, currGame->cols);
  readLeaderBoard("../Files/leaderboard", currGame->head);
  currGame->count = countUnstepped(currGame->levelMap, currGame->rows, currGame->cols);
  playerPosition **tempSolArr = readSolution("../Files/saved_level_solution");
  memcpy(currGame->solArray, tempSolArr, currGame->count * sizeof(playerPosition *));
  currGame->nMoves = 0;
  renderStage(currGame->level, currGame->endPosLevel, currGame->rows, currGame->cols);
  renderUpdateStage(currGame->playerPos, 1, currGame->rows, currGame->cols,
                    player_texture);
}

/**
 * Finds the initial player position and the end player position in the level
 * map and assigns the variables to the gameState
 * @param currGame
 */
void findPlayerPos(gameState_t *currGame) {
  for (int r = 0; r < currGame->rows; r++) {
    for (int c = 0; c < currGame->cols; c++) {
      if (currGame->levelMap[r][c] == PLAYER) {
        currGame->initPosLevel.r = r;
        currGame->initPosLevel.c = c;
      } else if (currGame->levelMap[r][c] == END_POSITION) {
        currGame->endPosLevel.r = r;
        currGame->endPosLevel.c = c;
      }
    }
  }
}

//TODO ADD NES CODE HERE
/**
 * Calculates the menu input from the user input
 * @param choices is the options according to that menu
 * @param num_options is the number of those options
 * @param menu_background is the menu background texture
 * @param isEndLevel is a boolean to see if the function is called from the
 * end of the level
 * @param currGame is the gameState struct
 * @return the menu chosen by the user
 */
menuOptions_t getMenuInput(menuOptions_t choices[], int num_options,
                           SDL_Texture *menu_background, int isEndLevel,
                           gameState_t *currGame) {
  int index = 0;

  //Renders the initial position of the selection box
  renderRectOutline(renderer, X_DISP,
                    Y_DISP + index * Y_WIDTH_SIZE,
                    X_WIDTH_SIZE,
                    Y_WIDTH_SIZE, 0, 0, 255);

  int game_running = 1;
  //The loop for the input choices
  while (game_running) {
    int lastTicks = SDL_GetTicks();

    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
      game_running = 0;
    }
    //Button for selecting the option
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a) {
      game_running = 0;
    }
    //Button for moving the selection box
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN) {

      //If the level is the last level render draw the final timer count
      //Else draw the normal side menu
      if (currGame->currGameMode == RANDOM) {
        if (isEndLevel) {
          renderTimer(convertTimetoStr(currGame->timer_count));
          renderSideMenuTimer();
          renderLevelCounter(currGame);
        } else {
          renderSideMenu();
        }
      } else if (isEndLevel && currGame->currGameMode == SAVED) {
        renderLeaderboard(currGame);
      }
      renderMenu(menu_background);

      //Increment the index and normalise it to the range 0 - 4
      index++;
      index = (index + num_options) % num_options;
      //Draw the new selection box
      renderRectOutline(renderer, X_DISP,
                        Y_DISP + index * Y_WIDTH_SIZE,
                        X_WIDTH_SIZE,
                        Y_WIDTH_SIZE, 0, 0, 255);
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP) {

      //If the level is the last level render draw the final timer count
      //Else draw the normal side menu
      if (currGame->currGameMode == RANDOM) {
        if (isEndLevel) {
          renderTimer(convertTimetoStr(currGame->timer_count));
          renderSideMenuTimer();
          renderLevelCounter(currGame);
        } else {
          renderSideMenu();
        }
      } else if (isEndLevel) {
        renderLeaderboard(currGame);
      }
      renderMenu(menu_background);

      //Increment the index and normalise it to the range 0 - 4
      index = (index + 3) % num_options;
      //Draw the new selection box
      renderRectOutline(renderer, X_DISP,
                        Y_DISP + index * Y_WIDTH_SIZE,
                        X_WIDTH_SIZE,
                        Y_WIDTH_SIZE, 0, 0, 255);
    }
    int delay = 1000 / MENU_FPS_LIMIT - SDL_GetTicks() + lastTicks;
    if (delay > 0) {
      SDL_Delay(delay);
    }
    // SDL_Delay(120);
    SDL_RenderPresent(renderer);
  }
  return choices[index];
}

//TODO ADD NES CODE HERE
/**
 * Sorts the interface for the specific
 * @param currGame is the gameState struct
 * @return the chosen option for the options menus
 */
menuOptions_t getOptMenuInput(gameState_t *currGame) {


  int index = 0;
  int game_running = 1;
  int switch_choice = 0;

  //Renders the initial position of the selection box
  renderRectOutline(renderer, X_DISP_FIRST,
                    Y_DISP_FIRST,
                    X_WIDTH_SIZE_OPT,
                    Y_WIDTH_SIZE_OPT, 0, 0, 255);
  SDL_RenderPresent(renderer);

  //The loop for the input choices based on when the user selects the option
  //The selection options are hard coded in the switch cases
  while (game_running) {
    int lastTicks = SDL_GetTicks();
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
      game_running = 0;
    }

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_a) {
      switch (switch_choice) {
        case 0:
          if (index == 0) {
            renderSideMenu();
            renderMenu(options_menu_background);
            renderRectOutline(renderer, X_DISP_SECOND,
                              Y_DISP_SECOND,
                              X_WIDTH_SIZE_OPT,
                              Y_WIDTH_SIZE_OPT, 0, 0, 255);
            switch_choice = 1;
            index = 0;
            SDL_RenderPresent(renderer);
          } else if (index == 1) {
            renderSideMenu();
            renderMenu(options_menu_background);
            renderRectOutline(renderer, X_DISP_THIRD,
                              Y_DISP_THIRD,
                              X_WIDTH_SIZE_OPT,
                              Y_WIDTH_SIZE_OPT, 0, 0, 255);
            switch_choice = 2;
            index = 0;
            SDL_RenderPresent(renderer);
          }
          break;
        case 1:
          if (index == 0) {
            currGame->currGameMode = RANDOM;
            clearScreen();
          } else if (index == 1) {
            currGame->currGameMode = SAVED;
            SDL_RenderClear(renderer);
            //TODO DO SAVED CODE
          }
          return MAIN_MENU;
        case 2:
          if (index == 0) {
            currGame->difficultyGameMode = EASY;
            clearScreen();
          } else if (index == 1) {
            currGame->difficultyGameMode = MEDIUM;
            clearScreen();
          } else if (index == 2) {
            currGame->difficultyGameMode = HARD;
            clearScreen();
          }
          return MAIN_MENU;
      }
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_b) {
      switch (switch_choice) {
        case 0:
          clearScreen();
          return MAIN_MENU;
        case 1:
          renderSideMenu();
          renderMenu(options_menu_background);
          renderRectOutline(renderer, X_DISP_FIRST,
                            Y_DISP_FIRST,
                            X_WIDTH_SIZE_OPT,
                            Y_WIDTH_SIZE_OPT, 0, 0, 255);
          switch_choice = 0;
          index = 0;
          SDL_RenderPresent(renderer);
          break;
        case 2:
          renderSideMenu();
          renderMenu(options_menu_background);
          renderRectOutline(renderer, X_DISP_FIRST,
                            Y_DISP_THIRD,
                            X_WIDTH_SIZE_OPT,
                            Y_WIDTH_SIZE_OPT, 0, 0, 255);
          switch_choice = 0;
          index = 1;
          SDL_RenderPresent(renderer);
          break;
      }
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym ==
                                     SDLK_DOWN) {

      switch (switch_choice) {
        case 0:
          renderSideMenu();
          renderMenu(options_menu_background);
          index++;
          index = (index + 2) % 2;
          renderRectOutline(renderer, X_DISP_FIRST,
                            Y_DISP_FIRST + index * 2 * Y_WIDTH_SIZE_OPT,
                            X_WIDTH_SIZE_OPT,
                            Y_WIDTH_SIZE_OPT, 0, 0, 255);
          SDL_RenderPresent(renderer);
          break;
        case 1:
          renderSideMenu();
          renderMenu(options_menu_background);
          index++;
          index = (index + 2) % 2;
          renderRectOutline(renderer, X_DISP_SECOND,
                            Y_DISP_SECOND + index * Y_WIDTH_SIZE_OPT,
                            X_WIDTH_SIZE_OPT,
                            Y_WIDTH_SIZE_OPT, 0, 0, 255);
          SDL_RenderPresent(renderer);
          break;
        case 2:
          renderSideMenu();
          renderMenu(options_menu_background);
          index++;
          index = (index + 3) % 3;
          renderRectOutline(renderer, X_DISP_THIRD,
                            Y_DISP_THIRD + index * Y_WIDTH_SIZE_OPT,
                            X_WIDTH_SIZE_OPT,
                            Y_WIDTH_SIZE_OPT, 0, 0, 255);
          SDL_RenderPresent(renderer);
          break;
      }
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.sym ==
                                     SDLK_UP) {

      switch (switch_choice) {
        case 0:
          renderSideMenu();
          renderMenu(options_menu_background);
          index = (index + 1) % 2;
          renderRectOutline(renderer, X_DISP_FIRST,
                            Y_DISP_FIRST + index * 2 * Y_WIDTH_SIZE_OPT,
                            X_WIDTH_SIZE_OPT,
                            Y_WIDTH_SIZE_OPT, 0, 0, 255);
          SDL_RenderPresent(renderer);
          break;
        case 1:
          renderSideMenu();
          renderMenu(options_menu_background);
          index = (index + 1) % 2;
          renderRectOutline(renderer, X_DISP_SECOND,
                            Y_DISP_SECOND + index * Y_WIDTH_SIZE_OPT,
                            X_WIDTH_SIZE_OPT,
                            Y_WIDTH_SIZE_OPT, 0, 0, 255);
          SDL_RenderPresent(renderer);
          break;
        case 2:
          renderSideMenu();
          renderMenu(options_menu_background);
          index = (index + 2) % 3;
          renderRectOutline(renderer, X_DISP_THIRD,
                            Y_DISP_THIRD + index * Y_WIDTH_SIZE_OPT,
                            X_WIDTH_SIZE_OPT,
                            Y_WIDTH_SIZE_OPT, 0, 0, 255);
          SDL_RenderPresent(renderer);
          break;
      }
    }
    int delay = 1000 / MENU_FPS_LIMIT - SDL_GetTicks() + lastTicks;
    if (delay > 0) {
      SDL_Delay(delay);
    }
    // SDL_Delay(120);

  }
  return EXIT;
}

/**
 * Resets the parameters in the currGame struct to the initial values
 * @param currGame is the gamestate struct
 */
void resetLevel(gameState_t *currGame) {
  printf("Resetting...\n");
  setLevel(currGame->level, currGame->levelMap, &
               (currGame->playerPos),
           &(currGame->initPosLevel),
           currGame->rows, currGame->cols);
  currGame->nMoves = 0;
  currGame->level_menu_timer = 0;
  renderStage(currGame->level, currGame->endPosLevel, currGame->rows, currGame->cols);
  if (currGame->currGameMode == RANDOM) {
    renderSideMenuTimer();
  }
  renderUpdateStage(currGame->playerPos, 1, currGame->rows, currGame->cols,
                    player_texture);

}

/**
 * Frees the memory allocation used in the game
 * @param currGame is the gamestate struct
 */
void freeCurrGame(gameState_t *currGame) {
  //currGame->levelCount = 0;
  freeMap(currGame->levelMap);
  freeMap(currGame->level);
  resetLeaderBoard(currGame->head);
}