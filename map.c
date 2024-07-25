//
// Created by m1396 on 2021/8/24.
//

#include "map.h"
#include "stack.h"
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

maze_size_t m_getindex(Matrix m, maze_size_t i, maze_size_t j) {
    maze_size_t res;
    Matrix_getindex(m, i, j, maze_size_t, &res);
    return res;
}

int m_setindex(Matrix m, maze_size_t i, maze_size_t j, maze_size_t d) {
    return Matrix_setindex(m, i, j, maze_size_t, &d);
}

Point p_getindex(Matrix m, maze_size_t i, maze_size_t j) {
    Point res;
    Matrix_getindex(m, i, j, Point, &res);
    return res;
}

int p_setindex(Matrix m, maze_size_t i, maze_size_t j, Point d) {
    return Matrix_setindex(m, i, j, Point, &d);
}

int push(Stack S, Point p, Point lp, maze_size_t step) {
    Stack_push(S, Point, &p);
    Stack_push(S, Point, &lp);
    Stack_push(S, maze_size_t, &step);
    return 1;
}

int pop(Stack S, Point *p, Point *lp, maze_size_t *step) {
    Stack_pop(S, maze_size_t, step);
    Stack_pop(S, Point, lp);
    Stack_pop(S, Point, p);
    return 1;
}

int add(Queue Q, Point p, Point lp, maze_size_t step) {
    Queue_add(Q, Point, &p);
    Queue_add(Q, Point, &lp);
    Queue_add(Q, maze_size_t, &step);
    return 1;
}

int leave(Queue Q, Point *p, Point *lp, maze_size_t *step) {
    Queue_leave(Q, Point, p);
    Queue_leave(Q, Point, lp);
    Queue_leave(Q, maze_size_t, step);
    return 1;
}

/*
 * 判断数组是否越界
 */
#define is_overrange(r, c, p) ((p).p0 < 0 || (p).p0 >= (r) || (p).p1 < 0 || (p).p1 >= (c))

#pragma region 地图初始化
/*
 * 使用深度优先生成地图
 */

/*
 * 获取一个点周围所有的墙
 */
size_t mapinit_get_walls(Matrix map, Point p, Point lp, Point *res) {
    static Point res_temp[4];
    if (res == NULL) return -1;

    res_temp[0] = Point_get(p.p0 + 1, p.p1);
    res_temp[1] = Point_get(p.p0 - 1, p.p1);
    res_temp[2] = Point_get(p.p0, p.p1 + 1);
    res_temp[3] = Point_get(p.p0, p.p1 - 1);

    int length = 0;
    for (int i = 0; i < 4; ++i) {
        Point t = res_temp[i];
        if (is_overrange(map.row, map.column, t)) continue;
        if (Point_equal(t, lp)) continue;

        res[length] = res_temp[i];
        ++length;
    }
    return length;
}

/*
 * 检查这个墙能否被生成道路
 * p: 这个墙
 * lp: 上一个墙
 */
int mapinit_check_wall(Matrix map, Point p, Point lp) {
    static Point temp[4];
    int length = mapinit_get_walls(map, p, lp, temp);
    for (int i = 0; i < length; ++i) {
        if (m_getindex(map, temp[i].p0, temp[i].p1) != MAP_WALL) {
            return 0;
        }
    }
    return 1;
}

/*
 * 打乱数组
 * length: 数组长度
 */
int mapinit_rand_walls(Random r, maze_size_t length, Point *around_walls) {
    static Point temp_walls[4];
    static maze_size_t rand_index[4];

    int res = Random_randindex(r, length, rand_index);
    for (int i = 0; i < length; ++i) {
        temp_walls[i] = around_walls[rand_index[i]];
    }
    for (int i = 0; i < length; ++i) {
        around_walls[i] = temp_walls[i];
    }
    return res;
}

