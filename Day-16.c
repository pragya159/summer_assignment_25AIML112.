
// Q1 Find Missing Number in Array

#include <stdio.h>

int findMissingNumber(int arr[], int size, int n) {
    
    int expectedSum = n * (n + 1) / 2;
    int actualSum = 0;
    
    for (int i = 0; i < size; i++) {
        actualSum += arr[i];
    }
    
    return expectedSum - actualSum;
}

int main() {
    int arr[] = {1, 2, 4, 5, 6};
    int size = sizeof(arr) / sizeof(arr);
    int n = 6; // Range is 1 to 6
    
    int missing = findMissingNumber(arr, size, n);
    printf("Missing number: %d\n", missing); 
    
    return 0;
}


// Q2 Find Maximum Frequency Element

#include <stdio.h>

void findMaxFrequency(int arr[], int size) {
    int maxElement = arr;
    int maxCount = 0;

    for (int i = 0; i < size; i++) {
        int count = 0;
        for (int j = 0; j < size; j++) {
            if (arr[i] == arr[j]) {
                count++;
            }
        }
        if (count > maxCount) {
            maxCount = count;
            maxElement = arr[i];
        }
    }

    printf("Element: %d, Frequency: %d\n", maxElement, maxCount);
}

int main() {
    int arr[] = {1, 3, 2, 1, 4, 1, 3, 2, 2, 2};
    int size = sizeof(arr) / sizeof(arr);
    
    findMaxFrequency(arr, size); 
    
    return 0;
}




// Q3 Find Pair with Given Sum

#include <stdio.h>

int findPairWithSum(int arr[], int size, int target) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (arr[i] + arr[j] == target) {
                printf("Pair found: (%d, %d)\n", arr[i], arr[j]);
                return 1; 
            }
        }
    }
    printf("No pair found with the given sum.\n");
    return 0;
}

int main() {
    int arr[] = {10, 15, 3, 7};
    int target = 17;
    int size = sizeof(arr) / sizeof(arr);
    
    findPairWithSum(arr, size, target); 
    
    return 0;
}




// Q4 Remove Duplicates from a Sorted Array#include <stdio.h>

int removeDuplicates(int arr[], int size) {
    if (size == 0) return 0;
    
    int writeIndex = 1;
    for (int i = 1; i < size; i++) {
        if (arr[i] != arr[i - 1]) {
            arr[writeIndex] = arr[i];
            writeIndex++;
        }
    }
    return writeIndex; 


int main() {
    int arr[] = {1, 1, 2, 2, 3, 4, 4};
    int size = sizeof(arr) / sizeof(arr);
    
    int newSize = removeDuplicates(arr, size);
    
    printf("Modified array: ");
    for (int i = 0; i < newSize; i++) {
        printf("%d ", arr[i]);
    }
    // Output: 1 2 3 4 
    printf("\n");
    
    return 0;
}
