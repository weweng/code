/*
 * Implement array based stack, which is fast access
 * but limited by size.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Stack_ {
    int size;
    int top;
    void* S[0];
} Stack;

Stack *createStack(int size) {
    Stack *st;

    st = calloc(1, sizeof(Stack) + sizeof(void *)*size);
    if (st) {
        st->size = size;
        st->top = -1; // set to -1 to indicate it is empty
    }

    return st;
} 

int push(Stack *st, void* val) {
    if (st->top >= st->size -1) {
        printf("\n stack is full, can not push");
        return 0;
    }

    st->top++;
    st->S[st->top] = val;
    return 1;
}

int pop(Stack *st, void** val) {
    if (st->top < 0) {
        printf("\n nothing to pop\n");
        return 0;
    }
    *val = st->S[st->top];
    st->top--;
    return 1;
}

void* peek(Stack *st, int index) {
    if (st->top - index >= 0) {
        return st->S[st->top-index];
    } else {
        printf("\n no such element \n");
        return NULL;
    }
}

void displayStack(Stack *st) {
    for (int i=st->top; i>=0; i--) {
        printf("%p ", st->S[i]);
    }
}

int isFullStack(Stack *st) {
    return (st->top >= st->size -1);
}

int isEmptyStack(Stack *st) {
    return (st->top == -1);
}

void freeStack(Stack *st) {
    free(st);
}

