#include <string.h>
#include "gameUtils.h"
#include "io.h"

#define MAX_ROW_COL_DIGITS 2
#define BASE10 10

/**
 * Returns a heap allocated string representation of the map
 * @param map the map whose string representation is to be returned
 * @param rows the number of rows in the map
 * @param cols the number of columns in the map
 * @return pointer to the heap allocated string representation of the map
 */
static char **toString(squareState **map, int rows, int cols) {
  char **mapAsString = (char **) malloc((size_t)((rows + 2) * sizeof(char *)));
  mapAsString[0] = (char *) calloc(MAX_ROW_COL_DIGITS + 1, sizeof(char));
  mapAsString[1] = (char *) calloc(MAX_ROW_COL_DIGITS + 1, sizeof(char));
  sprintf(mapAsString[0], "%d", rows);
  sprintf(mapAsString[1], "%d", cols);
  for (int k = 2; k < rows + 2; ++k) {
    mapAsString[k] = (char *) calloc((size_t)(2 * cols), sizeof(char));
  }
  char digitAsString[2];
  char space[2] = " ";
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      sprintf(digitAsString, "%d", map[i][j]);
      strcat(mapAsString[i + 2], digitAsString);
      strcat(mapAsString[i + 2], space);
    }
  }
  return mapAsString;
}

/**
 * Writes the given map into the file whose filename is passed as a parameter
 * @param fileName the name of the file to be written into
 * @param map the map to be written into the file
 * @param rows the number of rows in the map
 * @param cols the number of columns in the map
 */
void writeMap(char fileName[], squareState **map, int rows, int cols) {
  char **mapAsString = toString(map, rows, cols);
  FILE *mapFilePtr = fopen(fileName, "w");
  if (mapFilePtr == NULL) {
    perror("Could not open the file to write the map in!");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < rows + 2; ++i) {
    fprintf(mapFilePtr, "%s\n", mapAsString[i]);
  }
  int closeSuccessful = fclose(mapFilePtr);
  if (closeSuccessful == -1) {
    perror("Could not close the file in which the map was written!");
    exit(EXIT_FAILURE);
  }
  free(mapAsString[0]);
  free(mapAsString[1]);
  free(mapAsString);
}

/**
 * Reads a map from the file whose filename is passed as a parameter
 * @param fileName the name of the file to be read from
 * @param rows the number of rows in the map
 * @param cols the number of columns in the map
 * @return a pointer to a heap allocated map of squareStates
 */
squareState **readMap(char fileName[], int *rows, int *cols) {
  FILE *mapFilePtr = fopen(fileName, "r");
  if (mapFilePtr == NULL) {
    perror("Could not open the file to write the map in!");
    exit(EXIT_FAILURE);
  }
  fscanf(mapFilePtr, "%d", rows);
  fscanf(mapFilePtr, "%d", cols);
  squareState **map = getMapPtr(*rows, *cols);
  char buffer[2 * (*cols)];
  char *rest;
  for (int i = 0; i < *rows; ++i) {
    fgets(buffer, 2 * (*cols), mapFilePtr);
    //The fgets above reads a newline character, so you need another read to
    //actually start reading the map.
    fgets(buffer, 2 * (*cols), mapFilePtr);
    //strtol() ignores leading whitespaces when parsing a number, so no need to
    //tokenize
    map[i][0] = (squareState) strtol(buffer, &rest, BASE10);
    for (int j = 1; j < *cols; ++j) {
      map[i][j] = (squareState) strtol(rest, &rest, BASE10);
    }
  }
  int closeSuccessful = fclose(mapFilePtr);
  if (closeSuccessful == -1) {
    perror("Could not close the file in which the map was written!");
    exit(EXIT_FAILURE);
  }
  return map;
}

/**
 * Returns the size of the binary file passed as input
 * @param binFile file pointer to the binary file whose size is to be computed
 * @return the size of the file
 */
static int getFileSize(FILE *binFile) {
  fseek(binFile, 0L, SEEK_END);
  int size = (int) ftell(binFile);
  fseek(binFile, 0L, SEEK_SET);
  return size;
}

