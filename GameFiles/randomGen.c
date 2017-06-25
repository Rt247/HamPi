#include "randomGen.h"
#include "string.h"
#include "game.h"

#define IS_PATH_LONG_ENOUGH(length, target, tries) (length > ((100 - (3 * tries))) * target  / 100)


int countUnstepped(squareState **map, int rows, int cols) {
  int count = 0;
  for (int i = 0; i < cols; ++i) {
    for (int j = 0; j < rows; ++j) {
      if (map[j][i] == UNSTEPPED || map[j][i] == END_POSITION) {
        ++count;
      }
    }
  }
  return count;
}

int getdx(move mov) {
  switch (mov) {
    case LEFT:
      return -1;
    case RIGHT:
      return 1;
    default:
      return 0;
  }
}

int getdy(move mov) {
  switch (mov) {
    case UP:
      return -1;
    case DOWN:
      return 1;
    default:
      return 0;
  }
}

void getTryMoveSequence(move *moves) {
  //Pre: moves has space to store 4 moves
  int randNum = rand() % 4;
  moves[0] = (move) randNum;
  moves[1] = moves[0] % 3 + 1;
  moves[2] = moves[1] % 3 + 1;
  moves[3] = (move) (6 - (moves[0] + moves[1] + moves[2]));
}

int generatePath(int pathLength, int targetLen, int rows, int cols,
                 playerPosition *currPos, playerPosition **solArray,
                 playerPosition *end, squareState **map, int tries) {
  if (pathLength == targetLen) {
    end->c = currPos->c;
    end->r = currPos->r;
    map[currPos->r][currPos->c] = END_POSITION;
    return 1;
  }
  playerPosition *pos = initPosition(0, 0);
  move tryMoves[4];
  getTryMoveSequence(tryMoves);
  for (int i = 0; i < 4; ++i) {
    pos->r = currPos->r + getdy(tryMoves[i]);
    pos->c = currPos->c + getdx(tryMoves[i]);
    if (pos->r < 0 || pos->r >= rows || pos->c < 0 || pos->c >= cols
        || map[pos->r][pos->c] != BLACK_SPACE) {
      continue;
    }
    solArray[pathLength] = pos;
    map[pos->r][pos->c] = UNSTEPPED;
    if (!generatePath(pathLength + 1, targetLen, rows, cols, pos, solArray,
    end, map, tries)) {
      map[pos->r][pos->c] = BLACK_SPACE;
      solArray[pathLength] = NULL;
      tries++;
    } else {
      return 1;
    }
  }
  free(pos);
  if (IS_PATH_LONG_ENOUGH(pathLength, targetLen, tries)) {
    map[currPos->r][currPos->c] = END_POSITION;
    end->r = currPos->r;
    end->c = currPos->c;
    return 1;
  }
  return 0;
}

squareState **generate_internal(int pathLength, int rows, int cols,
                                playerPosition *pos, playerPosition **solArray,
                                playerPosition *end) {
  squareState **map = getMapPtr(rows, cols);
  pos->c = rand() % (cols);
  pos->r = rand() % (rows);
  map[pos->r][pos->c] = PLAYER;
  generatePath(0, pathLength, rows, cols, pos, solArray, end, map, 0);
    printLevel(map, rows, cols);
  return map;
}

/*
 * Attempts to randomly generate a map with a random path length equal to
 * at least half the number of squares.
 */
// PRE: CALL srand(time(NULL))
squareState **generate(int rows, int cols, playerPosition *position,
                       playerPosition **solArray, playerPosition *endPos) {
  int numSquares = rows * cols;
  int pathLength = rand() % (numSquares / 2) + (numSquares / 2);

  return generate_internal(pathLength, rows, cols, position, solArray, endPos);
}

