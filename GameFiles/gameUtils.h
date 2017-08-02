#ifndef ARM11_31_GAMEUTILS_H
#define ARM11_31_GAMEUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "nes.h"

#define MAX(A, B) A < B ? B : A
#define MIN(A, B) A < B ? A : B

typedef enum squareState {
  BLACK_SPACE, UNSTEPPED, STEPPED, PLAYER, END_POSITION, RAND_OBSTACLE
} squareState;


typedef struct playerPosition {
  int r;
  int c;
} playerPosition;

typedef enum move {
  UP, DOWN, LEFT, RIGHT, NO_MOVE
} move;

typedef struct leaderBoard {
  uint32_t time;
  struct leaderBoard *next;
}leaderBoard;

extern squareState r1[];
extern squareState r2[];
extern squareState r3[];
extern squareState *level1[];

extern playerPosition initPosLevel1;

squareState **getMapPtr(int rows, int cols);

playerPosition *initPosition(int r, int c);

void setLevel(squareState **currentMap, squareState **levelMap,
              playerPosition *currentPos, playerPosition *initPos,
              int rows, int cols);

void freeMap(squareState **mapPtr);

move getMove();
//move getMove(squareState **currentMap, playerPosition *currentPos,
//                  int rows, int cols);

_Bool checkValidMove(squareState **currentMap, playerPosition *currentPos,
                     int rows, int cols, move mov);

leaderBoard *allocateLeaderBoard(void);

int movePlayer(squareState **currentMap, playerPosition *currentPos,
                int rows, int cols);

void printLevel(squareState **map, int rows, int cols);

void freeSolArr(playerPosition **path, int nMoves);

void printSol(playerPosition **path, int nMoves);

leaderBoard *updateLeaderBoard(leaderBoard *head, uint32_t time);

leaderBoard *getLeaderBoardElem(leaderBoard *head, int pos);

void resetLeaderBoard(leaderBoard *head);

leaderBoard *updateLeaderboard(leaderBoard *head, uint32_t time);

void freeLeaderBoard(leaderBoard *head);

#endif //ARM11_31_GAMEUTILS_H
