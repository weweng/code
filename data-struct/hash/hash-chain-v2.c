#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHTABLE_SIZE 10

typedef struct Node_ {
    struct Node_ *next;
    int key;
    char val[0]; //act as place holder for value
} Node;

int hash(int key) {
    return key % HASHTABLE_SIZE;
}

void hashInsert(Node **hash_tab, int key, char *val) {
    int index = hash(key);

    Node *newNode = calloc(1, sizeof(Node) + strlen(val)+1);
    newNode->key = key;
    memcpy(newNode->val, val, strlen(val));

    // use insert sort way to make list to be in sorted order
    Node *head = hash_tab[index];
    if (head == NULL || head->key > key) {
        hash_tab[index] = newNode;
        newNode->next = head;
        return;
    }

    // insert between current and next
    Node *cur = hash_tab[index];
    while (cur->next) {
        if (cur->next->key > key) {
            newNode->next = cur->next;
            cur->next = newNode;
            return;
        }
        cur = cur->next;
    }

    // append at the end
    cur->next = newNode;
}
void  hashDelete(Node **hash_tab, int key) {
    int index = hash(key);

    Node *head = hash_tab[index];
    if (head == NULL) return;

    if (head->key == key) {
        hash_tab[index] = head->next;
        free(head);
        return;
    }

    Node *cur = head;
    while (cur->next) {
        if (cur->next->key == key) {
            Node *tmp = cur->next;
            cur->next = cur->next->next;
            free(tmp);
            return;
        }
        if (cur->next->key > key) return; // list is sorted, no need check further
        cur = cur->next;
    }
}

char* hashGet(Node ** hash_tab, int key) {
    int index = hash(key);

    if (hash_tab[index] == NULL) return NULL;

    Node *cur = hash_tab[index];
    while (cur) {
        if (cur->key == key) {
            return cur->val;
        } else if (cur->key > key) return NULL;
        cur = cur->next;
    }

    return NULL;
}

void  displayHashTable(Node **hash_tab) {
    for (int i = 0; i < HASHTABLE_SIZE; i++) {
        Node *head = hash_tab[i];
        if (head == NULL) {
            printf("\n ---slot %d is empty \n", i);
        } else {
            Node *cur = head;
            printf("\n ---slot %d has:\n", i);
            while (cur) {
                printf (" (key=%d, val=%s) ", cur->key, cur->val);
                cur = cur->next;
            }
            printf("\n");
        }
    }
}

int main(int argc, char *argv[]) {

    Node **hash_tab;

    hash_tab = calloc(HASHTABLE_SIZE, sizeof(Node*));
    for (int i = 0; i < 30; i++) {
        char val[128];
        snprintf(val, 128, "hello world %d", i);
        hashInsert(hash_tab, i, val);
    }

    displayHashTable(hash_tab);

    for (int i = 29; i >= 0; i--) {
        hashDelete(hash_tab, i);
    }

    printf("\n After delete all:\n");
    displayHashTable(hash_tab);
                    
    return 1;
}
