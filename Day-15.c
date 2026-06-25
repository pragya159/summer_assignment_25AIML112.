// Q1 Write a program to Reverse array.
#include <stdio.h>

int main() {
    int arr = {1, 2, 3, 4, 5};
    int i;
    int start = 0;
    int end = 4;
    int temp;
    while (start < end) {
        temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
    printf("Reversed array: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}



// Q2 Write a program to Rotate array left.

#include <stdio.h>

int main() {
    int arr = {1, 2, 3, 4, 5};
    int i;
    int first = arr;
    for (i = 0; i < 4; i++) {
        arr[i] = arr[i + 1];
    }
    arr = first;
    printf("Left rotated array: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}



// Q3 Write a program to Rotate array right.
#include <stdio.h>

int main() {
    int arr = {1, 2, 3, 4, 5};
    int i;
    int last = arr;
    for (i = 4; i > 0; i--) {
        arr[i] = arr[i - 1];
    }
    arr = last;
    printf("Right rotated array: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}


// Q4 Write a program to Move zeroes to end.
#include <stdio.h>

int main() {
    int arr = {1, 0, 2, 0, 3, 4};
    int i;
    int count = 0;
    int temp;
    for (i = 0; i < 6; i++) {
        if (arr[i] != 0) {
            temp = arr[i];
            arr[i] = arr[count];
            arr[count] = temp;
            count++;
        }
    }
    printf("Array after moving zeroes: ");
    for (i = 0; i < 6; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
