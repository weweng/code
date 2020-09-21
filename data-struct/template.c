#include <stdio.h>
#include <stdlib.h>


void foo(int n) {
    if (n >0) {
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
	printf("\n Please provide a positive integer number.");
	return -1;
    }
    int n = atoi(argv[1]);
    if (n <= 0) {
	printf("\n Please provide a valid positive integer number.");
	return -2;
    }
    int tower_A = 1, tower_B = 2, tower_C = 3;
    
    unsigned long t0 = __builtin_ia32_rdtsc();
    foo(n);
    unsigned long t1 = __builtin_ia32_rdtsc();

    printf("\n : total %d time=%lu \n", n, t1-t0);
    
    return 0;
}
