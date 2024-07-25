//
// Created by m1396 on 2021/8/26.
//

#ifndef MAZE_POINT_H
#define MAZE_POINT_H

#include "mazedef.h"

typedef struct {
    maze_size_t p0;
    maze_size_t p1;
} Point;

Point Point_get(maze_size_t p0, maze_size_t p1);

int Point_init_p(Point *point, const maze_size_t p[2]);
int Point_init_d(Point *point, maze_size_t p0, maze_size_t p1);

int Point_equal(Point p1, Point p2);
int Point_equal_d(Point p, maze_size_t p0, maze_size_t p1);

#endif //MAZE_POINT_H
