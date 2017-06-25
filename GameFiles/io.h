//
// Created by nsn116 on 13/06/17.
//

#ifndef ARM11_31_IO_H
#define ARM11_31_IO_H

void writeMap(char fileName[], squareState **map, int rows, int cols);
squareState **readMap(char fileName[], int *rows, int *cols);
void writeSolution(char fileName[], playerPosition *solArray[], int size);
playerPosition **readSolution(char fileName[]);
void readLeaderBoard(char fileName[], leaderBoard *head);
void writeLeaderBoard(char fileName[], leaderBoard *head);
#endif //ARM11_31_IO_H
