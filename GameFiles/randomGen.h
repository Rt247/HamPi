#ifndef ARM11_31_RANDOMGEN_H
#define ARM11_31_RANDOMGEN_H

#include <stdlib.h>
#include "gameUtils.h"

int countUnstepped(squareState **map, int rows, int cols);

squareState **generate(int rows, int cols, playerPosition *position,
                       playerPosition **solArray, playerPosition *endPos);


#endif //ARM11_31_RANDOMGEN_H
