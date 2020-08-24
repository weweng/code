#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this assume the number between [l, m] and [m,h] are sorted
// but [1,h] is not sorted yet.
void merge(int A[], int l, int m, int h) {
    int i, j , k;
    int B[h-l+1];

    if (l>=h) return;

    i = l; j = m+1; k =0;
    memset(B, 0, h-l+1);
    do {
        if (A[i] < A[j]) 
            B[k++] = A[i++];
        else
            B[k++] = A[j++];
    } while (i<=m && j <=h);
    for(; j<=h; j++) 
        B[k++] = A[j]; 
    for(;i <=m; i++) 
        B[k++] = A[i]; 

    for(i=l;i<=h;i++) {
        A[i] = B[i-l];
    }
}

void merge_array(int A[], int l, int h) {
    if (l < h) {
        int m = l + (h-l)/2;
        merge_array(A, l,m);
        merge_array(A, m+1, h);
        merge(A,l,m,h);
    }
}


int main(int argc, char *argv[]) {
    int A[] = {9, 2, 3, 10, 20, 6, 30, 16, 1 ,6, 78, 1, 17};
    int N = sizeof(A)/sizeof(int);

    merge_array(A, 0, N-1);

    printf("\n After Array iterative merge sort: \n");
    for (int i = 0; i < N; i++) {
	printf("%d ", A[i]);
	printf("%s", (i == N-1) ? "\n":",");
    }

    return 1;
}
