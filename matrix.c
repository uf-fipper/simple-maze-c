//
// Created by m1396 on 2021/9/1.
//

#include "matrix.h"

#include <stdlib.h>

int Matrix_init_size(Matrix *m, maze_size_t row, maze_size_t column, maze_size_t size) {
    m->row = row;
    m->column = column;
    m->size = size;
    m->data = (BYTE *) malloc(sizeof(BYTE) * size * row * column);
    if (m->data == NULL) {
        return 0;
    }
    return 1;
}

void Matrix_free(Matrix m) {
    free(m.data);
}

int Matrix_getindex_size(Matrix m, maze_size_t i, maze_size_t j, maze_size_t size, BYTE *data) {
    if (data == NULL) {
        return 0;
    }
    int base_idx = i * m.column * size + j * size;
    for (int t = 0; t < size; ++t) {
        data[t] = m.data[base_idx + t];
    }
    return 1;
}

int Matrix_setindex_size(Matrix m, maze_size_t i, maze_size_t j, maze_size_t size, const BYTE *data) {
    if (data == NULL) {
        return 0;
    }
    int base_idx = i * m.column * size + j * size;
    for (int t = 0; t < size; ++t) {
        m.data[base_idx + t] = data[t];
    }
    return 1;
}
