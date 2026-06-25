// Q1 Write a program to Linear search.
#include <stdio.h>

int main() {
    int arr = {10, 25, 30, 45, 50};
    int key = 30;
    int i;
    int found = -1;
    for (i = 0; i < 5; i++) {
        if (arr[i] == key) {
            found = i;
            break;
        }
    }
    if (found != -1) {
        printf("Element found at index: %d\n", found);
    } else {
        printf("Element not found\n");
    }
    return 0;
}



// Q2 Write a program to Frequency of an element.	
#include <stdio.h>

int main() {
    int arr = {10, 20, 10, 30, 10, 20};
    int key = 10;
    int i;
    int count = 0;
    for (i = 0; i < 6; i++) {
        if (arr[i] == key) {
            count++;
        }
    }
    printf("Frequency of %d is: %d\n", key, count);
    return 0;
}



// Q3 Write a program to Second largest element.	
#include <stdio.h>

int main() {
    int arr = {12, 35, 1, 10, 34};
    int i;
    int largest = arr;
    int secondLargest = -1;
    for (i = 1; i < 5; i++) {
        if (arr[i] > largest) {
            secondLargest = largest;
            largest = arr[i];
        } else if (arr[i] < largest && arr[i] > secondLargest) {
            secondLargest = arr[i];
        }
    }
    printf("Second largest element is: %d\n", secondLargest);
    return 0;
}



// Q4 Write a program to Find duplicates in array.
#include <stdio.h>

int main() {
    int arr = {2, 3, 1, 2, 3, 4};
    int i, j;
    printf("Duplicate elements are: ");
    for (i = 0; i < 6; i++) {
        for (j = i + 1; j < 6; j++) {
            if (arr[i] == arr[j]) {
                printf("%d ", arr[i]);
                break;
            }
        }
    }
    printf("\n");
    return 0;
}

