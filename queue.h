//
// Created by m1396 on 2021/8/24.
//

#ifndef MAZE_QUEUE_H
#define MAZE_QUEUE_H

#include "mazedef.h"

struct queue_list_t {
    BYTE *list;
    struct queue_list_t *next;
};

/*
 * int a = 3;
 * Queue Q = Queue_new();
 * Queue_add(Q, int, &a); // 将a入队
 * Queue_leave(Q, int, &a); // 将a出队
 * Queue_free(Q);
 *
 * Queue_new_size(maze_size_t size) // size 为一个链表所占的字节数，实际上无论多少都可以无限入队
 * 如果需要通过值类型入队，可以这样写
int Queue_add_T(Queue Q, T data) {
    return Queue_add(Q, T, &data);
}
 * 然后直接调用即可
 * (想不到更好的解决方法了)
 */
typedef struct queue_t {
    maze_size_t LIST_LENGTH;
    maze_size_t rear;
    maze_size_t front;
    struct queue_list_t *rear_list;
    struct queue_list_t *front_list;
} *Queue;

Queue Queue_new_size(maze_size_t size);
void Queue_free(Queue Q);
int Queue_add_byte(Queue Q, BYTE data);
int Queue_add_byte_size(Queue Q, maze_size_t size, const BYTE *data);
int Queue_leave_byte(Queue Q, BYTE *data);
int Queue_leave_byte_size(Queue Q, int size, BYTE *data);
int Queue_isempty(Queue Q);
maze_size_t Queue_length(Queue Q);

#define Queue_new() Queue_new_size(sizeof(maze_size_t))
#define Queue_add(Q, t, dp) Queue_add_byte_size(Q, sizeof(t), (BYTE *) (dp))
#define Queue_leave(Q, t, rp) Queue_leave_byte_size(Q, sizeof(t), (BYTE *) (rp))

#endif //MAZE_QUEUE_H
