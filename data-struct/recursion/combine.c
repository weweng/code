#include <stdio.h>
#include <stdlib.h>


//Recurse: use pascal triangle C(n,r) = C(n-1,r-1) + C(n-1,r)
// time complexity O(pow(2,n)), space complexity O(n)
int comb_rec(int n, int r) {
    if (r == 0 || n == r) return 1; 
    return comb_rec(n-1, r-1) + comb_rec(n-1,r);
}

//Loop: time complexity O(n), space complexity O(1)
// C(n,r) = n!/(r!*(n-r)!)
int comb_loop(int n, int r) {
    unsigned long f_n, f_r, f_n_r;
    unsigned long tmp = 1;
    
    for (int i = 1; i <= n;i++) {
	tmp *=i;
	if (i == r) f_r = tmp;
	if (i == (n-r)) f_n_r = tmp;
	if (i == n) f_n = tmp;
    }

    return f_n/(f_r * f_n_r);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
	printf("\n Please provide two positive integer number.");
	return -1;
    }
    int n = atoi(argv[1]);
    int r = atoi(argv[2]);
    if (n < 0 || r < 0) {
	printf("\n Please provide valid positive integer number.");
	return -2;
    }

    unsigned long t0 = __builtin_ia32_rdtsc();
    int comb_r = comb_rec(n, r);
    unsigned long t1 = __builtin_ia32_rdtsc();
    int comb_l = comb_loop(n, r);
    unsigned long t2 = __builtin_ia32_rdtsc();
    printf("\n Recur: comb(%d, %d)=%d, time=%lu \n", n,r, comb_r, t1-t0);
    printf("\n Loop:  comb(%d, %d)=%d, time=%lu \n", n,r, comb_l, t2-t1);

    return 0;
}

    /* test log:
       weng@weng-u1604:/mnt/vb-win7-share/github/code/data-struct/recursion$ ./a.out  20 7

       Recur: comb(20, 7)=77520, time=1617946 

       Loop:  comb(20, 7)=77520, time=198 
       weng@weng-u1604:/mnt/vb-win7-share/github/code/data-struct/recursion$ ./a.out  10  4

       Recur: comb(10, 4)=210, time=5000 

       Loop:  comb(10, 4)=210, time=260 
       weng@weng-u1604:/mnt/vb-win7-share/github/code/data-struct/recursion$ ./a.out  20 7
     */
