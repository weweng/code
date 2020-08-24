#include <stdio.h>
#include <stdlib.h>

void count_array(int A[], int N) {
    int i, j;
    
    unsigned long t0 = __builtin_ia32_rdtsc();
    // find max number and allocate enough space
    int max = A[0];
    for (i = 1; i < N; i++) 
        if (A[i] > max) max = A[i];

    // fill in presence of each number in respective slot
    int *aux_A = calloc(max+1, sizeof(int));
    for (i = 0; i < N; i++) {
        aux_A[A[i]]++;
    }

    // push from aux space to original space to overwrite it
    i = 0;
    for (j = 0; j < max+1; j++) {
        while (aux_A[j]) {
            A[i++] = j;
            aux_A[j] = aux_A[j] - 1;
        }
    } 

    free(aux_A);

    unsigned long t1 = __builtin_ia32_rdtsc();
    printf("\n %s spent time: %lu \n", __FUNCTION__, t1-t0);
}

int main(int argc, char *argv[]) {
    int A[] = {9, 2, 3, 10, 20, 2, 10, 99, 36, 6, 30, 16, 38, 36, 1 , 78};
    int N = sizeof(A)/sizeof(int);

    count_array(A, N);
    
    printf("\n After Array count sort: \n");
    for (int i = 0; i < N; i++) {
	printf("%d ", A[i]);
	printf("%s", (i == N-1) ? "\n":",");
    }

    return 1;
}
