#include <stdio.h>
#include <stdlib.h>

//Recurse: time complexity O(n), space complexity O(n)
// risk of stack overflow, e.g. N=1,000,000
int sum_recur(int n) {
    if (n <= 0) return 0; 
    return sum_recur(n-1) + n;
}
//Loop: time complexity O(n), space complexity O(1)
int sum_loop(int n) {
    int s = 0;
    for (int i=0; i<=n; i++) s+=i;
    return s;
}
//Formula: time complexity O(1), space complexity O(1)
//could possibly overflow even the sum is within range of max INT
// if not using long long integer, e.g. N=1,000,000
int sum_formula(int n) {
    return (unsigned long long)n*(n+1)/2;
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

    printf("\n Recur: Sum of 1 to %d is: %d \n", N, sum_recur(N));
    printf("\n Loop: Sum of 1 to %d is: %d \n", N, sum_loop(N));
    printf("\n Formular: Sum of 1 to %d is: %d \n", N, sum_formula(N));

    return 0;
}
