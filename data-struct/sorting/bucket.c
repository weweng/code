#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;

void bucket_array(int A[], int N) {
    int i, j;
    Node *tmp_nodes;

    unsigned long t0 = __builtin_ia32_rdtsc();

    // space to converte int into a node
    tmp_nodes = calloc(N, sizeof(Node));

    // find max number and allocate enough space
    int max = -1;
    for (i = 0; i < N; i++) { 
        if (A[i] > max) max = A[i];
        tmp_nodes[i].data = A[i]; // treat as Node tmp_nodes[N]
        //(tmp_nodes+i)->data = A[i]; // treat as Node* tmp_nodes
    }

    // fill in presence of each number in respective slot
    // with a singly linked list
    Node **aux_space; // same as Node *aux_space[max+1] but with dynamic allocation
    aux_space = calloc(max+1, sizeof(Node *));

    for (i = 0; i < N; i++) {
        Node *tmpNode = &tmp_nodes[i];
        tmpNode->data = A[i];
        // insert in the beginning of given slot list
        Node *head = (aux_space[A[i]]);
        aux_space[A[i]] = tmpNode; 
        tmpNode->next = head;
    }

    // push from aux space to original space to overwrite it
    i = 0;
    for (j = 0; j < max+1; j++) {
        Node *tmp = aux_space[j];
        while (tmp) {
            A[i++] = tmp->data;
            tmp= tmp->next;
        }
    } 

    free(aux_space);
    free(tmp_nodes);

    unsigned long t1 = __builtin_ia32_rdtsc();
    printf("\n %s spent time: %lu \n", __FUNCTION__, t1-t0);
}

void bucket_list(Node **head_ref) {
    int i;
    Node *tmp_nodes;

    if (*head_ref == NULL) return;

    unsigned long t0 = __builtin_ia32_rdtsc();

    Node *it = *head_ref;
    int max = it->data;
    while (it->next) {
        if (it->next->data > max)
            max = it->next->data;
        it = it->next;
    }
    
    Node **aux_space = calloc(max+1, sizeof(Node *));
    // fill in aux_space based on value
    it = *head_ref;
    while (it) {
        Node *tmp_next = it->next;
        Node * tmp = aux_space[it->data];
        aux_space[it->data] = it;
        it->next = tmp;
        it = tmp_next;
    }

    // form a new list by going thru all valid sublist in aux_space
    // connect all sublists into one list
    Node *last_in_slot = NULL; // serve as head of next slot
    for (i = 0; i < max+1; i++) {
        Node *current_head = aux_space[i];
        if ( current_head != NULL) {
            if (last_in_slot == NULL) {
                *head_ref = current_head;
            } else {
                last_in_slot->next = current_head;
            }
            it = current_head;
            while (it->next) it = it->next;
            last_in_slot = it;
        } 
    }

    free(aux_space);

    unsigned long t1 = __builtin_ia32_rdtsc();
    printf("\n %s spent time: %lu \n", __FUNCTION__, t1-t0);
}
int main(int argc, char *argv[]) {
    int A[] = {9, 2, 3, 10, 20, 2, 10, 99, 36, 6, 30, 16, 38, 36, 1 , 78};
    int N = sizeof(A)/sizeof(int);

    bucket_array(A, N);
    
    printf("\n After Array bucket sort: \n");
    for (int i = 0; i < N; i++) {
	printf("%d ", A[i]);
	printf("%s", (i == N-1) ? "\n":",");
    }

    // create a single linked list
    int B[] = {9, 2, 3, 10, 20, 2, 10, 99, 36, 6, 30, 16, 38, 36, 1 , 78 };
    N = sizeof(B)/sizeof(int);
    Node *head = NULL;
    for (int i = 0; i < N; i++) {
	Node *tmp = (Node *)malloc(sizeof(Node));
	tmp->data = B[i];
	if (head == NULL) {
	    head = tmp;
	} else {
	    tmp->next = head;
	    head = tmp;
	}
    }

    bucket_list(&head);

    printf("\n After List bubble sort: \n");
    Node *node = head;
    while (node) {
	printf("%d ", node->data);
	printf("%s", (node->next == NULL) ? "\n":",");
	node = node->next;
    };
    
    return 1;
}
