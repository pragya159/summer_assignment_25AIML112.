
1. "Recursive Factorial"

#include <stdio.h>

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main() {
    int num = 5;
    printf("Factorial of %d is %d\n", num, factorial(num));
    return 0;
}





2. "Recursive Fibonacci"

#include <stdio.h>

int fibonacci(int n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int term = 6;
    printf("Fibonacci number at position %d is %d\n", term, fibonacci(term));
    return 0;
}


3. "Recursive Sum of Digits"

#include <stdio.h>

int sumOfDigits(int n) {
    if (n == 0) {
        return 0;
    }
    return (n % 10) + sumOfDigits(n / 10);
}

int main() {
    int num = 1234;
    printf("Sum of digits of %d is %d\n", num, sumOfDigits(num));
    return 0;
}


4. "Recursive reverse a number"

#include <stdio.h>

int reverseNumber(int n, int rem) {
    if (n == 0) {
        return rem;
    }
    return reverseNumber(n / 10, rem * 10 + n % 10);
}

int main() {
    int num = 1234;
    printf("Reverse of %d is %d\n", num, reverseNumber(num, 0));
    return 0;
}


