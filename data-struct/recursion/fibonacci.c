#include <stdio.h>
#include <stdlib.h>


// F(n) = F(n-1) + F(n-2)
// F((n) = n when n=0, 1
// 0, 1, 1, 3, 4, 7
// Formula: F(n) = (power(Phi, n) - power(phi, n))/sqrt(5)
//   Phi=(1+sqrt(5))/2, phi=(1-sqrt(5))/2
//

//Recurse: time complexity O(pow(2,n), space complexity O(n)
int fib_rec(int n) {  
    if (n <=1) return n;
    return fib_rec(n-1) + fib_rec(n-2);
}

// fibonacci enhanced version
// since there are a lot repeat computation, e.g. fib_rec(n-1) will do
// fib_rec(n-2), fib_rec(n-3,.. fib(0), when we do fib_rec(n-2) we don't have to do it
// if it has been cached while it was done first time, this would save significantly
// the pre-requsite is that, before call this, it need preapre enough space for cache
//Recurse: time complexity O(n), space complexity O(n)
static int *fib_cache;
int fib_rec_e(int n) {
    if (n <=1) {
	fib_cache[n] = n;
	return n;
    }
    if (!fib_cache[n])  fib_cache[n] = fib_rec_e(n-1) + fib_rec_e(n-2);

    return fib_cache[n];
}

/*
 * Using two helper variables during loop
 * This is most efficient way: No functio call overhead, no repeat operation!
 */
//Recurse: time complexity O(n), space complexity O(1)
int fib_loop(int N) {
    int n_1 = 1, n_2 = 0;
    int r;
    
    if (N <=1) return N;
    for (int i = 2; i <= N; i++) {
	r = n_1 + n_2;
	n_2 = n_1; n_1 = r;
    }

    return r;
}
// Formula: F(n) = (power(Phi, n) - power(phi, n))/sqrt(5)
//   Phi=(1+sqrt(5))/2, phi=(1-sqrt(5))/2
//
// Although we have exact formular, but we can't use it as it needs
// floating point computation
int fib_form(int N) {
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
	printf("\n Please provide two positive integer number.");
	return -1;
    }
    int n = atoi(argv[1]);
    if (n < 0 ) {
	printf("\n Please provide valid positive integer number.");
	return -2;
    }

    fib_cache = calloc(n+1,sizeof(int)); // we cover from 0->n, inclusive

    unsigned long t0 = __builtin_ia32_rdtsc();
    int fib_r = fib_rec(n);
    unsigned long t1 = __builtin_ia32_rdtsc();
    int fib_r_e = fib_rec_e(n);
    unsigned long t2 = __builtin_ia32_rdtsc();
    
    int fib_l = fib_loop(n);
    unsigned long t3 = __builtin_ia32_rdtsc();
    printf("\n Recur: fib(%d)=%d, time=%lu \n", n, fib_r, t1-t0);
    printf("\n Recur_e: fib(%d)=%d, time=%lu \n", n, fib_r_e, t2-t1);
    printf("\n Loop: fib(%d)=%d, time=%lu \n", n, fib_l, t3-t2);

    free(fib_cache);

    return 0;
}

    /* test log:
      weng@weng-u1604:/mnt/vb-win7-share/github/code/data-struct/recursion$ ./a.out 10

      Recur: fib(10)=55, time=2152 

      Recur_e: fib(10)=55, time=348 

      Loop: fib(10)=55, time=146 
      weng@weng-u1604:/mnt/vb-win7-share/github/code/data-struct/recursion$ ./a.out 20

      Recur: fib(20)=6765, time=349846     O(2,n) is shown here!!!!

      Recur_e: fib(20)=6765, time=986     O(n)

      Loop: fib(20)=6765, time=230       O(n) without function call overhead
      weng@weng-u1604:/mnt/vb-win7-share/github/code/data-struct/recursion$ 

     */
