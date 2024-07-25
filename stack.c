//
// Created by m1396 on 2021/8/24.
//

#include "stack.h"
#include <stdlib.h>

struct stack_list_t *stack_list_new(maze_size_t size) {
    struct stack_list_t *res = (struct stack_list_t *) malloc(sizeof(struct stack_list_t));
    res->list = (BYTE *) malloc(sizeof(BYTE) * size);
    res->next = NULL;
    return res;
}

void stack_list_free(struct stack_list_t *list) {
    free(list->list);
    free(list);
}

Stack Stack_new_size(maze_size_t size) {
    Stack res = (Stack) malloc(sizeof(struct stack_t));
    res->list = stack_list_new(size);
    res->LIST_LENGTH = size;
    res->top = -1;
    return res;
}

void Stack_free(Stack S) {
    struct stack_list_t *temp = S->list;
    while (temp) {
        S->list = S->list->next;
        stack_list_free(temp);
        temp = S->list;
    }
    free(S);
}

int Stack_push_byte(Stack S, BYTE data) {
    ++S->top;
    maze_size_t top = S->top % S->LIST_LENGTH;
    if (top == 0 && S->top != 0) {
        struct stack_list_t *temp = stack_list_new(S->LIST_LENGTH);
        if (temp == NULL) return 0;
        temp->list[0] = data;
        temp->next = S->list;
        S->list = temp;
    }
    else {
        S->list->list[top] = data;
    }
    return 1;
}

int Stack_push_byte_size(Stack S, maze_size_t size, BYTE *data) {
    for (maze_size_t i = 0; i < size; ++i) {
        if (!Stack_push_byte(S, data[i])) {
            return 0;
        }
    }
    return 1;
}

int Stack_pop_byte(Stack S, BYTE *data) {
    if (Stack_isempty(S)) {
        return 0;
    }
    maze_size_t top = S->top % S->LIST_LENGTH;
    if (data != NULL) *data = S->list->list[top];
    if (top == 0 && S->top != 0) {
        struct stack_list_t *temp = S->list->next;
        stack_list_free(S->list);
        S->list = temp;
    }
    --S->top;
    return 1;
}

int Stack_pop_byte_size(Stack S, maze_size_t size, BYTE *data) {
    for (maze_size_t i = size - 1; i >= 0; --i) {
        if (!Stack_pop_byte(S, &data[i])) {
            return 0;
        }
    }
    return 1;
}

int Stack_isempty(Stack S) {
    return S->top == -1;
}

maze_size_t Stack_length(Stack S) {
    return S->top + 1;
}
