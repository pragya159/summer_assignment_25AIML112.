
Q1 Write a program to Write function to find sum of two numbers

#include <stdio.h>

int findSum(int a, int b) {
    return a + b;
}

int main() {
    int num1 = 10, num2 = 20;
    int sum = findSum(num1, num2);
    printf("Sum: %d\n", sum);
    return 0;
}

Q2 Write a program to Write function to find maximum

#include <stdio.h>

int findMaximum(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

int main() {
    int num1 = 15, num2 = 25;
    int max = findMaximum(num1, num2);
    printf("Maximum: %d\n", max);
    return 0;
}


Q3 Write a program to Write function to check prime.

#include <stdio.h>

int isPrime(int n) {
    int i;
    if (n <= 1) {
        return 0;
    }
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int num = 17;
    if (isPrime(num)) {
        printf("%d is a prime number\n", num);
    } else {
        printf("%d is not a prime number\n", num);
    }
    return 0;
}


Q4 Write a program to Write function to find factorial.

#include <stdio.h>

long long findFactorial(int n) {
    int i;
    long long fact = 1;
    for (i = 1; i <= n; i++) {
        fact = fact * i;
    }
    return fact;
}

int main() {
    int num = 5;
    long long result = findFactorial(num);
    printf("Factorial of %d is %lld\n", num, result);
    return 0;
}

