//
// Created by m1396 on 2021/8/24.
//

#include "random.h"
#include <stdlib.h>
#include <limits.h>
#include <windows.h>

unsigned long long _update_state(DWORD seed, unsigned long long state) {
    unsigned long long res = state % UINT_MAX;
    while (res < (UINT_MAX << 8)) {
        res = res * res + 1;
        res = res + (res % (seed + 0x489851123)) + (res + seed) % (seed + 0x894651231);
    }
    return res;
}

Random Random_new() {
    DWORD t = GetTickCount();
    return Random_new_seed(t);
}

Random Random_new_seed(DWORD seed) {
    if (seed == 0) seed = GetTickCount();
    Random r = (Random) malloc(sizeof(struct random_t));
    const int state_first = 0x12345678;
    r->seed = seed;
    r->state = _update_state(seed, state_first);
    return r;
}

void Random_free(Random r) {
    free(r);
}

int Random_nextint(Random r, maze_size_t a, maze_size_t b) {
    if (a >= b) return a;
    int res = (int) r->state % (b - a) + a;
    if (res < a) res += (b - a);
    else if (res >= b) res -= (b - a);
    r->state = _update_state(r->seed, r->state);
    return res;
}

int Random_randindex(Random r, maze_size_t n, maze_size_t *arr) {
    if (n <= 0) return 0;
    for (maze_size_t i = 0; i < n; ++i) arr[i] = -1;
    for (maze_size_t i = 0; i < n; ++i) {
        while (1) {
            maze_size_t rn = (maze_size_t) Random_nextint(r, 0, n);
            if (arr[rn] == -1) {
                arr[rn] = i;
                break;
            }
        }
    }
    return n;
}
