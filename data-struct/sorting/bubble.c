#include <stdio.h>
#include <stdlib.h>

void bubble_array(int A[], int N) {
    // bubblde sort is adaptive,it can be stopped
    // during middle no swapped is detected during one pass sort 
    int swapped = 0;
    
    unsigned long t0 = __builtin_ia32_rdtsc();
    for (int i = 0; i < N-1; i++) {
        swapped = 0;
        for (int j = 0; j < N-1-i; j++) {
            if (A[j] > A[j+1]) {
                int tmp = A[j+1];
                A[j+1] = A[j];
                A[j] = tmp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }

    unsigned long t1 = __builtin_ia32_rdtsc();
    printf("\n %s spent time: %lu \n", __FUNCTION__, t1-t0);
}

typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;

void bubble_list(Node *head) {
    Node *current, *last_cmp = NULL;
    int swapped = 0;

    if (head == NULL) return;
    
    while (1) {
        current = head;
        swapped = 0;
        while (current->next != last_cmp) { // check if we reach checking point
            if (current->data > current->next->data) {
                // swap is needed
                int t = current->next->data;
                current->next->data = current->data;
                current->data = t;
                swapped = 1;
            }
            current = current->next;
        }
        last_cmp = current; // update last check point
        if (!swapped) break;
    }
}

int main(int argc, char *argv[]) {
    int A[] = {9, 2, 3, 10, 20, 6, 30, 16, 1 , 78};
    int N = sizeof(A)/sizeof(int);

    bubble_array(A, N);
    
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

    bubble_list(head);

    printf("\n After List bubble sort: \n");
    Node *node = head;
    while (node) {
	printf("%d ", node->data);
	printf("%s", (node->next == NULL) ? "\n":",");
	node = node->next;
    };
    
    return 1;
}
