//
// Created by m1396 on 2021/8/24.
//

#ifndef MAZE_MAP_H
#define MAZE_MAP_H

#include "mazedef.h"
#include "random.h"
#include "point.h"
#include "matrix.h"

// 请保留 0-127
#define MAP_EMPTY 0
#define MAP_WALL 1
#define MAP_ROAD 2

/*
 * 地图类
 * row: 多少行
 * column: 多少列
 * st: 起点坐标
 * ed: 终点坐标
 *
 * _random: 随机结构
 * _init_st: 开始生成地图的点
 * _map: 地图，_map[row][column], 但是使用一维列表表示
 * solve_step: 解步骤
 * solve_list: 解列表，列表长度 = (解步骤 + 1)（包括起点和终点）
 */
typedef struct {
    maze_size_t row;
    maze_size_t column;

    Point st;
    Point ed;

    Random _random;
    Point _init_st;

    Matrix _map;
    maze_size_t solve_step;
    Point *solve_list;
} MazeMap;

/*
 * MazeMap map;
 * MazeMap_init(&map, row, column);
 * ...
 * MazeMap_free(&map);
 */
int MazeMap_init(MazeMap *map, maze_size_t row, maze_size_t column);
int MazeMap_init_seed(DWORD seed, MazeMap *map, maze_size_t row, maze_size_t column);
void MazeMap_free(MazeMap map);

/*
 * int length = MazeMap_to_str(map, NULL); // 当 res 为 NULL 时返回需要的字符串长度
 * char *str = (char *) malloc(sizeof(char) * length);
 * MazeMap_to_str(map, str); // 依然返回需要的字符串长度
 * MazeMap_to_str_solve(map, str, 'A'); // 打印解路径，字符为 'A'
 */
#define MazeMap_to_str(map, res) MazeMap_to_str_solve(map, res, ' ')
#define MazeMap_to_str_solve(map, res, solve) MazeMap_to_str_ex(map, res, 'X', 'X',  ' ', 'S', 'E', solve)
int MazeMap_to_str_ex(MazeMap map, char *res, char border, char wall, char road, char st, char ed, char solve);

/*
 * 打印，返回打印的长度
 */
#define MazeMap_print(map) MazeMap_print_solve(map, ' ')
#define MazeMap_print_solve(map, solve) MazeMap_print_ex(map, 'X', 'X',  ' ', 'S', 'E', MazeMap_solve)
int MazeMap_print_ex(MazeMap map, char _border, char _wall, char _road, char _st, char _ed, char _solve);

/*
 * 求解迷宫
 * solve(&map, NULL); // 从起点开始求解，可以使用字符串方式查看
 * MazeMap_solve(&map, st); // 从 st 开始求解
 */
int MazeMap_solve(MazeMap *map, Point st);

/*
 * 请勿直接使用 map._map[i][j] 获取对象
 * 请使用 MazeMap_getindex(map, i, j) 获取对象
 * //或使用 MazeMap_setindex(map, i, j, v) 设置对象
 * MazeMap_getindex_p(map, p): p 是 Point
 */
#define MazeMap_getindex_p(map, p) MazeMap_getindex(map, (p).p0, (p).p1)
maze_value_t MazeMap_getindex(MazeMap map, maze_size_t i, maze_size_t j);
//#define MazeMap_setindex_p(map, p, v) MazeMap_setindex(map, (p).p0, (p).p1, v)
//void MazeMap_setindex(MazeMap map, maze_size_t i, maze_size_t j, maze_value_t v);

#endif //MAZE_MAP_H
