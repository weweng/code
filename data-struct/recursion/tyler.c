#include <stdio.h>
#include <stdlib.h>

// tyler series
// e(x) = f(x,n) = 1 + x/1 + x2/2! + x3/3! + x4/4! + ... + xn/n! +

//utility function
int power(int x, int n) {
    int p = 1;
    for (int i = 0; i < n; i++) p *=x;
    return p;
}
int fact(int n) {
    int f = 1;
    for (int i = 1; i <= n; i++) f *=i;
    return f;
}

//Recurse: time complexity O(n), space complexity O(n)
float tyler_recur_1(int n, int x) {
    if (n == 0) return 1.0; 

    // head recursion, returning task
    return tyler_recur_1(n-1, x) + (float)power(x,n)/fact(n);
}

// optimizing the power and factorial to save number of multiplications
float tyler_recur_2(int n, int x) {
    static float p = 1 , f = 1;
    float r;
    if (n == 0) return 1.0; 

    // head recursion, we need keep the return result as tmp
    r = tyler_recur_2(n-1, x);
    // using static variable to update power and factorial for
    // part of return and next use as citical optimization
    p *= x; f *= n;
    
    return r + (double)p/f;
}

float tyler_recur_horner(int n, int x) {
    static float s = 1.0;
    if (n == 0) return s; // return s is the key
    s = 1.0 + ((double)x/n)*s; 
    return tyler_recur_horner(n-1, x);
}

//Loop: time complexity O(n), space complexity O(1)

float tyler_loop(int n, int x) {
    float s = 1.0;
    for (int i=1; i<=n; i++) {
	s += (float)power(x,i)/fact(i);
    }
    return s;
}


//Loop: time complexity O(n), space complexity O(1)
float tyler_loop_e(int n, int x) {
    float s = 1.0;
    float tmp = 1.0;
    for (int i=1; i<=n; i++) {
	tmp = tmp*x/i;
        s += tmp;
    }
    return s;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
	printf("\n Please provide two positive integer number.");
	return -1;
    }
    int x = atoi(argv[1]); // e power of X
    int n = atoi(argv[2]); // precesion 
    if (n < 0 || x <= 0) {
	printf("\n Please provide valid positive integer number.");
	return -2;
    }

    unsigned long t0_ = __builtin_ia32_rdtsc();
    float t_rec_1 = tyler_recur_1(n, x);
    unsigned long t0 = __builtin_ia32_rdtsc();
    float t_rec_2 = tyler_recur_2(n, x);
    unsigned long t1 = __builtin_ia32_rdtsc();
    float t_rec_h = tyler_recur_horner(n, x);
    unsigned long t2 = __builtin_ia32_rdtsc();
    float t_loop = tyler_loop(n, x);
    unsigned long t3 = __builtin_ia32_rdtsc();
    float t_loop_e = tyler_loop_e(n, x);
    unsigned long t4 = __builtin_ia32_rdtsc();

    printf("\n Recur: tyler_recur1(%d,%d)=%f , time=%lu \n", n, x, t_rec_1, (t0-t0_) );
    printf("\n Recur: tyler_recur2(%d,%d)=%f , time=%lu \n", n, x, t_rec_2, (t1-t0) );
    printf("\n Recur: tyler_recur_horner(%d,%d)=%f, time=%lu \n", n, x, t_rec_h, (t2-t1));
    printf("\n Loop: tyler_loop(%d,%d)=%f time=%lu \n", n, x, t_loop, (t3-t2));
    printf("\n Loop: tyler_loop_e(%d,%d)=%f time=%lu \n", n, x, t_loop_e, (t4-t3));

    /*
      weng@weng-u1604:/mnt/vb-win7-share/github/code/data-struct/recursion$ ./a.out 2 10

      Recur: tyler_recur1(10,2)=7.388995 , time=1312 

      Recur: tyler_recur2(10,2)=7.388995 , time=338 

      Recur: tyler_recur_horner(10,2)=7.388995, time=310 

      Loop: tyler_loop(10,2)=7.388995 time=1030 

      Loop: tyler_loop_e(10,2)=7.388995 time=210 
      weng@weng-u1604:/mnt/vb-win7-share/github/code/data-struct/recursion$ 
    
     */
    return 0;
}
