/*
 * Use array based stack, to evaluate an expression
 * single digit number expression first need to be converte int postfix form using stack
 * then evaluate using stack.
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
        printf("\n stack is full, can not push, top=0x%x", st->top);
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

    stack = createStack(strlen(input));

    if (stack == NULL) {
        printf("\n Failed to create stack.\n");
        return 0;
    }

    // first converte from infix to postfix form
    char *postfix = malloc(strlen(input)+1);
    memset(postfix, 0, strlen(input)+1);
    char *tmp = postfix;

    while (*p) {
        //printf("\n processing %s \n ", p);
        if (*p >= '0' && *p <= '9') {
            // for operand push into postfix
            *tmp++ = *p;
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            // push into stack if the current stack operator is lower precedence
            // otherwise pop all operators in stack which has same or high precedence
            if (isEmpty(stack)) {
                if (!push(stack, *p)) {
                    printf("\n ERR: Fails to push into stack #1");
                    goto cleanup;
                }
            } else {
                do {  
                    int cur_op = peek(stack, 0);
                    if (cur_op == -1) goto cleanup;
                
                    //now compare the precedence
                    int pop_act = 1;
                    if (*p == '+' || *p == '-') {
                        pop_act = 1;
                    } else if (*p == '*' || *p == '/') {
                        if (cur_op == '-' || cur_op == '+') {
                            pop_act = 0;
                        } else {
                            pop_act = 1;
                        }
                    }
                    if (!pop_act) {
                        if (!push(stack, *p)) {
                            printf("\n ERR: Failed to push #2");
                            goto cleanup;
                        }
                        // push into stack, we are done with this character
                        break;
                    } else {
                        if (pop(stack, &cur_op)) {
                            // send cur_op to posfix, then check check next one
                            *tmp++ = cur_op;
                            if (isEmpty(stack)) {
                                push(stack, *p);
                                break;
                            }
                            continue;
                        } else {
                            printf("\n ERR: failed to pop #3");
                            goto cleanup;
                        }
                    }
                } while (1);
            }
        } else {
            if (*p != ' ') printf("\nWARNING: ignore invalid character: %d", *p);
        }
        p++;
    }
    // popup any operator remaining in the stack
    while (!isEmpty(stack)) {
        int cur_op;
        pop(stack, &cur_op);
        *tmp++ = cur_op;
    }
    printf("\nINFO: postfix is %s \n", postfix);

    // evaluate 
    p = postfix;
    int result;
    while (*p) {
        if ( *p >= '0' && *p <= '9') {
            if (!push(stack, (*p - '0'))) {
                printf("\n ERR: Failed to push into stack #10\n");
                goto cleanup;
            }
        } else if (*p == '+'  || *p == '-' || *p == '*' || *p == '/') {
            int t1, t2;
            if (!pop(stack, &t2)) {
                printf("\n ERR: Failed to pop stack #11\n");
                goto cleanup;
            }
            if (!pop(stack, &t1)) {
                printf("\n ERR: Failed to pop stack #12\n");
                goto cleanup;
            }
            if (*p == '+') {
                push(stack, t1+t2);
                printf("\n info: %d + %d ", t1, t2);
            }
            if (*p == '-') {
                push(stack, t1-t2);
                printf("\n info: %d - %d ", t1, t2);
            }
            if (*p == '*') {
                push(stack, t1*t2);
                printf("\n info: %d * %d ", t1, t2);
            }
            if (*p == '/') {
                push(stack, t1/t2);
                printf("\n info: %d / %d ", t1, t2);
            }
        }
        p++;
    }
    if (!pop(stack, &result)) {
        printf("\n ERR: Failed to pop stack #13\n");
        goto cleanup;
    }
    printf("\n result = %d, stack empty=%d \n", result, isEmpty(stack));

cleanup:
    free(postfix);
    free(stack);

    return 1;
}
