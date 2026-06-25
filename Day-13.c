// Q1 Write a program to Input and display array
#include <stdio.h>

int main() {
    int arr;
    int i;
    printf("Enter 5 elements:\n");
    for (i = 0; i < 5; i++) {
        scanf("%d", &arr[i]);
    }
    printf("The elements are: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}



// Q2 Write a program to Find sum and average of array
#include <stdio.h>

int main() {
    int arr = {10, 20, 30, 40, 50};
    int i;
    int sum = 0;
    float average;
    for (i = 0; i < 5; i++) {
        sum = sum + arr[i];
    }
    average = (float)sum / 5;
    printf("Sum = %d\n", sum);
    printf("Average = %.2f\n", average);
    return 0;
}



// Q3 Write a program to Find largest and smallest element.
#include <stdio.h>

int main() {
    int arr = {25, 11, 7, 49, 36};
    int i;
    int largest = arr;
    int smallest = arr;
    for (i = 1; i < 5; i++) {
        if (arr[i] > largest) {
            largest = arr[i];
        }
        if (arr[i] < smallest) {
            smallest = arr[i];
        }
    }
    printf("Largest element = %d\n", largest);
    printf("Smallest element = %d\n", smallest);
    return 0;
}




// Q4 Write a program to Count even and odd elements
#include <stdio.h>

int main() {
    int arr = {12, 37, 44, 19, 56};
    int i;
    int evenCount = 0;
    int oddCount = 0;
    for (i = 0; i < 5; i++) {
        if (arr[i] % 2 == 0) {
            evenCount++;
        } else {
            oddCount++;
        }
    }
    printf("Total even elements = %d\n", evenCount);
    printf("Total odd elements = %d\n", oddCount);
    return 0;
}

