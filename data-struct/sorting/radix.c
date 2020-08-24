#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;

void radix_array(int A[], int N) {
    int i, j, round = 0;
    Node *tmp_nodes;
    Node *aux_space[10]; //radix: only need ten space

    unsigned long t0 = __builtin_ia32_rdtsc();

    // space to converte int into a node
    tmp_nodes = calloc(N, sizeof(Node));

    while (1) {
        //check if it is in sorted stated
        int sorted = 1;
        for (i=0; i<N-1; i++) {
            if (A[i] > A[i+1]) {
                sorted = 0;
                break;
            }
        }
        if (sorted) break;
        
        round++;
        // populate data into a SLL to be used next
        memset(tmp_nodes, 0, N*sizeof(Node));
        for (i=0; i<N; i++) 
            tmp_nodes[i].data = A[i]; // treat as Node tmp_nodes[N]
        
        for (j=0; j<10; j++) {
            aux_space[j] = NULL;
        }
        
        //populate into aux space
        for(i=0; i<N; i++) {
            //check the last nth digits
            int mod=1;
            for (j=0; j<round; j++) mod *=10;
            int slot = (A[i]%mod)/(mod/10);
            if (aux_space[slot] == NULL) {
                aux_space[slot] = &tmp_nodes[i];
            } else {
                Node *it = aux_space[slot];
                while (it->next) it = it->next;
                it->next = &tmp_nodes[i];
            }
        }

        //push back to original array based on current sorting result
        i = 0;
        for(j=0; j<10; j++) {
            if (aux_space[j]) {
                Node *it = aux_space[j];
                while (it) {
                    A[i++] = it->data;
                    it = it->next;
                }
            }
        }

    }

    free(tmp_nodes);

    unsigned long t1 = __builtin_ia32_rdtsc();
    printf("\n %s spent time: %lu \n", __FUNCTION__, t1-t0);
}

void radix_list(Node **head_ref) {
    int i, j, round = 0;
    Node *aux_space[10];

    if (*head_ref == NULL) return;

    unsigned long t0 = __builtin_ia32_rdtsc();

    while (1) {
        // check if it is sorted alread
        int sorted = 1;
        Node *it = *head_ref;
        while (it->next) {
            if (it->data > it->next->data) {
                sorted = 0;
                break;
            }
            it = it->next;
        }
        if (sorted) break;

        // start a new round of sorting
        // fill into aux_space
        for(j=0; j<10; j++) aux_space[j] = NULL;
        round++;
        int mod = 1;
        for (i=0; i<round; i++) mod *=10;
        it = *head_ref;
        while (it) {
            Node *next = it->next;
            int slot = (it->data%mod)/(mod/10);
            if (aux_space[slot] == NULL) {
                aux_space[slot] = it;
                aux_space[slot]->next = NULL;
            } else {
                Node *it_ = aux_space[slot];
                while (it_->next) it_ = it_->next;
                it_->next = it;
                it->next = NULL;
            }
            it = next;
        }

        // now we merge all into one list
        Node *newHead = NULL;
        Node *last_node_slot = NULL;
        for (j=0; j<10; j++) {
            if (aux_space[j] != NULL) {
                if (newHead == NULL) {
                    newHead = aux_space[j];
                } else {
                    last_node_slot->next = aux_space[j];
                }
                Node *it_ = aux_space[j];
                while (it_->next) it_ = it_->next;
                last_node_slot = it_;
                last_node_slot->next = NULL;
            } 
        }
        *head_ref = newHead;
    }

    unsigned long t1 = __builtin_ia32_rdtsc();
    printf("\n %s spent time: %lu \n", __FUNCTION__, t1-t0);
}
int main(int argc, char *argv[]) {
    int A[] = {9, 2, 3, 8888, 321, 9999999, 20, 3456, 10, 99, 36, 6, 30, 16, 38, 36, 1 , 78};
    int N = sizeof(A)/sizeof(int);

    radix_array(A, N);
    
    printf("\n After Array radix sort: \n");
    for (int i = 0; i < N; i++) {
	printf("%d ", A[i]);
	printf("%s", (i == N-1) ? "\n":",");
    }

    // create a single linked list
    int B[] = {9, 2, 3, 8888, 321, 9999999, 20, 3456, 10, 99, 36, 6, 30, 16, 38, 36, 1 , 78};
    N = sizeof(B)/sizeof(int);
    Node *head = NULL;
    for (int i = 0; i < N; i++) {
	Node *tmp = (Node *)calloc(1, sizeof(Node));
	tmp->data = B[i];
	if (head == NULL) {
	    head = tmp;
	} else {
	    tmp->next = head;
	    head = tmp;
	}
    }
    radix_list(&head);

    printf("\n After List Radix sort: \n");
    Node *it = head;
    while (it) {
	printf("%d ", it->data);
	printf("%s", (it->next == NULL) ? "\n":",");
	it = it->next;
    }

    printf("\nAll done!\n");

    return 1;
}
