#include <stdio.h>
#include <stdlib.h>

unsigned long steps = 0;

void toh(int n, int A, int B, int C) {
    if (n >0) {
      toh(n-1, A, C, B);
      printf("\n(%d, %d), ", A, C);
      steps++;
      toh(n-1, B, A, C);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
	printf("\n Please provide a positive integer number.");
	return -1;
    }
    int N = atoi(argv[1]);
    if (N <= 0) {
	printf("\n Please provide a valid positive integer number.");
	return -2;
    }
    int tower_A = 1, tower_B = 2, tower_C = 3;
    
    toh(N, tower_A, tower_B, tower_C);
    printf("\n Recur: total steps for %d discs: %lu \n", N, steps);

    return 0;
}
