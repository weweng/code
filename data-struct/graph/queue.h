/* 
 * Circular Q implementation
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct Queue_ {
    int size;
    int front; // for dequeue: init to zero, to use: increment then access
    int rear;  // for enqueue: init to zero, to use: increment then access
    int Q[0];  // array of object to hold
} Queue;

int isEmpty(Queue *q) {
    return q->front == q->rear;
}

int isFull(Queue *q) {
    return q->front == (q->rear + 1)%q->size ;
}

Queue *createQueue(int size) {
    if (size == 0) return NULL;
    Queue *q = calloc(1, sizeof(Queue) + sizeof(int)*size); // cinldue space for Q
    q->size = size;
    q->front = q->rear = 0;
    return q;
}

int enqueue(Queue *q, int val) {
    if (isFull(q))         return 0;
    q->rear = (q->rear+1)%q->size; // can not use q->rear++
    q->Q[q->rear] = val;
    return 1;
}

int dequeue(Queue *q) {
    if (isEmpty(q)) return 0;
    q->front = (q->front+1)%q->size;
    return q->Q[q->front];
}

void displayQ(Queue *q) {
    for (int i = q->front; i <= q->rear; i++) {
        i %=q->size;
        printf("q[%d]=%d ", i, q->Q[i]);
    }
    printf("\n");
}
