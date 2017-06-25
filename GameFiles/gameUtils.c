#include "gameUtils.h"
#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL_keycode.h>
#include "draw.h"
#include <assert.h>

squareState r1[] = {UNSTEPPED, END_POSITION, BLACK_SPACE};
squareState r2[] = {UNSTEPPED, UNSTEPPED, UNSTEPPED};
squareState r3[] = {BLACK_SPACE, PLAYER, UNSTEPPED};
squareState *level1[] = {r1,r2, r3};
playerPosition initPosLevel1 = {2, 1};


squareState **getMapPtr(int rows, int cols) {
  squareState **mapPtr = (squareState **) malloc(rows * sizeof(squareState *));
  if (mapPtr == NULL) {
    perror("Unable to allocate memory for the map pointers!");
    exit(EXIT_FAILURE);
  }
  mapPtr[0]
      = (squareState *)calloc((size_t) (rows * cols), sizeof(squareState));
  if (mapPtr[0] == NULL) {
    perror("Unable to allocate memory for the map!");
    exit(EXIT_FAILURE);
  }
  for (int i = 1; i < rows; ++i) {
    mapPtr[i] = mapPtr[i - 1] + cols;
  }
  return mapPtr;
}

squareState **initMap(squareState **mapPtr, squareState **levelMap, int rows, int cols) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      mapPtr[i][j] = levelMap[i][j];
    }
  }
  return mapPtr;
}



void freeMap(squareState **mapPtr) {
  if(mapPtr != NULL) {
    free(mapPtr[0]);
    free(mapPtr);
  }
}

playerPosition *initPosition(int r, int c) {
  playerPosition *pos = malloc(sizeof(playerPosition));
  if (pos == NULL) {
    perror("initPosition: Unable to allocate position");
    exit(EXIT_FAILURE);
  }
  pos->r = r;
  pos->c = c;
  return pos;
}

void setLevel(squareState **currentMap, squareState **levelMap,
                playerPosition *currentPos, playerPosition *initPos,
                int rows, int cols) {
  initMap(currentMap, levelMap, rows, cols);
  currentPos->r = initPos->r;
  currentPos->c = initPos->c;
}

