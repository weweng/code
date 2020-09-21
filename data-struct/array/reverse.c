#include <stdio.h>
#include <stdlib.h>

/* reverse array */
void reverse(int arr[], int len) {
    int l = 0, h = len-1;

    while (l < h) {
        int tmp = arr[l];
        arr[l] = arr[h];
        arr[h] = tmp;
        l++;h--;
    }
}

int main(int argc, char *argv[]) {

    int arr[] = { 1, 3, 5, 13, 17, 21, 25, 28, 30, 32};
    int len = sizeof(arr)/sizeof(int);
    reverse(arr, len);
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 1;
}
