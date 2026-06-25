// Q1 Merge Two Arrays

#include <stdio.h>

void mergeArrays(int arr1[], int size1, int arr2[], int size2, int result[]) {
    int i = 0, j = 0, k = 0;

    while (i < size1) {
        result[k] = arr1[i];
        i++;
        k++;
    }

    while (j < size2) {
        result[k] = arr2[j];
        j++;
        k++;
    }
}

int main() {
    int arr1[] = {1, 3, 5};
    int arr2[] = {2, 4, 6};
    int size1 = sizeof(arr1) / sizeof(arr1);
    int size2 = sizeof(arr2) / sizeof(arr2);
    
    int result[size1 + size2];
    
    mergeArrays(arr1, size1, arr2, size2, result);
    
    printf("Merged array: ");
    for (int i = 0; i < size1 + size2; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    return 0;
}




// Q2 Union of Two Arrays (Unique elements from both)

#include <stdio.h>

void printUnion(int arr1[], int size1, int arr2[], int size2) {
    printf("Union: ");
    
    for (int i = 0; i < size1; i++) {
        printf("%d ", arr1[i]);
    }
    
    for (int i = 0; i < size2; i++) {
        int isDuplicate = 0;
        for (int j = 0; j < size1; j++) {
            if (arr2[i] == arr1[j]) {
                isDuplicate = 1;
                break;
            }
        }
        if (isDuplicate == 0) {
            printf("%d ", arr2[i]);
        }
    }
    printf("\n");
}

int main() {
    int arr1[] = {1, 2, 3, 4};
    int arr2[] = {3, 4, 5, 6};
    int size1 = sizeof(arr1) / sizeof(arr1);
    int size2 = sizeof(arr2) / sizeof(arr2);
    
    printUnion(arr1, size1, arr2, size2);
    
    return 0;
}






// Q3 Intersection of Two Arrays (Elements present in both)

#include <stdio.h>

void printIntersection(int arr1[], int size1, int arr2[], int size2) {
    printf("Intersection: ");
    
    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size2; j++) {
            if (arr1[i] == arr2[j]) {
                printf("%d ", arr1[i]);
                break;
            }
        }
    }
    printf("\n");
}

int main() {
    int arr1[] = {1, 2, 3, 4};
    int arr2[] = {3, 4, 5, 6};
    int size1 = sizeof(arr1) / sizeof(arr1);
    int size2 = sizeof(arr2) / sizeof(arr2);
    
    printIntersection(arr1, size1, arr2, size2);
    
    return 0;
}




// Q4 Find Common Elements in Three Arrays

#include <stdio.h>

void findCommon(int arr1[], int size1, int arr2[], int size2, int arr3[], int size3) {
    printf("Common Elements: ");
    
    for (int i = 0; i < size1; i++) {
        int foundInArr2 = 0;
        int foundInArr3 = 0;
        
        for (int j = 0; j < size2; j++) {
            if (arr1[i] == arr2[j]) {
                foundInArr2 = 1;
                break;
            }
        }
        
        for (int k = 0; k < size3; k++) {
            if (arr1[i] == arr3[k]) {
                foundInArr3 = 1;
                break;
            }
        }
        
        if (foundInArr2 && foundInArr3) {
            printf("%d ", arr1[i]);
        }
    }
    printf("\n");
}

int main() {
    int arr1[] = {1, 5, 10, 20};
    int arr2[] = {6, 5, 20, 30};
    int arr3[] = {5, 13, 15, 20};
    
    int size1 = sizeof(arr1) / sizeof(arr1);
    int size2 = sizeof(arr2) / sizeof(arr2);
    int size3 = sizeof(arr3) / sizeof(arr3);
    
    findCommon(arr1, size1, arr2, size2, arr3, size3);
    
    return 0;
}




