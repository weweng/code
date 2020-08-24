#include <stdio.h>
#include <stdlib.h>

void insert_array(int A[], int N) {
    // insertion sort is not adaptive
    int i , j;
    
    unsigned long t0 = __builtin_ia32_rdtsc();
    for (i = 1; i < N-1; i++) {
        int tmp = A[i];
        for (j = i - 1; j >= 0; j--) {
            if (A[j] > tmp)
                A[j+1] = A[j]; // shift to right
            else
                break;
        }
        A[j+1] = tmp;
    }

    unsigned long t1 = __builtin_ia32_rdtsc();
    printf("\n %s spent time: %lu \n", __FUNCTION__, t1-t0);
}

typedef struct Node_ {
    struct Node_ *next;
    int data;
} Node;

void printList (Node *head) {
    Node *node = head;
    while (node) {
	printf("%d ", node->data);
	printf("%s", (node->next == NULL) ? "\n":",");
	node = node->next;
    };
}

void sortedInsert(Node **head_ref, Node* new_node) {
    printf("\n insert node %d \n", new_node->data);
    
    // head element need special treatment
    // if this is the first node, it becomes head
    // or if it is smaller than head, it becomes head as well
    Node *head = *head_ref;
    if (head == NULL || head->data > new_node->data) {
        new_node->next = head;
        *head_ref = new_node;
        //printList(new_node);
        return;
    }

    Node *current = head;
    while (current->next) {
        if (current->next && current->next->data > new_node->data) {
            // insert new node between current and next
            Node *tmp = current->next;
            current->next = new_node;
            new_node->next = tmp;
            return;
        }
        current = current->next;
    }
    // this seems to be the largest one so far, append at the end
    current->next = new_node;
    new_node->next = NULL;

    //printList(*head_ref);
}

/* *
 */
void insert_list(Node **head) {
    Node *current, *next = NULL;
    Node *sortedHead = NULL;
    
    if (*head == NULL) return;

    current = *head;
    while (current) {
        next = current->next;
        sortedInsert(&sortedHead, current);
        current = next;
    }

    *head = sortedHead;
}

int main(int argc, char *argv[]) {
    int A[] = {9, 2, 3, 10, 20, 6, 30, 16, 1 , 78};
    int N = sizeof(A)/sizeof(int);

    insert_array(A, N);
    
    printf("\n After insertion sort: \n");
    for (int i = 0; i < N; i++) {
	printf("%d ", A[i]);
	printf("%s", (i == N-1) ? "\n":",");
    }

    ///////////////////////////////////////////////////
    // create a single linked list
    int B[] = {2, 3, 20, 6, 30, 16, 1, 78, 9, 10};
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

    insert_list(&head);

    printf("\n After List insertion sort: \n");
    Node *node = head;
    while (node) {
	printf("%d ", node->data);
	printf("%s", (node->next == NULL) ? "\n":",");
	node = node->next;
    };
    
    return 1;
}
