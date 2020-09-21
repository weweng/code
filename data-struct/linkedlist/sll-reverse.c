#include <stdio.h>
#include <stdlib.h>

typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;

typedef struct Stack_ {
    int size;
    int top;
    /*int*/ void* S[];
} Stack;

Stack *createStack(int size) {
    Stack *st = calloc(1, sizeof(Stack) + size*sizeof(void *));
    st->size = size;
    st->top = -1;

    return st;
}

int isStackFull(Stack *st) {
    return st->top == (st->size-1);
}

int isStackEmpty(Stack *st) {
    return st->top == -1;
}

int push(Stack *st, void* val) {
    if (isStackFull(st)) return 0;

    st->top++;
    st->S[st->top] = val;
    return 1;
}

int pop(Stack *st, void **val) {
    if (isStackEmpty(st)) return 0;

    *val = st->S[st->top];
    st->top--;
    return 1;

}

Node *newNode(int val) {
    Node * node = calloc(1, sizeof(Node));
    node->data = val;
    return node;
}

int main(int argc, char *argv[]) {
    
    Stack *st = createStack(100);

    Node *head = newNode(0);
    Node *cur = head;
    for (int i = 1; i<10; i++) {
        Node *n = newNode(i);
        cur->next = n;
        cur = n;
    }
    
    cur = head;
    while (cur) {
        printf(" %d ", cur->data);
        cur=cur->next;
    }
#if 0
    // reverse the linked list using stack by swap value
    // need change the Stack storage to be int array
    cur = head;
    while (cur) {
        if (!push(st, cur->data)) return 0;
        cur=cur->next;
    }
    cur = head;
    while (cur) {
        int val;
        if (!pop(st, &val)) return 0;
        cur->data = val;
        cur=cur->next;
    }
    cur = head;
    while (cur) {
        printf(" %d ", cur->data);
        cur=cur->next;
    }
#endif

    // reverse the linked list using stack by link/pointer value
    cur = head;
    while (cur) {
        if (!push(st, (void *)cur)) return 0;
        cur=cur->next;
    }
    head = NULL;
    cur = NULL;
    do {
        void *val;
        if (!pop(st, &val)) break;
        if (head == NULL) {
            head = (Node*)val;
            cur = head;
        } else {
            cur->next = (Node *)val;
            cur = cur->next;
            cur->next = NULL;
        }
    } while (1);

    cur = head;
    while (cur) {
        printf(" %d ", cur->data);
        cur=cur->next;
    }

    // reverse it using sliding pointers
    // for every move forward, shit pointer value r <- q <- p
    Node *r = NULL, *q = NULL, *p;
    p = head;
    while (p) {
        r = q; q = p ; p = p->next;
        q->next = r;
    }
    head = q;

    // show it
    cur = head;
    while (cur) {
        printf(" %d ", cur->data);
        cur=cur->next;
    }

    return 1;
}
