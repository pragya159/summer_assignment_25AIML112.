// Q1 Create student record system using arrays and strings

#include <stdio.h>

int main() {
    int count;
    char names;
    int rollNo;
    
    printf("How many tracks: ");
    scanf("%d", &count);
    
    for (int i = 0; i < count; i++) {
        printf("Roll no for student %d: ", i + 1);
        scanf("%d", &rollNo[i]);
        printf("Name for student %d: ", i + 1);
        scanf("%s", names[i]);
    }
    
    printf("\nOutput Register:\n");
    for (int i = 0; i < count; i++) {
        printf("[%d] Name reference: %s\n", rollNo[i], names[i]);
    }
    
    return 0;
}




// Q2 Create mini library system

#include <stdio.h>

int main() {
    char basicTitles = {"C Programming", "Data Structures", "Core Java"};
    int trackedStatus = {1, 1, 1};
    int selectId;
    
    printf("Available books:\n");
    for (int i = 0; i < 3; i++) {
        printf("ID %d : %s Status(%d)\n", i, basicTitles[i], trackedStatus[i]);
    }
    
    printf("Enter book ID to checkout: ");
    scanf("%d", &selectId);
    
    if (selectId >= 0 && selectId < 3) {
        if (trackedStatus[selectId] == 1) {
            trackedStatus[selectId] = 0;
            printf("Success! You checked out: %s\n", basicTitles[selectId]);
        } else {
            printf("Book is currently unavailable.\n");
        }
    } else {
        printf("Wrong ID selection.\n");
    }
    
    return 0;
}



// Q3 Create mini employee management system

#include <stdio.h>
#include <string.h>

int main() {
    char ids = {"E01", "E02", "E03"};
    char names = {"Amit", "Rahul", "Pooja"};
    char searchId;
    int found = 0;
    
    printf("Enter Employee ID to fetch data: ");
    scanf("%s", searchId);
    
    for (int i = 0; i < 3; i++) {
        if (strcmp(ids[i], searchId) == 0) {
            printf("Match Found -> Name: %s\n", names[i]);
            found = 1;
            break;
        }
    }
    
    if (found == 0) {
        printf("Record does not exist.\n");
    }
    
    return 0;
}



// Q4 
Develop complete mini project using arrays, strings and functions

#include <stdio.h>

void showAppMessage() {
    printf("--- Central Data Processing App ---\n");
}

int calculateSum(int data[], int limit) {
    int totalSum = 0;
    for (int i = 0; i < limit; i++) {
        totalSum += data[i];
    }
    return totalSum;
}

int main() {
    int readings;
    int size = 5;
    
    showAppMessage();
    
    printf("Enter %d numbers:\n", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &readings[i]);
    }
    
    int computation = calculateSum(readings, size);
    printf("Total calculated value output is: %d\n", computation);
    
    return 0;
}
