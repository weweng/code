#include <stdio.h>
#include <stdlib.h>

void selection_array(int A[], int N) {
    // bubblde sort is adaptive,
    // during middle no swapped is detected during one pass sort 
    int swap_need = 0;
    int candidate_index = 0;
    int candidate_val = 0;
    int i , j;
    unsigned long t0 = __builtin_ia32_rdtsc();
    for (i = 0; i < N-1; i++) {
        candidate_index = i;
        candidate_val = A[i];
        swap_need = 0;
        for (j = i+1; j < N ; j++) {
            if (A[j] < candidate_val) {
                candidate_val = A[j];
                candidate_index = j;
                swap_need = 1;
            }
        }
        if (swap_need) {
            //printf("\n swap between A[%d]=%d, A[%d]=%d \n",
            //       i, candidate_index, A[i], A[candidate_index]);
            int tmp = A[i];
            A[i] = candidate_val;
            A[candidate_index] = tmp;
        }
    }

    unsigned long t1 = __builtin_ia32_rdtsc();
    printf("\n %s spent time: %lu \n", __FUNCTION__, t1-t0);
}

typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;

void selection_list(Node *head) {
    Node *current = NULL;
    Node *cand_node = 0;
    int swap_need = 0;
    
    if (head == NULL) return;
    
    current = head;
    while (current->next) {
        Node *tmp = current;
        cand_node = current;
        swap_need = 0;
        while (tmp->next) {
            if (tmp->next->data < cand_node->data) {
                cand_node = tmp->next;
                swap_need = 1;
            }
            tmp = tmp->next;
        }
        if (swap_need) {
            int tmp = current->data;
            current->data = cand_node->data;
            cand_node->data = tmp;
        }
        current = current->next;
    }
}

int main(int argc, char *argv[]) {
    int A[] = {9, 2, 3, 10, 20, 6, 30, 16, 1 , 78};
    int N = sizeof(A)/sizeof(int);

    selection_array(A, N);
    
    printf("\n After Array bubble sort: \n");
    for (int i = 0; i < N; i++) {
	printf("%d ", A[i]);
	printf("%s", (i == N-1) ? "\n":",");
    }

    ///////////////////////////////////////////////////
    // create a single linked list
    int B[] = {78, 9, 2, 3, 10, 20, 6, 30, 16, 1 };
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

    selection_list(head);

    printf("\n After List selection sort: \n");
    Node *node = head;
    while (node) {
	printf("%d ", node->data);
	printf("%s", (node->next == NULL) ? "\n":",");
	node = node->next;
    };
    
    return 1;
}
