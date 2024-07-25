//
// Created by m1396 on 2021/8/26.
//

#include "player.h"

#include <string.h>

int Player_init(Player *player, Point pos, const char *name) {
    if (player == NULL) return 0;
    if (name != NULL) {
        strncpy(player->name, name, 15);
        player->name[15] = '\0';
    }
    player->pos = pos;
    player->step = player->move_times = 0;
    return 1;
}

void Player_free(Player *player) {}
