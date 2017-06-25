#ifndef ARM11_31_NES_H
#define ARM11_31_NES_H

#define DATA 18
#define LATCH 23
#define CLOCK 24

struct controllerState{
  int stateA;
  int stateB;
  int stateSelect;
  int stateStart;
  int stateUp;
  int stateDown;
  int stateLeft;
  int stateRight;
} nesController;

typedef struct controllerState controllerState;
void nesInitialise();
void inputStage();

#endif //ARM11_31_NES_H_H