int mapinit_init_map(Matrix map,
                     Random random,
                     Point *inst_st,
                     Point *st,
                     Point *ed) {
    for (size_t i = 0; i < map.row; ++i) {
        for (size_t j = 0; j < map.column; ++j) {
            maze_value_t wall = MAP_WALL;
            Matrix_setindex(map, i, j, maze_value_t, &wall);
        }
    }
    inst_st->p0 = Random_nextint(random, 0, map.row);
    inst_st->p1 = Random_nextint(random, 0, map.column);
    Stack S = Stack_new_size((sizeof(Point) * 2 + sizeof(maze_size_t)) * 16);

    Point p = *inst_st;
    Point lp = {-1, -1};
    maze_size_t step = 0;
    push(S, p, lp, step);
    while (!Stack_isempty(S)) {
        pop(S, &p, &lp, &step);
        if (!mapinit_check_wall(map, p, lp)) {
            continue;
        }
        m_setindex(map, p.p0, p.p1, MAP_ROAD);
        static Point around_walls[4];
        size_t length = mapinit_get_walls(map, p, lp, around_walls);
        if (length == 0) continue;
        mapinit_rand_walls(random, length, around_walls);
        for (int i = 0; i < length; ++i) {
            push(S, around_walls[i], p, step + 1);
        }
    }
    int st_get = !st;
    int ed_get = !ed;
    for (int i = 0; i < map.row; ++i) {
        for (int j = 0; j < map.column; ++j) {
            if (st_get && ed_get) {
                i = map.row;
                j = map.column;
                continue;
            }
            if (!st_get && m_getindex(map, i, j) == MAP_ROAD) {
                st->p0 = i;
                st->p1 = j;
                st_get = 1;
            }
            if (!ed_get && m_getindex(map, map.row - 1 - i, map.column - 1 - j) == MAP_ROAD) {
                ed->p0 = map.row - 1 - i;
                ed->p1 = map.column - 1 - j;
                ed_get = 1;
            }
        }
    }
    Stack_free(S);
    return 1;
}

int MazeMap_init(MazeMap *map, maze_size_t row, maze_size_t column) {
    return MazeMap_init_seed(GetTickCount(), map, row, column);
}

int MazeMap_init_seed(DWORD seed, MazeMap *map, maze_size_t row, maze_size_t column) {
    Random r = Random_new_seed(seed);
    map->_random = r;
    map->row = row;
    map->column = column;
    map->solve_list = NULL;
    Matrix_init(&map->_map, row, column, maze_value_t);

    int res = mapinit_init_map(map->_map, r, &map->_init_st, &map->st, &map->ed);
    if (!res) return res;
    MazeMap_solve(map, map->st);
    return 1;
}

#pragma endregion

void MazeMap_free(MazeMap map) {
    Matrix_free(map._map);
    free(map.solve_list);
    Random_free(map._random);
}

#pragma region 解
/*
 * 使用广度优先求解
 */

/*
 * 获得一个点周围所有没遍历过的路
 */
int mapsolve_get_roads(MazeMap *map, Matrix map_temp, Point p, Point *res) {
    static Point res_temp[4];
    if (res == NULL) return -1;
    res_temp[0] = Point_get(p.p0 + 1, p.p1);
    res_temp[1] = Point_get(p.p0 - 1, p.p1);
    res_temp[2] = Point_get(p.p0, p.p1 + 1);
    res_temp[3] = Point_get(p.p0, p.p1 - 1);

    int length = 0;
    for (int i = 0; i < 4; ++i) {
        if (is_overrange(map->row, map->column, res_temp[i])) continue;
        Point idx = p_getindex(map_temp, res_temp[i].p0, res_temp[i].p1);
        if (!Point_equal_d(idx, -1, -1)) continue;
        if (MazeMap_getindex_p(*map, res_temp[i]) != MAP_ROAD) continue;
        else {
            res[length] = res_temp[i];
            ++length;
        }
    }
    return length;
}

