#include <stdio.h>
#include <stdlib.h>


//Recurse: time complexity O(n), space complexity O(n)
int pow_recur(int m, int n) {
    if (n == 0) return 1; 
    return pow_recur(m,n-1)*m;
}
//Loop: time complexity O(n), space complexity O(1)
int pow_loop(int m, int n) {
    int p = 1;
    for (int i=1; i<=n; i++) p *= m;
    return p;
}
// Optimized recurse version, put some work
// during compile time, run time cut by about half
int pow2_recur(int m, int n) {
    pow2_count++;
    if (n == 0) return 1;
    if (n%2 == 0) 
        return pow2_recur(m*m,n/2);
    else
        return pow2_recur(m*m, n/2)*m;
}
// cut run time by ~75%
int pow4_recur(int m, int n) {
    pow4_count++;
    if (n == 0) return 1;
    
    if (n%4 == 0)     
        return pow4_recur(m*m*m*m,n/4);
    else if (n%4 == 1)  
        return pow4_recur(m*m*m*m, n/4)*m;
    else if (n%4 == 2)  
        return pow4_recur(m*m*m*m, n/4)*m*m;
    else if (n%4 == 3)  
        return pow4_recur(m*m*m*m, n/4)*m*m*m; 	
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
	printf("\n Please provide two positive integer number.");
	return -1;
    }
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    if (n < 0 || m <= 0) {
	printf("\n Please provide valid positive integer number.");
	return -2;
    }

    unsigned long t0 = __builtin_ia32_rdtsc();
    int p_loop = pow_loop(m, n);
    unsigned long t1 = __builtin_ia32_rdtsc();
    int p_recur = pow_recur(m, n);
    unsigned long t2 = __builtin_ia32_rdtsc();
    int p2_recur = pow2_recur(m, n);
    unsigned long t3 = __builtin_ia32_rdtsc();
    int p4_recur = pow4_recur(m, n);
    unsigned long t4 = __builtin_ia32_rdtsc();
    
    printf("\n Loop: Power(%d,%d) is: %d \n", m, n, p_loop);
    printf("\n Recur: Power(%d,%d) is: %d \n", m, n, p_recur);
    printf("\n Recur2: Power(%d,%d) is: %d \n", m, n, p2_recur);
    printf("\n Recur4: Power(%d,%d) is: %d \n", m, n, p4_recur);

    printf("\n Time elapsed:  loop=%lu, recur=%lu, recur2=%lu, recu4=%lu \n", t1-t0, t2-t1, t3-t2, t4-t3);
    return 0;
}
