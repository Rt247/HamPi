#include "randomGen.h"
#include "string.h"
#include "game.h"

/**
 * Counts the number of unstepped tiles in the map, ie. the remaining tiles
 * to be passed through by the player, including the end tile. Allows to
 * calculate path length by calling it on the original map
 * @param map
 * @param rows
 * @param cols
 * @return
 */
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

/**
 * Returns the horizontal displacement given a move enum
 * @param mov The mov enum you want to evaluate
 * @return the horizontal displacement: 1, 0 or -1
 */
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

/**
 * Returns the vertical displacement given a move enum
 * @param mov The mov enum you want to evaluate
 * @return the vertical displacement: 1, 0 or -1
 */
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

/**
 * Determines if the path length is acceptable for the number of attempts to
 * generate the path with respect of the target length according to
 * heuristics which make the path generation not time out
 * @param pathLength The length of the path generated
 * @param targetLength The target length for the path
 * @param tries Number of failed attempts at generate a path of sufficient
 * length
 * @return 1 if the path is long enough, otherwise 0
 */
int isPathLongEnough(int pathLength, int targetLength, int tries) {
  int scalingConst = 3;
  int _100Percent = 100;
  return (pathLength > ((_100Percent - (scalingConst * tries))) *
                       targetLength  / _100Percent);
}

/**
 * PRE: moves has space to store 4 moves
 * Fills in the given moves array with a sequence of UP DOWN LEFT RIGHT. This
 * is determined by the first direction which is generated, that is:
 * UDLR, DLRU, LRDU and RDLU
 * @param moves A pointer to the array where the move sequence is to be stored
 */
void getTryMoveSequence(move *moves) {
  //Random move sequence mechanics. Generates the sequence through modular and
  //normal arithmetic
  int randNum = rand() % 4;
  moves[0] = (move) randNum;
  moves[1] = moves[0] % 3 + 1;
  moves[2] = moves[1] % 3 + 1;
  moves[3] = (move) (6 - (moves[0] + moves[1] + moves[2]));
}

/**
 * Backtracking algorithm which proceeds along the path, randomly generating
 * the order of the directions it is going to try for a given move. It tries to
 * move in each direction and if it can, it proceeds recursively until it
 * reaches the predetermined path length. If it can’t move any further and the
 * path is too short, it backtracks and tries to move in another direction.
 * To prevent a timeout, the recursive function counts the number of
 * backtracks in the current path, reducing the minimum path length needed to
 * exit the function as the number of failed attempts increases.
 * @param pathLength The length of the path up until the last call
 * @param targetLen The desired path length
 * @param rows Number of rows in the map
 * @param cols Number of culumns in the map
 * @param currPos Struct holding the current position in the path
 * @param solArray The array where you write the sequence of steps in the path
 * as playerPosition struct pointers
 * @param end Pointer to the struct which stores the the end position of the
 * player in the map
 * @param map pointer to the 2D map of the level, on which the path is being
 * traced
 * @param tries The number of backtracks in the current path
 * @return An integer representing the success or failure of the path generation
 * for the function call
 */
int generatePath(int pathLength, int targetLen, int rows, int cols,
                 playerPosition *currPos, playerPosition **solArray,
                 playerPosition *end, squareState **map, int tries) {
  //If the path has the desired length then end with success
  if (pathLength == targetLen) {
    //Set the end position to the current position and the end tile to the
    //END_POSITION enum
    end->c = currPos->c;
    end->r = currPos->r;
    map[currPos->r][currPos->c] = END_POSITION;
    return 1;
  }

  //Otherwise initialise a temporary position which will change throughout the
  //loop iterations
  playerPosition *pos = initPosition(0, 0);
  //Declare an array to store the move sequence that will be tested
  move tryMoves[4];
  //Write the move sequence
  getTryMoveSequence(tryMoves);
  for (int i = 0; i < 4; ++i) {
    //Assign pos to a displacement of the current position based on the
    //direction you are trying to move in
    pos->r = currPos->r + getdy(tryMoves[i]);
    pos->c = currPos->c + getdx(tryMoves[i]);
    if (pos->r < 0 || pos->r >= rows || pos->c < 0 || pos->c >= cols
        || map[pos->r][pos->c] != BLACK_SPACE) {
      //If the new position is invalid skip it
      continue;
    }
    //Update solution array
    solArray[pathLength] = pos;
    //Update map
    map[pos->r][pos->c] = UNSTEPPED;
    //Recursively proceed along the path
    if (!generatePath(pathLength + 1, targetLen, rows, cols, pos, solArray,
    end, map, tries)) {
      //If a map with a path of sufficient couldn't be generated then reset the
      //state of the map and solution array and record the failed attempt
      map[pos->r][pos->c] = BLACK_SPACE;
      solArray[pathLength] = NULL;
      tries++;
    } else {
      //Path generation was successful
      return 1;
    }
  }
  //Couldn't move in any direction, so free the new position
  free(pos);
  if (isPathLongEnough(pathLength, targetLen, tries)) {
    //if the path length taking into account the number of tries is enough,
    //then set the end position to the current position and the end tile to the
    //END_POSITION enum and return with success
    map[currPos->r][currPos->c] = END_POSITION;
    end->r = currPos->r;
    end->c = currPos->c;
    return 1;
  }
  //Otherwise return with failure
  return 0;
}

/**
 * PRE: CALL srand(time(NULL))
 * Generate a map with a random path length equal to at least half the number
 * of squares.
 * @param rows Number of rows in the map
 * @param cols Number of culumns in the map
 * @param position Pointer to the struct which stores the the intial position of
 * the player in the map
 * @param solArray The array of positions in the path which is filled up by the
 * helper method which generates the path
 * @param endPos Pointer to the struct which stores the the end position of the
 * player in the map
 * @return A pointer to the 2D array storing the map of the level, with the
 * player set in its starting position, the end position labeled and all the
 * intermediate tiles set to unstepped
 */
squareState **generate(int rows, int cols, playerPosition *position,
                       playerPosition **solArray, playerPosition *endPos) {
  //Calculate total number of squares
  int numSquares = rows * cols;
  //Calculate path length
  int pathLength = rand() % (numSquares / 2) + (numSquares / 2);
  //Allocate space for map
  squareState **map = getMapPtr(rows, cols);
  //Calculate initial position and update pos struct
  position->c = rand() % (cols);
  position->r = rand() % (rows);
  //Set player initial position
  map[position->r][position->c] = PLAYER;
  //Call helper method to fill in the path
  generatePath(0, pathLength, rows, cols, position, solArray, endPos, map, 0);
  return map;
}

