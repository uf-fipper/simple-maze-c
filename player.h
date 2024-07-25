//
// Created by m1396 on 2021/8/26.
//

#ifndef MAZE_PLAYER_H
#define MAZE_PLAYER_H

#include "map.h"
#include "point.h"

typedef struct {
    char name[16];
    Point pos;
    maze_size_t step;
    maze_size_t move_times;
} Player;

int Player_init(Player *player, Point pos, const char *name);
void Player_free(Player *player);

#endif //MAZE_PLAYER_H