/**
 * Writes the solution of the level to be saved, to a binary file
 * @param fileName the name of the file to be written into
 * @param solArray the solution of the given map represented as player
 * coordinates
 * @param size the number of entries in the solution array
 */
void writeSolution(char fileName[], playerPosition *solArray[], int size) {
  FILE *mapFilePtr = fopen(fileName, "wb");
  if (mapFilePtr == NULL) {
    perror("Could not open the file to write the map in!");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < size; ++i) {
    fwrite(solArray[i], sizeof(playerPosition), 1, mapFilePtr);
  }
  int closeSuccessful = fclose(mapFilePtr);
  if (closeSuccessful == -1) {
    perror("Could not close the file in which the map was written!");
    exit(EXIT_FAILURE);
  }
}

/**
 * Reads the solution to a saved level from a binary file
 * @param fileName the name of the file to be read from
 * @return a pointer to a heap allocated memory containing the solution of the
 * map that has been saved
 */
playerPosition **readSolution(char fileName[]) {
  FILE *mapFilePtr = fopen(fileName, "rb");
  if (mapFilePtr == NULL) {
    perror("Could not open the file to write the map in!");
    exit(EXIT_FAILURE);
  }
  int size = getFileSize(mapFilePtr) / sizeof(playerPosition);
  playerPosition **solArray
      = (playerPosition **) malloc(size * sizeof(playerPosition *));
  playerPosition tempPosition;
  for (int i = 0; i < size; ++i) {
    fread(&tempPosition, sizeof(playerPosition), 1, mapFilePtr);
    solArray[i] = initPosition(tempPosition.r, tempPosition.c);
  }
  int closeSuccessful = fclose(mapFilePtr);
  if (closeSuccessful == -1) {
    perror("Could not close the file in which the map was written!");
    exit(EXIT_FAILURE);
  }
  return solArray;
}

/**
 * Writes the linked list representing the leaderboard into the file identified
 * by the file name passed as a parameter
 * @param fileName the name of the file in which the leaderboard is
 * to be written into
 * @param head the head of the linked list representing the leaderboard
 */
void writeLeaderBoard(char *fileName, leaderBoard *head) {
  FILE *leaderboardFilePtr = fopen(fileName, "wb");
  if (leaderboardFilePtr == NULL) {
    perror("Could not open the file to write the leaderboard in!");
    exit(EXIT_FAILURE);
  }
  leaderBoard *current = head;
  while(current != NULL) {
    fwrite(&(current->time), sizeof(uint32_t), 1, leaderboardFilePtr);
    current = current->next;
  }
  int closeSuccessful = fclose(leaderboardFilePtr);
  if (closeSuccessful == -1) {
    perror("Could not close the file in which the leaderboard was written!");
    exit(EXIT_FAILURE);
  }
}

// PRE: leaderboard with three elements has been allocated, with head being the
// pointer to the first element.
/**
 * Reads the leaderboard from a file which is identified by the file name
 * passed as a parameter
 * @param fileName the name of the file in which the leaderboard is
 * to be read from
 * @param head the head of the linked list representing the leaderboard
 */
void readLeaderBoard(char *fileName, leaderBoard *head) {
  FILE *leaderboardFilePtr = fopen(fileName, "rb");
  if (leaderboardFilePtr == NULL) {
    perror("Could not open the file to read the leaderboard from!");
    exit(EXIT_FAILURE);
  }
  int numRecords = getFileSize(leaderboardFilePtr) / sizeof(uint32_t);
  int count = 0;
  for (leaderBoard *curr = head;
       curr != NULL && count < numRecords; curr = curr->next) {
    fread(&(curr->time), sizeof(uint32_t), 1, leaderboardFilePtr);
    count++;
  }
  int closeSuccessful = fclose(leaderboardFilePtr);
  if (closeSuccessful == -1) {
    perror("Could not close the file from which the leaderboard was read!");
    exit(EXIT_FAILURE);
  }
}
