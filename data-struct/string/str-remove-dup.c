#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    char str[] = "abcd edf axbyz";

    int i = 0, j= 0, pos = 1 ;
    for (i = 1; i < strlen(str); i++) {
        char pivot = str[i];
        int has_dup = 0;
        for (j = 0; j < i; j++) {
            if (str[j] == pivot) {
                has_dup = 1 ;
                break;
            }
        }
        if (!has_dup) {
            str[pos++] = pivot;
        }
    }
    str[pos] = '\0';
    printf("\n result=%s \n", str);

    return 1;
}
