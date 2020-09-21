/*
 * Using array based stack to check if the given string parenthesis matched
 * or balanced
 * key point:
 * scan through string characters, if it is opening parenthesis, push into stack
 * if it is closing parenthesis, pop one from stack to compare to see if it is matched
 * if matches, keep going until reach the end of string and stack is empty.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    if (argc < 2) {
        printf("\n Please give an expression with parenthsises to check \n");
        return 0;
    }

    char *input = argv[1];
    char *p = input;

    stack = createStack(strlen(input+1));

    if (stack == NULL) {
        printf("\n Failed to create stack.\n");
        return 0;
    }

    while (*p) {
        if ((*p == '(') || (*p == '{') || (*p == '[')) {
            if (!push(stack, *p)) {
                printf("\n error while push into stack\n");
                return 0;
            }
        } else if ((*p == ')') || (*p == '}') || (*p == ']')) {
            int val = 0;
            if (!pop(stack, &val)) {
                printf("\n not matched, nothing from stack\n");
                return 0;
            }
            if ((*p == ')' && val == '(') ||
                (*p == '}' && val == '{') ||
                (*p == ']' && val == '[')) {
                // cool, keep going
            } else {
                printf("\n not matched \n");
                return 0;
            }
        }
        p++;
    }

    if (isEmpty(stack)) printf("\n Good match. \n");

    free(stack);

    return 1;
}
