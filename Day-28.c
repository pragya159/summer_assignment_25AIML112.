// Q1Create library management system

#include <stdio.h>

struct Book {
    int bno;
    char title;
    char author;
};

int main() {
    struct Book lib;
    int count = 0, choice;
    
    while (1) {
        printf("\n1. Add Book\n2. Display All\n3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            printf("Enter book number: ");
            scanf("%d", &lib[count].bno);
            printf("Enter title: ");
            scanf("%s", lib[count].title);
            printf("Enter author: ");
            scanf("%s", lib[count].author);
            count++;
        } else if (choice == 2) {
            printf("\n--- Library Books ---\n");
            for (int i = 0; i < count; i++) {
                printf("No: %d, Title: %s, Author: %s\n", lib[i].bno, lib[i].title, lib[i].author);
            }
        } else if (choice == 3) {
            break;
        } else {
            printf("Invalid input\n");
        }
    }
    
    return 0;
}


// Q2 Create bank account system

#include <stdio.h>

struct Account {
    int accNo;
    char holder;
    float bal;
};

int main() {
    struct Account user;
    
    printf("Create Account:\nEnter Account Number: ");
    scanf("%d", &user.accNo);
    printf("Enter Holder Name: ");
    scanf("%s", user.holder);
    printf("Enter Initial Balance: ");
    scanf("%f", &user.bal);
    
    printf("\nAccount created successfully!\n");
    printf("Number: %d | Name: %s | Balance: %.2f\n", user.accNo, user.holder, user.bal);
    
    return 0;
}




// Q3 Create ticket booking system

#include <stdio.h>

int main() {
    int available = 20;
    int choice, req;
    
    while (1) {
        printf("\n1. Book Tickets\n2. Exit\nChoice: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            printf("Available seats: %d\nEnter tickets needed: ", available);
            scanf("%d", &req);
            if (req <= available) {
                available -= req;
                printf("Booking Confirmed! %d seats booked.\n", req);
            } else {
                printf("Not enough seats available!\n");
            }
        } else if (choice == 2) {
            break;
        } else {
            printf("Wrong option\n");
        }
    }
    
    return 0;
}



// Q4Create contact management system

#include <stdio.h>

struct Contact {
    char name;
    char phone;
};

int main() {
    struct Contact directory;
    int count = 0, choice;
    
    while (1) {
        printf("\n1. Save Contact\n2. Show Contacts\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        
        if (choice == 1) {
            printf("Enter name: ");
            scanf("%s", directory[count].name);
            printf("Enter phone number: ");
            scanf("%s", directory[count].phone);
            count++;
        } else if (choice == 2) {
            printf("\n--- Saved Contacts ---\n");
            for (int i = 0; i < count; i++) {
                printf("%s : %s\n", directory[i].name, directory[i].phone);
            }
        } else if (choice == 3) {
            break;
        }
    }
    
    return 0;
}
