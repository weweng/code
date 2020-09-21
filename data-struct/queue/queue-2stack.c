#include <stdio.h>
#include <stdlib.h>

typedef struct Stack_ {
    int size;
    int top;
    int *S;
} Stack;

typedef struct Queue_ {
    Stack *enq;
    Stack *deq;
} Queue;

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

int enqueue(Queue *q, int val) {
    return push(q->enq, val);
}

int dequeue(Queue *q, int *val) {
    // if stack is empty, need transfer all elements
    if (isEmpty(q->deq)) {
        int val;
        while (pop(q->enq, &val)) {
            push(q->deq, val);
        }
    }

    return pop(q->deq, val);
}

int main(int argc, char *argv[]) {
    
    Queue q;
    q.enq = createStack(100);
    q.deq = createStack(100);

    for (int i = 0; i < 10; i++)  enqueue(&q, i);
    for (int i = 0; i < 7; i++) {
        int val;
        if (!dequeue(&q, &val)) {
            printf("\n Failed to dequeue #1\n");
            break;
        } else {
            printf(" %d ", val);
        }
    }

    for (int i = 10; i < 25; i++)  enqueue(&q, i);

    for (int i = 0; i < 13; i++) {
        int val;
        if (!dequeue(&q, &val)) {
            printf("\n Failed to dequeue #1\n");
            break;
        } else {
            printf(" %d ", val);
        }
    }

    for (int i = 25; i < 30; i++)  enqueue(&q, i);
    for (int i = 0; i < 13; i++) {
        int val;
        if (!dequeue(&q, &val)) {
            printf("\n Failed to dequeue #1\n");
            break;
        } else {
            printf(" %d ", val);
        }
    }

    freeStack(q.enq);
    freeStack(q.deq);

   return 1;
}
