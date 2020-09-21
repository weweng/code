#include <stdio.h>
#include <stdlib.h>

/* return the index of given key in the array */
int RBinarySearch(int arr[], int l, int h, int key) {
    if (l > h) return -1;

    if (l == h) {
        return (arr[l] == key)? l:-1;
    }

    int m = (l + h)/2;
    if (arr[m] == key) {
        return m;
    } else {
        if (arr[m] > key) {
            h = m - 1;
        }
        if (arr[m] < key) {
            l = m + 1;
        }
        return RBinarySearch(arr, l , h, key);
    }

}

/* return the index of given key in the array */
int IBinarySearch(int arr[], int len, int key) {
    int index = -1;

    int l = 0, h = len-1;

    while (l <= h) { // equal is needed in case the key is the last element
        int m = (l + h)/2;
        if (arr[m] == key) {
            index = m;
            break;
        } else if (arr[m] > key) {
            h = m-1;
        } else {
            l = m+1;
        }
    }

    return index;
}

int main(int argc, char *argv[]) {

    int arr[] = { 1, 3, 5, 13, 17, 21, 25, 28, 30, 32};
    int len = sizeof(arr)/sizeof(int);
    int key = 1;
    int index = IBinarySearch(arr, len, key);
    if ( index >= 0) {
        printf("\n Iterative: key=%d index is %d \n", key, index);
    } else {
        printf("\n Iterative: key=%d is not found \n", key);
    }

    index = RBinarySearch(arr, 0, len-1, key);
    if ( index >= 0) {
        printf("\n Recursive: key=%d index is %d \n", key, index);
    } else {
        printf("\n Recursive: key=%d is not found \n", key);
    }

    return 1;
}
