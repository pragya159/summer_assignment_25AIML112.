// Q1 Create number guessing game

#include <stdio.h>

int main() {
    int target = 45; 
    int guess;
    
    printf("Guess a number between 1 and 100: ");
    
    while (1) {
        scanf("%d", &guess);
        if (guess > target) {
            printf("Too high! Try again: ");
        } else if (guess < target) {
            printf("Too low! Try again: ");
        } else {
            printf("Correct! You guessed it.\n");
            break;
        }
    }
    
    return 0;
}



// Q2 Create voting eligibility system

#include <stdio.h>

int main() {
    int age;
    
    printf("Enter your age: ");
    scanf("%d", &age);
    
    if (age >= 18) {
        printf("You are eligible to vote.\n");
    } else {
        printf("You are not eligible to vote. Wait %d more year(s).\n", 18 - age);
    }
    
    return 0;
}



// Q3 Create ATM simulation

#include <stdio.h>

int main() {
    int choice;
    int balance = 5000, amount;
    
    while (1) {
        printf("\n1. Check Balance\n2. Deposit\n3. Withdraw\n4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", choice);
        
        if (choice == 1) {
            printf("Current Balance: Rs. %d\n", balance);
        } else if (choice == 2) {
            printf("Enter amount to deposit: ");
            scanf("%d", &amount);
            balance += amount;
            printf("Successfully deposited.\n");
        } else if (choice == 3) {
            printf("Enter amount to withdraw: ");
            scanf("%d", &amount);
            if (amount > balance) {
                printf("Insufficient balance!\n");
            } else {
                balance -= amount;
                printf("Successfully withdrawn.\n");
            }
        } else if (choice == 4) {
            printf("Thank you for using the ATM.\n");
            break;
        } else {
            printf("Invalid choice!\n");
        }
    }
    
    return 0;
}




// Q4 reate quiz application

#include <stdio.h>

int main() {
    int ans1, ans2;
    int score = 0;
    
    printf("Q1. What is the size of int in standard C?\n1. 1 byte\n2. 2 or 4 bytes\n3. 8 bytes\n");
    printf("Your answer: ");
    scanf("%d", &ans1);
    if (ans1 == 2) {
        score += 10;
    }
    
    printf("\nQ2. Which symbol is used for addresses?\n1. &\n2. *\n3. $\n");
    printf("Your answer: ");
    scanf("%d", &ans2);
    if (ans2 == 1) {
        score += 10;
    }
    
    printf("\nQuiz Finished! Your total score is: %d/20\n", score);
    
    return 0;
}