int MazeMap_solve(MazeMap *map, Point st) {
    Queue Q = Queue_new_size((sizeof(Point) * 2 + sizeof(maze_size_t)) * 16);
    Matrix map_temp;
    Matrix_init(&map_temp, map->row, map->column, Point);
    for (int i = 0; i < map->row; i++) {
        for (int j = 0; j < map->column; j++) {
            p_setindex(map_temp, i, j, Point_get(-1, -1));
        }
    }

    Point p = st;
    if (Point_equal_d(p, -1, -1)) {
        p = map->st;
    }
    Point lp = {-2, -2};
    maze_size_t step = 0;
    add(Q, p, lp, step);
    while (!Point_equal(p, map->ed)) {
        leave(Q, &p, &lp, &step);
        p_setindex(map_temp, p.p0, p.p1, lp);
        Point roads[4];
        maze_size_t length = mapsolve_get_roads(map, map_temp, p, roads);
        for (int i = 0; i < length; ++i) {
            add(Q, roads[i], p, step + 1);
        }
    }
    Queue_free(Q);
    Point *res = (Point *) malloc(sizeof(Point) * (step + 1));
    if (res == NULL) {
        Matrix_free(map_temp);
        return 0;
    }
    Point rp = p_getindex(map_temp, map->ed.p0, map->ed.p1);
    res[step] = map->ed;
    for (int i = step - 1; i >= 0; --i) {
        res[i] = rp;
        rp = p_getindex(map_temp, rp.p0, rp.p1);
    }
    Matrix_free(map_temp);
    if (!Point_equal(res[0], st)) {
        free(res);
        return 0;
    }
    free(map->solve_list);
    map->solve_step = step;
    map->solve_list = res;

    return step;
}

#pragma endregion

maze_size_t MazeMap_to_str_ex(MazeMap map, char *res,
                              char _border, char _wall, char _road,
                              char _st, char _ed, char _solve) {
    if (res == NULL) return (map.row + 2) * (map.column + 3);

    Matrix temp_res;
    temp_res.data = (BYTE *) res;
//    temp_res.row = map.row + 2;
    temp_res.column = map.column + 3;
    temp_res.size = sizeof(char);

    const static char nl = '\n';
    const static char no = '\0';

    for (maze_size_t i = 0; i < map.column + 2; ++i) {
        Matrix_setindex(temp_res, 0, i, char, &_border);
    }
    Matrix_setindex(temp_res, 0, map.column + 2, char, &nl);
    for (maze_size_t i = 1; i <= map.row; ++i) {
        Matrix_setindex(temp_res, i, 0, char, &_border);
        for (maze_size_t j = 1; j <= map.column; ++j) {
            maze_value_t val = MazeMap_getindex(map, i - 1, j - 1);
            switch (val) {
                case MAP_WALL:
                    Matrix_setindex(temp_res, i, j, char, &_wall);
                    break;
                case MAP_ROAD:
                    Matrix_setindex(temp_res, i, j, char, &_road);
                    break;
                default:
                    return -1;
            }
        }
        Matrix_setindex(temp_res, i, map.column + 1, char, &_border);
        Matrix_setindex(temp_res, i, map.column + 2, char, &nl);
    }
    for (maze_size_t i = 0; i < map.column + 2; ++i) {
        Matrix_setindex(temp_res, map.row + 1, i, char, &_border);
    }
    Matrix_setindex(temp_res, map.row + 1, map.column + 2, char, &no);

    Matrix_setindex(temp_res, map.st.p0 + 1, map.st.p1 + 1, char, &_st);
    Matrix_setindex(temp_res, map.ed.p0 + 1, map.ed.p1 + 1, char, &_ed);

    for (int i = 1; i < map.solve_step; ++i) {
        Point idx = map.solve_list[i];
        Matrix_setindex(temp_res, idx.p0 + 1, idx.p1 + 1, char, &_solve);
    }
    return (map.row + 2) * (map.column + 3);
}

int MazeMap_print_ex(MazeMap map, char _border, char _wall, char _road, char _st, char _ed, char _solve) {
    int length = MazeMap_to_str(map, NULL);
    char *res = (char *) malloc(sizeof(char) * length);
    MazeMap_to_str_ex(map, res, _border, _wall, _road, _st, _ed, _solve);
    printf("%s\n", res);
    free(res);
    return length;
}

maze_value_t MazeMap_getindex(MazeMap map, maze_size_t i, maze_size_t j) {
    return m_getindex(map._map, i, j);
}

void MazeMap_setindex(MazeMap map, maze_size_t i, maze_size_t j, maze_value_t v) {
    m_setindex(map._map, i, j, v);
}
