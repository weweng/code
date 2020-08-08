#include <stdio.h>
void foo (int *p /*, int &r : not supported in C */)   {
 (*p)++;
}

int main () {

int a = 0, b = 0;
//int &c = b;

foo(&a/*, &b*/);

printf("a=%d, b=%d\n", a,b);

}

