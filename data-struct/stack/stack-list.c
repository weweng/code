/*
 * Implement linkes list based stack, which is not limited by size
 * but it is going to be slow due to allocate space for each node etc.
 * Stack will be a pointer which is head of a linked list always
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Stack_ {
    struct Stack_ *next;
    int data;
} Stack;

typedef Stack Node;

Stack *createStack(int initVal) {
    Stack *st = malloc(sizeof(Stack));
    st->next = NULL;
    st->data = initVal;

    return st;
}

int push(Stack **st_ref, int val) {
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("\n stack is full, can not push");
        return 0;
    }
    Stack *tmp = *st_ref;
    newNode->data = val;
    newNode->next = tmp;
    *st_ref = newNode;

    return 1;
}

int pop(Stack **st_ref, int *val) {
    Stack *st  = *st_ref;
    if (st == NULL) {
        printf("\n nothing to pop\n");
        return 0;
    }
    *val = st->data;
    *st_ref = st->next;
    return 1;
}

int peek(Stack *st, int index) {
    int count = 0;
    while (st) {
        count++;
        if (count == index) {
            return st->data;
        }
        st = st->next;
    }
    printf("\n no such element \n");
    return -1;
}

void displayStack(Stack *st) {
    while (st) {
        printf("%d ", st->data);
        st = st->next;
    }
}

int isFull(Stack *st) {
    return 0; //;
}

int isEmpty(Stack *st) {
    return (st == NULL);
}

void freeStack(Stack *st) {
    while (st) {
        Stack *tmp = st->next;
        free(st);
        st = tmp;
    }
}

int main(int argc, char *argv[]) {

    Stack *stack;
    int initVal = 34;

    stack = createStack(initVal);

    if (stack == NULL) {
        printf("\n Failed to create stack.\n");
        return 0;
    }

    for (int i = 0; i < 60; i++) {
        if (!push(&stack, i)) break;
    }

    printf("\n Stack is full: %d ", isFull(stack));

    printf("\n Show whole stack:\n");
    displayStack(stack);

    printf("\n Pop last 40 elements:\n");
    for (int i = 0; i < 40; i++) {
        int val ;
        if (!pop(&stack, &val)) break;
        printf("%d, ", val);
    }
    printf("\n Stack is empty: %d ", isEmpty(stack));

    printf("\n Pop another 40 elements:\n");
    for (int i = 0; i < 40; i++) {
        int val ;
        if (!pop(&stack, &val)) break;
        printf("%d, ", val);
    }

    printf("\n Stack is empty: %d \n", isEmpty(stack));

    free(stack);

    return 1;
}