leaderBoard *allocateLeaderBoard(void) {
  leaderBoard *ptr = malloc(sizeof(leaderBoard));
  if (ptr == NULL) {
    perror("Unable to allocate space for new leader board node");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

move getMove(squareState **currentMap, playerPosition *currentPos,
             int rows, int cols) {
  if (event.type == SDL_KEYDOWN && event.key.keysym.sym ==
                                   SDLK_UP) {
    return UP;
  }

  if (event.type == SDL_KEYDOWN && event.key.keysym.sym ==
                                   SDLK_DOWN) {
    return DOWN;
  }

  if (event.type == SDL_KEYDOWN && event.key.keysym.sym ==
                                   SDLK_LEFT) {
    return LEFT;
  }

  if (event.type == SDL_KEYDOWN && event.key.keysym.sym ==
                                   SDLK_RIGHT) {
    return RIGHT;
  }
  return NO_MOVE;
}

_Bool checkValidMove(squareState **currentMap, playerPosition *currentPos,
                     int rows, int cols, move mov) {
  switch (mov) {
    case UP:
      return (currentPos->r > 0) &&
          (currentMap[(currentPos->r) - 1][currentPos->c] == UNSTEPPED ||
              currentMap[(currentPos->r) - 1][currentPos->c] == END_POSITION);
    case DOWN:
      return (currentPos->r < (rows - 1)) &&
          (currentMap[(currentPos->r) + 1][currentPos->c] == UNSTEPPED ||
              currentMap[(currentPos->r) + 1][currentPos->c] == END_POSITION);
    case LEFT:
      return (currentPos->c > 0) &&
          (currentMap[currentPos->r][(currentPos->c) - 1] == UNSTEPPED ||
              currentMap[currentPos->r][(currentPos->c) - 1] == END_POSITION);
    case RIGHT:
      return (currentPos->c < (cols - 1)) &&
          (currentMap[currentPos->r][(currentPos->c) + 1] == UNSTEPPED ||
              currentMap[currentPos->r][(currentPos->c) + 1] == END_POSITION);
    default:
      return 1;
  }
}

int movePlayer(squareState **currentMap, playerPosition *currentPos,
                int rows, int cols) {
  move mov = getMove(currentMap, currentPos, rows, cols);
  if (checkValidMove(currentMap, currentPos, rows, cols, mov)) {
    int rCurr = currentPos->r;
    int cCurr = currentPos->c;
    switch (mov) {
      case UP:
        currentMap[rCurr][cCurr] = STEPPED;
        currentMap[rCurr - 1][cCurr] = PLAYER;
        currentPos->r--;
        return 1;
      case DOWN:
        currentMap[rCurr][cCurr] = STEPPED;
        currentMap[rCurr + 1][cCurr] = PLAYER;
        currentPos->r++;
        return 1;
      case LEFT:
        currentMap[rCurr][cCurr] = STEPPED;
        currentMap[rCurr][cCurr - 1] = PLAYER;
        currentPos->c--;
        return 1;
      case RIGHT:
        currentMap[rCurr][cCurr] = STEPPED;
        currentMap[rCurr][cCurr + 1] = PLAYER;
        currentPos->c++;
        return 1;
      default:
        break;
    }
  }
  return 0;
}

void printLevel(squareState **map, int rows, int cols) {
  for (int r = 0; r < rows; ++r) {
    for (int c = 0; c < cols; ++c) {
      switch (map[r][c]) {
        case BLACK_SPACE:
          printf("B ");
          break;
        case PLAYER:
          printf("P ");
          break;
        case STEPPED:
          printf("S ");
          break;
        case UNSTEPPED:
          printf("U ");
          break;
        default:
          printf("E ");
          break;
      }
    }
    printf("\n");
  }
}

static int getBit(uint8_t n, int bitidx) {
  return ((n & (1 << bitidx)) >> bitidx);
}

static void getInput() {
  uint8_t controller = 0;
  char str[10];
  printf("Controller: \n");
  scanf("%s", str);
  controller = ~((uint8_t) strtol(str, NULL, 0));
  nesController.stateA = getBit(controller, 0);
  nesController.stateB = getBit(controller, 1);
  nesController.stateSelect = getBit(controller, 2);
  nesController.stateStart = getBit(controller, 3);
  nesController.stateUp = getBit(controller, 4);
  nesController.stateDown = getBit(controller, 5);
  nesController.stateLeft = getBit(controller, 6);
  nesController.stateRight = getBit(controller, 7);
}

void printSol(playerPosition **path, int nMoves) {
  printf("Printing path\n");
  for (int i = 0; i < nMoves; i++) {
    printf("(%u, %u)\n", path[i]->c, path[i]->r);
  }
}


//TODO
leaderBoard *updateLeaderBoard(leaderBoard *head, uint32_t time) {
  if (head->next->next->time <= time) {
    return head;
  }
  free(head->next->next);
  head->next->next = NULL;
  leaderBoard *newEntry = allocateLeaderBoard();
  newEntry->time = time;
  if (head->time > time) {
    newEntry->next = head;
    return newEntry;
  } else if (head->next->time > time) {
    newEntry->next = head->next;
    head->next = newEntry;
  } else {
    newEntry->next = head->next->next;
    head->next->next = newEntry;
  }
  return head;
}

//TODO
void resetLeaderBoard(leaderBoard *head) {
  leaderBoard *current = head;
  while (current != NULL) {
    current->time = (uint32_t) -1;
    current = current->next;
  }
}

leaderBoard *getLeaderBoardElem(leaderBoard *head, int pos) {
    assert(pos >= 1 && pos <= 3);
    if (pos == 1) {
        return head;
    } else if (pos == 2) {
        return head->next;
    } else {
        return head->next->next;
    }
}

void freeLeaderBoard(leaderBoard *head) {
    free(head->next->next);
    free(head->next);
    free(head);
}



