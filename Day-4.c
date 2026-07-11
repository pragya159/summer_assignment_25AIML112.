// Q1	Write a program to Generate Fibonacci series 
   #include <stdio.h>

int main() {
    int n, a = 0, b = 1, c, i;

    printf("Enter number: ");
    scanf("%d", &n);

    printf("%d %d ", a, b);

    for (i = 3; i <= n; i++) {
        c = a + b;
        printf("%d ", c);
        a = b;
        b = c;
    }

    return 0;
}


// Q2  Write a program to Find nth Fibonacci term

#include <stdio.h>

int main() {
    int n, a = 0, b = 1, c, i;

    printf("Enter position: ");
    scanf("%d", &n);

    if (n == 1) {
        printf("%d", a);
    } else if (n == 2) {
        printf("%d", b);
    } else {
        for (i = 3; i <= n; i++) {
            c = a + b;
            a = b;
            b = c;
        }
        printf("%d", c);
    }

    return 0;
}



// Q3   Write a program to Check Armstrong number.

#include <stdio.h>

int main() {
    int num, temp1, temp2, rem, count = 0, sum = 0, i, mul;

    printf("Enter number: ");
    scanf("%d", &num);

    temp1 = num;
    while (temp1 > 0) {
        temp1 = temp1 / 10;
        count++;
    }

    temp2 = num;
    while (temp2 > 0) {
        rem = temp2 % 10;
        
        mul = 1;
        for (i = 0; i < count; i++) {
            mul = mul * rem;
        }
        
        sum = sum + mul;
        temp2 = temp2 / 10;
    }

    if (sum == num) {
        printf("Yes");
    } else {
        printf("No");
    }

    return 0;
}


// Q4   Write a program to Print Armstrong numbers in a range.


#include <stdio.h>

int main() {
    int start, end, num, temp1, temp2, rem, count, sum, i, mul;

    printf("Enter start: ");
    scanf("%d", &start);

    printf("Enter end: ");
    scanf("%d", &end);

    for (num = start; num <= end; num++) {
        temp1 = num;
        count = 0;
        while (temp1 > 0) {
            temp1 = temp1 / 10;
            count = count + 1;
        }

        temp2 = num;
        sum = 0;
        while (temp2 > 0) {
            rem = temp2 % 10;

            mul = 1;
            for (i = 0; i < count; i++) {
                mul = mul * rem;
            }

            sum = sum + mul;
            temp2 = temp2 / 10;
        }

        if (sum == num) {
            printf("%d ", num);
        }
    }

    return 0;
}
