//
// Created by m1396 on 2021/8/26.
//

#include "game.h"
#include "stack.h"

#include <stdlib.h>
#include <string.h>
#include <windows.h>

Game Game_new() {
    Game game = (Game) malloc(sizeof(struct game_t));
    if (game == NULL) return NULL;
    game->_is_init_start = 0;
    return game;
}

int Game_start_seed(Game game, maze_size_t row, maze_size_t column, DWORD seed) {
    if (seed == 0) {
        seed = GetTickCount();
    }
    if (game->_is_init_start) {
        MazeMap_free(game->map);
        Player_free(&game->player);
    }
    if (!MazeMap_init_seed(seed, &game->map, row, column)) return 0;
    if (!Player_init(&game->player, game->map.st, NULL)) return 0;
    game->is_move = 0;
    game->_is_init_start = 1;
    return 1;
}

void Game_free(Game game) {
    MazeMap_free(game->map);
    Player_free(&game->player);
    free(game);
}

int Game_is_win(Game game) {
    return Point_equal(game->player.pos, game->map.ed);
}

#define is_overrange(game, p) ((p).p0 < 0 || (p).p1 < 0) || ((p).p0 >= (game)->map.row || (p).p1 >= (game)->map.column)

/*
 * 该点附近存在岔路或者无路可走，返回 {-1, -1}
 * 有且只有一条路，返回下一个点的坐标
 */
Point gamemove_find_road(Game game, Point p, Point lp) {
    static Point temp_p[4];
    temp_p[0] = Point_get(p.p0 - 1, p.p1);
    temp_p[1] = Point_get(p.p0 + 1, p.p1);
    temp_p[2] = Point_get(p.p0, p.p1 - 1);
    temp_p[3] = Point_get(p.p0, p.p1 + 1);

    Point res = {-1, -1};
    for (int i = 0; i < 4; ++i) {
        if (is_overrange(game, temp_p[i])) continue;
        if (Point_equal(temp_p[i], lp)) continue;
        if (MazeMap_getindex_p(game->map, temp_p[i]) != MAP_ROAD) continue;
        if (!Point_equal_d(res, -1, -1)) return Point_get(-1, -1);
        res = temp_p[i];
    }
    return res;
}

int Game_get_moves(Game game, enum MazeMoveEnum move, Point *list) {
    Point p = game->player.pos;
    Point lp = game->player.pos;
    if (move == move_up) --p.p0;
    else if (move == move_down) ++p.p0;
    else if (move == move_left) --p.p1;
    else if (move == move_right) ++p.p1;
    if (MazeMap_getindex_p(game->map, p) != MAP_ROAD) return 0;

    if (list) {
        list[0] = lp;
        list[1] = p;
    }
    int step = 1;
    Point next_road = gamemove_find_road(game, p, lp);
    while (!Point_equal_d(next_road, -1, -1) && !Point_equal(p, game->map.ed)) {
        ++step;
        lp = p;
        p = next_road;
        if (list) {
            list[step] = p;
        }
        next_road = gamemove_find_road(game, p, lp);
    }
    return step;
}

int Game_move(Game game, enum MazeMoveEnum move, Point *list) {
    int step = Game_get_moves(game, move, list);
    if (step == 0) return 0;
    if (list == NULL) {
        list = (Point *) malloc(sizeof(Point) * (step + 1));
        if (list == NULL) return 0;
        Game_get_moves(game, move, list);
    }
    game->player.pos = list[step];
    game->is_move = 1;
    game->player.step += step;
    game->player.move_times += 1;
    return step;
}
