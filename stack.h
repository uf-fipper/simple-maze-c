//
// Created by m1396 on 2021/8/24.
//

#ifndef MAZE_STACK_H
#define MAZE_STACK_H

#include "mazedef.h"

struct stack_list_t {
    BYTE *list;
    struct stack_list_t *next;
};

/*
 * int a = 3;
 * Stack S = Stack_new();
 * Stack_push(S, int, &a); // 将a入栈
 * Stack_pop(S, int, &a); // 将a出栈
 * Stack_free(S);
 *
 * Stack_new_size(maze_size_t size) // size 为一个链表所占的字节数，实际上无论多少都可以无限入栈
 * 如果需要通过值类型入栈，可以这样写
int Stack_push_T(Stack S, T data) {
    return Stack_push(S, T, &data);
}
 * 然后直接调用即可
 * (想不到更好的解决方法了)
 */
typedef struct stack_t {
    maze_size_t LIST_LENGTH;
    maze_size_t top;
    struct stack_list_t *list;
} *Stack;

Stack Stack_new_size(maze_size_t size);
void Stack_free(Stack S);
int Stack_push_byte(Stack S, BYTE data);
int Stack_push_byte_size(Stack S, maze_size_t size, BYTE *data);
int Stack_pop_byte(Stack S, BYTE *data);
int Stack_pop_byte_size(Stack S, maze_size_t size, BYTE *data);
int Stack_isempty(Stack S);
maze_size_t Stack_length(Stack S);
#define Stack_new() Stack_new_size(sizeof(maze_size_t))
#define Stack_push(S, t, dp) Stack_push_byte_size(S, sizeof(t), (BYTE *) (dp))
#define Stack_pop(S, t, rp) Stack_pop_byte_size(S, sizeof(t), (BYTE *) (rp))

#endif //MAZE_STACK_H
