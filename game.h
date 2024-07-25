//
// Created by m1396 on 2021/8/26.
//

#ifndef MAZE_GAME_H
#define MAZE_GAME_H

#include "map.h"
#include "player.h"

#include <windows.h>

enum MazeMoveEnum {
    move_up = 72,
    move_down = 80,
    move_left = 75,
    move_right = 77,
};

/*
 * Game game = Game_new();
 * Game_start(game, 3, 4); // 开始游戏，地图为3行4列
 * Game_start(game, 5, 6); // 开始新游戏，地图为5行6列
 * Game_free(game);
 */
typedef struct game_t {
    MazeMap map;
    Player player;
    int _is_init_start;
    int is_move;
} *Game;

Game Game_new();

#define Game_start(game, row, column) Game_start_seed(game, row, column, 0)
int Game_start_seed(Game game, maze_size_t row, maze_size_t column, DWORD seed);
void Game_free(Game game);

int Game_is_win(Game game);

/*
 * int step = Game_get_moves(game, move_up, NULL);
 * Point *list = (Point *) malloc(sizeof(Point) * (step + 1));
 * Game_get_moves(game, move_up, list);
 *
 * 通过 list 返回移动列表
 * list 传递 NULL 则只返回步数，需要的列表长度为 步数+1
 * 返回 0 代表不可移动
 * 注意该函数并不会真正地移动
 */
int Game_get_moves(Game game, enum MazeMoveEnum move, Point *list);

int Game_move(Game game, enum MazeMoveEnum move, Point *list);

#endif //MAZE_GAME_H
