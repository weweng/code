#include <stdio.h>
#include <stdlib.h>

typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;

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

int main(int argc, char *argv[]) {

    int arr[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,134, 15,21,432,231, 2,3};
    Node *head = createList(arr, 20);

    // make a loop, connect last node to the node with value 5
    Node *node5, *last;
    Node *cur = head;
    while (cur->next) {
        if (cur->data == 5) node5 = cur;
        cur = cur->next;
    }
    last = cur;
    last->next = node5; // a loop is created

    // to detect loop, we use two pointers:p & q, p moves forward with 1 step
    // q moves forward with 2 steps each time
    // if either of becomes NULL, no loop, if two of them becomse equal, loop
    Node *p = head, *q = head;
    while (p && q) {
        p = p->next;
        if (q->next) 
            q = q->next->next;
        else
            break;
        if (p == q) {
            printf("\n Loop is detected \n");
            break;
        }
    }

    // break the loop 
    last->next = NULL;

    cur = head;
    p = head, q = head;
    while (p && q) {
        p = p->next;
        if (q->next)
            q = q->next->next;
        else
            break;
    }
    printf("\n middle node is %d \n", p->data);

    freeList(head);

    return 1;
}
