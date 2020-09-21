#include <stdio.h>
#include <stdlib.h>

typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;

void display(Node *head) {
    Node *cur = head;
    while (cur) {
        printf(" %d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}

Node *newNode(int val) {

    Node *node = calloc(1, sizeof(Node));
    node->data = val;
    return node;
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

    Node * head1 = newNode(1);
    Node * head2 = newNode(2);

    Node *cur = head1;
    for (int i = 3; i < 40; i +=2) {
        Node *node = newNode(i);
        cur->next = node;
        cur = cur->next;
    }

    cur = head2;
    for (int i = 4; i < 22; i +=2) {
        Node *node = newNode(i);
        cur->next = node;
        cur = cur->next;
    }

    display(head1);
    display(head2);

    // form the head
    Node *cur1 = head1, *cur2 = head2;
    Node *newHead = cur1->data < cur2->data? head1:head2;
    if (cur1->data < cur2->data) {
        cur1 = cur1->next;
    } else {
        cur2 = cur2->next;
    }

    // loop thru both lists until one is depleted
    cur = newHead;
    while (cur1 && cur2) {
        if (cur1->data < cur2->data) {
            cur->next = cur1;
            cur = cur1;
            cur1 = cur1->next;
        } else {
            cur->next = cur2;
            cur = cur2;
            cur2 = cur2->next;
        }
    }

    // only one list has elements
    if (cur1) cur->next = cur1;
    if (cur2) cur->next = cur2;

    display(newHead);

    freeList(newHead);

    return 1;
}
