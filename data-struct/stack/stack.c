#include <stdio.h>
#include <stdlib.h>

typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;

typedef struct Stack_ {
    int size;
    int top;
    int *S;
} Stack;

Stack *createStack(int size) {
    Stack *st;

    st = malloc(sizeof(Stack));
    if (st == NULL) return st;

    st->size = size;
    st->S = (int *)malloc(size*sizeof(int));
    if (st->S == NULL) {
        free(st);
        return NULL;
    }
    st->top = -1; // set to -1 to indicate it is empty

    return st;
} 

int push(Stack *st, int val) {
    if (st->top >= st->size -1) {
        printf("\n stack is full, can not push");
        return 0;
    }

    st->top++;
    st->S[st->top] = val;
    return 1;
}

int pop(Stack *st, int *val) {
    if (st->top < 0) {
        printf("\n nothing to pop\n");
        return 0;
    }
    *val = st->S[st->top];
    st->top--;
    return 1;
}

int peek(Stack *st, int index) {
    if (st->top - index >= 0) {
        return st->S[st->top-index];
    } else {
        printf("\n no such element \n");
        return -1;
    }
}

void displayStack(Stack *st) {
    for (int i=st->top; i>=0; i--) {
        printf("%d ", st->S[i]);
    }
}

int isFull(Stack *st) {
    return (st->top >= st->size -1);
}

int isEmpty(Stack *st) {
    return (st->top == -1);
}

void freeStack(Stack *st) {
    free(st->S);
    free(st);
}

int main(int argc, char *argv[]) {

    Stack *stack;

    stack = createStack(58);

    if (stack == NULL) {
        printf("\n Failed to create stack.\n");
        return 0;
    }

    for (int i = 0; i < 60; i++) {
        if (!push(stack, i)) break;
    }

    printf("\n Stack is full: %d ", isFull(stack));

    printf("\n Show whole stack:\n");
    displayStack(stack);

    printf("\n Pop last 40 elements:\n");
    for (int i = 0; i < 40; i++) {
        int val ;
        if (!pop(stack, &val)) break;
        printf("%d, ", val);
    }
    printf("\n Stack is empty: %d ", isEmpty(stack));

    printf("\n Pop another 40 elements:\n");
    for (int i = 0; i < 40; i++) {
        int val ;
        if (!pop(stack, &val)) break;
        printf("%d, ", val);
    }

    printf("\n Stack is empty: %d \n", isEmpty(stack));

    free(stack);

    return 1;
}
