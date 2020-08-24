#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int partition(int A[], int l, int h) {
    int i = l, j = h;
    int pivot = A[l];

    do {
        do {i++;} while (A[i]<=pivot && i<=h);
        do {j--;} while (A[j]>pivot && j>=l);
        if (i<j) {
            int tmp = A[i]; A[i] = A[j]; A[j] = tmp;
        }
    } while (i < j);
    //swap pivot with j, as j is the position for pivot
    A[l] = A[j]; A[j] = pivot;

    return j;
}

void quick_array(int A[], int l, int h) {
    int j = 0;

    if (l>=h) return;

    unsigned long t0 = __builtin_ia32_rdtsc();
    // do partiton to find the position for pivot
    j = partition(A, l, h);
    //now we have two partitions
    quick_array(A, l, j);
    quick_array(A, j+1, h);

    unsigned long t1 = __builtin_ia32_rdtsc();
    printf("\n %s spent time: %lu \n", __FUNCTION__, t1-t0);
}


int main(int argc, char *argv[]) {
    int A[] = {9, 2, 3, 10, 20, 6, 30, 16, 1 ,6, 78, 1, INT32_MAX};
    int N = sizeof(A)/sizeof(int);

    quick_array(A, 0, N-1);
    
    printf("\n After Array quick sort: \n");
    for (int i = 0; i < N; i++) {
	printf("%d ", A[i]);
	printf("%s", (i == N-1) ? "\n":",");
    }

    return 1;
}
