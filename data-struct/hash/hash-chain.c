/*
 * This implements a hash table with chain.
 * Essentially it is an array of linked list, each slot of array
 * is filled with a linked list, which can be accessed by hash from key 
 * as index to access array
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHTABLE_SIZE 10

typedef struct Node_ {
    struct Node_ *next;
    int key;
    char value[0]; // this is a placehold for value, space will alocated as needed
} Node;

int hash(int key) {
    return key % HASHTABLE_SIZE;
}

void hashInsert(Node ** hash_tab, int key, char*val) {
    int index = hash(key);

    Node *head = hash_tab[index];
    
    // do a sortedInsert for the given key and value
    Node *newNode = calloc(1, sizeof(Node)+strlen(val)+1);
    newNode->key = key;
    memcpy(newNode->value, val, strlen(val)); // &newNode->value is same
    
    // special processing for the head if newNode becomes the head
    if (head == NULL || head->key > newNode->key) {
        hash_tab[index] = newNode;
        newNode->next = head;
        return;
    }

    //try to insert between current and next
    Node *cur = head;
    while (cur->next) {
        if (cur->next->key > newNode->key) {
            newNode->next = cur->next;
            cur->next = newNode;
            return;
        }
        cur = cur->next;
    } 

    // sadly, we need append it at the end
    cur->next = newNode;
}

char* hashGet(Node ** hash_tab, int key) {
    int index = hash(key);

    Node *cur = hash_tab[index];
    while (cur) {
        if (cur->key == key) return cur->value;
        // save some time because we save in a sorted order
        if (cur->key > key) return NULL; 
        cur= cur->next;
    }

    return NULL;
}

void hashDelete(Node ** hash_tab, int key) {
    int index = hash(key);

    Node *head = hash_tab[index];

    // need special processing for the head
    if (head == NULL) {
        printf("\n No item found for key %d #1\n", key);
        return;
    }
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
        // save some time because we save in a sorted order
        if (cur->key > key) {
            printf("\n No item found for key %d #2\n", key);
            return; 
        }
        cur= cur->next;
    }
}

void displayHash(Node **hash_tab) {
    for (int i = 0; i < HASHTABLE_SIZE; i++) {
        Node *cur = hash_tab[i];

        if (cur == NULL) {
            printf("\n slot %d is empty\n", i);
        } else {
            printf("\n slot %d has: \n", i);
            while (cur) {
                printf("(key=%d, value=%s), ", cur->key, cur->value);
                cur = cur->next;
            }
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    
    Node **hash_tab;

    hash_tab = calloc(HASHTABLE_SIZE, sizeof(Node *));

    for (int i =1; i < 30; i++) {
        char val[128];
        snprintf(val, 128, "hello world %d", i);
        hashInsert(hash_tab, i, val);
    }
    
    printf("\n After insertion all\n");

    displayHash(hash_tab);

    for (int i = 1; i < 30; i++) {
        hashDelete(hash_tab, i);
    }
    printf("\n After delete  all\n");
    displayHash(hash_tab);

    return 1;
}
