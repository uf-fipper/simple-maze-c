//
// Created by m1396 on 2021/8/24.
//

#ifndef MAZE_RANDOM_H
#define MAZE_RANDOM_H

#include <windows.h>

#include "mazedef.h"

typedef struct random_t {
    DWORD seed;
    unsigned long long state;
} *Random;

Random Random_new();
Random Random_new_seed(DWORD seed);
void Random_free(Random r);
int Random_nextint(Random r, maze_size_t a, maze_size_t b);
int Random_randindex(Random r, maze_size_t n, maze_size_t *arr);

#endif //MAZE_RANDOM_H
