// Q1 Merge two sorted arrays

#include <stdio.h>

int main() {
    int n1, n2, i, j, k;
    int arr1, arr2, res;
    
    printf("Enter size of first sorted array: ");
    scanf("%d", &n1);
    printf("Enter elements: ");
    for (i = 0; i < n1; i++) {
        scanf("%d", &arr1[i]);
    }
    
    printf("Enter size of second sorted array: ");
    scanf("%d", &n2);
    printf("Enter elements: ");
    for (i = 0; i < n2; i++) {
        scanf("%d", &arr2[i]);
    }
    
    i = 0; j = 0; k = 0;
    while (i < n1 && j < n2) {
        if (arr1[i] < arr2[j]) {
            res[k] = arr1[i];
            i++;
        } else {
            res[k] = arr2[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        res[k] = arr1[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        res[k] = arr2[j];
        j++;
        k++;
    }
    
    printf("Merged sorted array: ");
    for (i = 0; i < n1 + n2; i++) {
        printf("%d ", res[i]);
    }
    printf("\n");
    
    return 0;
}



// Q2 Find common characters in strings

#include <stdio.h>

int main() {
    char str1, str2;
    int count1 = {0};
    int count2 = {0};
    int i;
    
    printf("Enter first string: ");
    scanf("%s", str1);
    printf("Enter second string: ");
    scanf("%s", str2);
    
    for (i = 0; str1[i] != '\0'; i++) {
        count1[(unsigned char)str1[i]] = 1;
    }
    for (i = 0; str2[i] != '\0'; i++) {
        count2[(unsigned char)str2[i]] = 1;
    }
    
    printf("Common characters: ");
    for (i = 0; i < 256; i++) {
        if (count1[i] == 1 && count2[i] == 1) {
            printf("%c ", i);
        }
    }
    printf("\n");
    
    return 0;
}


// Q3 Sort names alphabetically

#include <stdio.h>
#include <string.h>

int main() {
    int n, i, j;
    char names, temp;
    
    printf("Enter number of names: ");
    scanf("%d", &n);
    
    printf("Enter names:\n");
    for (i = 0; i < n; i++) {
        scanf("%s", names[i]);
    }
    
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                strcpy(temp, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], temp);
            }
        }
    }
    
    printf("\nSorted names:\n");
    for (i = 0; i < n; i++) {
        printf("%s\n", names[i]);
    }
    
    return 0;
}



// Q4 Sort words by length
C
#include <stdio.h>
#include <string.h>

int main() {
    int n, i, j;
    char words, temp;
    
    printf("Enter number of words: ");
    scanf("%d", &n);
    
    printf("Enter words:\n");
    for (i = 0; i < n; i++) {
        scanf("%s", words[i]);
    }
    
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (strlen(words[i]) > strlen(words[j])) {
                strcpy(temp, words[i]);
                strcpy(words[i], words[j]);
                strcpy(words[j], temp);
            }
        }
    }
    
    printf("\nWords sorted by length:\n");
    for (i = 0; i < n; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}
