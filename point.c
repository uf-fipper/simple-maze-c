//
// Created by m1396 on 2021/8/26.
//

#include "point.h"
#include <stdio.h>

int Point_init_p(Point *point, const maze_size_t p[2]) {
    if (point == NULL || p == NULL) return 0;
    point->p0 = p[0];
    point->p1 = p[1];
    return 1;
}

int Point_init_d(Point *point, maze_size_t p0, maze_size_t p1) {
    if (point == NULL) return 0;
    point->p0 = p0;
    point->p1 = p1;
    return 1;
}

int Point_equal(Point p1, Point p2) {
    return p1.p0 == p2.p0 && p1.p1 == p2.p1;
}

int Point_equal_d(Point p, maze_size_t p0, maze_size_t p1) {
    return p.p0 == p0 && p.p1 == p1;
}

Point Point_get(maze_size_t p0, maze_size_t p1) {
    return (Point) {p0, p1};
}
