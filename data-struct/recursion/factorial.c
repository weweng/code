#include <stdio.h>
#include <stdlib.h>

//Recurse: time complexity O(n), space complexity O(n)
int fac_recur(int n) {
    if (n <= 1) return 1; 
    return fac_recur(n-1)*n;
}
//Loop: time complexity O(n), space complexity O(1)
int fac_loop(int n) {
    int f = 1;
    for (int i=1; i<=n; i++) f *= i;
    return f;
}
// No single formula solution

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

    printf("\n Recur: factorial of %d is: %d \n", N, fac_recur(N));
    printf("\n Loop: factorial %d is: %d \n", N, fac_loop(N));

    return 0;
}
