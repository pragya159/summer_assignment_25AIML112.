
// Q1
Create menu-driven calculator

#include <stdio.h>

int main() {
    int choice;
    float n1, n2;
    
    while (1) {
        printf("\n1. Add\n2. Subtract\n3. Multiply\n4. Divide\n5. Exit\nChoice: ");
        scanf("%d", &choice);
        
        if (choice == 5) {
            break;
        }
        
        printf("Enter two numbers: ");
        scanf("%f %f", &n1, &n2);
        
        if (choice == 1) printf("Result: %.2f\n", n1 + n2);
        else if (choice == 2) printf("Result: %.2f\n", n1 - n2);
        else if (choice == 3) printf("Result: %.2f\n", n1 * n2);
        else if (choice == 4) {
            if (n2 == 0) printf("Error! Division by zero.\n");
            else printf("Result: %.2f\n", n1 / n2);
        } else {
            printf("Invalid selection\n");
        }
    }
    
    return 0;
}



// Q2 Create menu-driven array operations system

#include <stdio.h>

int main() {
    int arr, n = 0, choice, i, val;
    
    while (1) {
        printf("\n1. Insert Element\n2. Display Array\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            printf("Enter value: ");
            scanf("%d", &val);
            arr[n] = val;
            n++;
        } else if (choice == 2) {
            printf("Array contents: ");
            for (i = 0; i < n; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");
        } else if (choice == 3) {
            break;
        }
    }
    
    return 0;
}




// Q3Create menu-driven string operations system

#include <stdio.h>
#include <string.h>

int main() {
    char str;
    int choice;
    
    printf("Enter standard string work item: ");
    scanf("%s", str);
    
    while (1) {
        printf("\n1. Print Length\n2. Print Reverse\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            printf("Length: %d\n", (int)strlen(str));
        } else if (choice == 2) {
            printf("Reverse order: ");
            for (int i = strlen(str) - 1; i >= 0; i--) {
                printf("%c", str[i]);
            }
            printf("\n");
        } else if (choice == 3) {
            break;
        }
    }
    
    return 0;
}



// Q4 Create inventory management system

#include <stdio.h>

struct Item {
    int code;
    char name;
    int stock;
};

int main() {
    struct Item store;
    int n = 0, choice;
    
    while (1) {
        printf("\n1. Add Item\n2. View Stock\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            printf("Code: ");
            scanf("%d", &store[n].code);
            printf("Name: ");
            scanf("%s", store[n].name);
            printf("Stock Quantity: ");
            scanf("%d", &store[n].stock);
            n++;
        } else if (choice == 2) {
            for (int i = 0; i < n; i++) {
                printf("Code: %d | Item: %s | Qty: %d\n", store[i].code, store[i].name, store[i].stock);
            }
        } else if (choice == 3) {
            break;
        }
    }
    
    return 0;
}


