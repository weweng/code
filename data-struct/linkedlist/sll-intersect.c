#include <stdio.h>
#include <stdlib.h>

typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;

typedef struct Stack_ {
    int size;
    int top;
    Node *S[0];
} Stack;

Stack *createStack(int size) {
    Stack *st = calloc(1, sizeof(Stack) + size * sizeof(Node *));
    st->size = size;
    st->top = -1;
    return st;
}

int push(Stack *st, Node *val) {
    if (st->top == st->size-1) return 0;
    st->top++;
    st->S[st->top] = val;
    return 1;
}

int pop(Stack *st, Node **val) {
    if (st->top == -1) return 0;
    *val = st->S[st->top];
    st->top--;
    return 1;
}

/* create a list */
Node *createList(int arr[], int len) {
    if (len <= 0) return NULL;

    Node *head = calloc(1, sizeof(Node));
    head->data = arr[0];
    Node *cur = head;
    for (int i = 1; i < len; i++) {
        Node *node = calloc(1, sizeof(Node));
        node->data = arr[i];
        cur->next = node;
        cur = cur->next;
    }

    return head;
}

void freeList(Node *head) {
    Node *cur = head;
    while (cur) {
        Node *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
}

void display(Node *head) {
    Node *cur = head;
    while (cur) {
        printf(" %d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    
    Stack *st1 = createStack(100);
    Stack *st2 = createStack(100);
    int arr1[] = { 1,3,5,7,9,13,15,21};
    int arr2[] = { 2,4,9,6,9,31,51,28};
    int arr3[] = { 100, 200, 300, 400, 500};

    Node *head1 = createList(arr1, 8);
    Node *head2 = createList(arr2, 8);
    Node *head3 = createList(arr3, 5);

    // append list 3 to both list 1 and list 2
    Node *cur = head1;
    while (cur->next) cur = cur->next;
    cur->next = head3;
    cur = head2;
    while (cur->next) cur = cur->next;
    cur->next = head3;

#if 0
    display(head1);
    display(head2);
#endif

    //push each list into stack
    cur = head1;
    while (cur) {
        if (!push(st1, cur)) {
            printf("\n Failed to push #1 \n");
            return 0;
        }
        cur = cur->next;
    }

    cur = head2;
    while (cur) {
        if (!push(st2, cur)) {
            printf("\n Failed to push #2 \n");
            return 0;
        }
        cur = cur->next;
    }

    // now we can pop from each stack and compare which one is diff
    Node *inters = NULL;
    while (1) {
        Node *n1, *n2;
        if (!pop(st1, &n1)) return 0 ;
        if (!pop(st2, &n2)) return 0 ;
        if (n1 == n2) 
            inters = n1;
        else 
            break;
    }
    if (inters) {
        printf("\n Intersect is found: \n");
        display(inters);
    } else {
        printf("\n No intersect is found \n");
    }
    
    // clean up
    cur = head1;
    while (cur) {
        Node *tmp = cur->next;
        if (cur == inters) break;
        free(cur);
        cur = tmp;
    }
    cur = head2;
    while (cur) {
        Node *tmp = cur->next;
        if (cur == inters) break;
        free(cur);
        cur = tmp;
    }
    cur = inters;
    while (cur) {
        Node *tmp = cur->next;
        free(cur);
        cur = tmp;
    }

    free(st1);
    free(st2);

    return 1;
}
