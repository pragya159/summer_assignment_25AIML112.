
Q1 Write a program to Write function for palindrome.
#include <stdio.h>

int isPalindrome(int n) {
    int original = n;
    int reversed = 0;
    int remainder;
    while (n > 0) {
        remainder = n % 10;
        reversed = reversed * 10 + remainder;
        n = n / 10;
    }
    if (original == reversed) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    int num = 121;
    if (isPalindrome(num)) {
        printf("%d is a palindrome\n", num);
    } else {
        printf("%d is not a palindrome\n", num);
    }
    return 0;
}





Q2 Write a program to Write function for Armstrong
#include <stdio.h>

int isArmstrong(int n) {
    int original = n;
    int sum = 0;
    int remainder;
    while (n > 0) {
        remainder = n % 10;
        sum = sum + (remainder * remainder * remainder);
        n = n / 10;
    }
    if (original == sum) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    int num = 153;
    if (isArmstrong(num)) {
        printf("%d is an Armstrong number\n", num);
    } else {
        printf("%d is not an Armstrong number\n", num);
    }
    return 0;
}



Q3 Write a program to Write function for Fibonacci.
#include <stdio.h>

void printFibonacci(int terms) {
    int t1 = 0, t2 = 1, nextTerm;
    int i;
    for (i = 1; i <= terms; i++) {
        printf("%d ", t1);
        nextTerm = t1 + t2;
        t1 = t2;
        t2 = nextTerm;
    }
    printf("\n");
}

int main() {
    int terms = 5;
    printf("Fibonacci Series up to %d terms: ", terms);
    printFibonacci(terms);
    return 0;
}



Q4 Write a program to Write function for perfect number
#include <stdio.h>

int isPerfect(int n) {
    int sum = 0;
    int i;
    for (i = 1; i < n; i++) {
        if (n % i == 0) {
            sum = sum + i;
        }
    }
    if (sum == n) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    int num = 28;
    if (isPerfect(num)) {
        printf("%d is a perfect number\n", num);
    } else {
        printf("%d is not a perfect number\n", num);
    }
    return 0;
}

