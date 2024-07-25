//
// Created by m1396 on 2021/8/24.
//

#include "queue.h"
#include <stdlib.h>

struct queue_list_t *queue_list_new(maze_size_t size) {
    struct queue_list_t *res = (struct queue_list_t *) malloc(sizeof(struct queue_list_t));
    if (res == NULL) return NULL;
    res->list = (BYTE *) malloc(sizeof(BYTE) * size);
    res->next = NULL;
    return res;
}

void queue_list_free(struct queue_list_t *list) {
    free(list->list);
    free(list);
}

Queue Queue_new_size(maze_size_t size) {
    Queue res = (Queue) malloc(sizeof(struct queue_t));
    res->LIST_LENGTH = size;
    res->rear = res->front = 0;
    res->front_list = res->rear_list = queue_list_new(size);
    return res;
}

void Queue_free(Queue Q) {
    struct queue_list_t *temp = Q->front_list;
    while (temp) {
        Q->front_list = Q->front_list->next;
        queue_list_free(temp);
        temp = Q->front_list;
    }
    free(Q);
}

int Queue_add_byte(Queue Q, BYTE data) {
    maze_size_t rear = Q->rear % Q->LIST_LENGTH;
    if (rear == 0 && Q->rear != 0) {
        Q->rear_list->next = queue_list_new(Q->LIST_LENGTH);
        if (Q->rear_list->next == NULL) return 0;
        Q->rear_list = Q->rear_list->next;
    }
    Q->rear_list->list[rear] = data;
    ++Q->rear;
    return 1;
}

int Queue_add_byte_size(Queue Q, maze_size_t size, const BYTE *data) {
    for (maze_size_t i = 0; i < size; ++i) {
        if (!Queue_add_byte(Q, data[i])) {
            return 0;
        }
    }
    return 1;
}

int Queue_leave_byte(Queue Q, BYTE *data) {
    if (Queue_isempty(Q)) return 0;
    *data = Q->front_list->list[Q->front];
    if (Q->front == Q->LIST_LENGTH - 1 && Q->rear != Q->LIST_LENGTH) {
        Q->front = 0;
        Q->rear -= Q->LIST_LENGTH;
        struct queue_list_t *temp = Q->front_list;
        Q->front_list = Q->front_list->next;
        queue_list_free(temp);
    }
    else if (Q->front == Q->LIST_LENGTH - 1) {
        Q->front = Q->rear = 0;
    }
    else {
        ++Q->front;
    }
    return 1;
}

int Queue_leave_byte_size(Queue Q, maze_size_t size, BYTE *data) {
    for (maze_size_t i = 0; i < size; ++i) {
        if (!(Queue_leave_byte(Q, &data[i]))) {
            return 0;
        }
    }
    return 1;
}

int Queue_isempty(Queue Q) {
    return Q->front == Q->rear;
}

maze_size_t Queue_length(Queue Q) {
    return Q->rear - Q->front;
}
