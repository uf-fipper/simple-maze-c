//
// Created by m1396 on 2021/9/1.
//

#ifndef MAZE_MATRIX_H
#define MAZE_MATRIX_H

#include "mazedef.h"

/*
 * int data;
 * Matrix m;
 * Matrix_init(&m, 30, 40, int); // 创建一个 30*40 的 int 型矩阵
 * Matrix_getindex(m, 3, 4, int, &data); // data = m[3, 4]
 * Matrix_setindex(m, 3, 4, int, &data); // m[3, 4] = data
 * Matrix_free(m);
 *
 * 如果希望直接取值，而不是通过传递地址的方式，则可以这么写
T Matrix_getindex_T(Matrix m, maze_size_t i, maze_size_t j) {
    T res;
    Matrix_getindex(m, i, j, &res);
    return res;
}
 * 就能直接 T res = Matrix_getindex_T(m, i, j); 了
 */
typedef struct {
    BYTE *data;
    maze_size_t row;
    maze_size_t column;
    maze_size_t size;
} Matrix;

int Matrix_init_size(Matrix *m, maze_size_t row, maze_size_t column, maze_size_t size);
void Matrix_free(Matrix m);
int Matrix_getindex_size(Matrix m, maze_size_t i, maze_size_t j, maze_size_t size, BYTE *data);
int Matrix_setindex_size(Matrix m, maze_size_t i, maze_size_t j, maze_size_t size, const BYTE *data);

#define Matrix_init(m, r, c, t) Matrix_init_size(m, r, c, sizeof(t))
#define Matrix_getindex(m, i, j, t, d) Matrix_getindex_size(m, i, j, sizeof(t), (BYTE *) (d))
#define Matrix_setindex(m, i, j, t, d) Matrix_setindex_size(m, i, j, sizeof(t), (BYTE *) (d))

#endif //MAZE_MATRIX_H
