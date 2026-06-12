" Day-3"

"1- Check whether a number is Prime"


#include<stdio.h>
int main() {
    int n, flag = 1;
    printf("Enter a number: ");
    scanf("%d", &n);
    
    if(n <= 1) flag = 0;
    for(int i = 2; i * i <= n; i++) {
        if(n % i == 0) {
            flag = 0;
            break;
        }
    }
    if(flag)
        printf("%d is Prime\n", n);
    else
        printf("%d is not Prime\n", n);
    return 0;
}


"2-Print Prime numbers in a range"

#include<stdio.h>
int main() {
    int start, end;
    printf("Enter start and end: ");
    scanf("%d %d", &start, &end);
    
    printf("Prime numbers between %d and %d:\n", start, end);
    for(int num = start; num <= end; num++) {
        int flag = 1;
        if(num <= 1) flag = 0;
        for(int i = 2; i * i <= num; i++) {
            if(num % i == 0) {
                flag = 0;
                break;
            }
        }
        if(flag) printf("%d ", num);
    }
    return 0;
}



"3- GCD of two numbers"

#include<stdio.h>
int main() {
    int a, b;
    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);
    
    while(b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    printf("GCD = %d\n", a);
    return 0;
}




"4-LCM of two numbers"

#include<stdio.h>
int main() {
    int a, b, gcd, lcm;
    printf("Enter two numbers: ");
    scanf("%d %d", &a, &b);
    
    int x = a, y = b;
    while(y != 0) {
        int temp = y;
        y = x % y;
        x = temp;
    }
    gcd = x;
    lcm = (a * b) / gcd;
    printf("LCM = %d\n", lcm);
    return 0;
}





"5-Generate Fibonacci Series"

#include<stdio.h>
int main() {
    int n, first = 0, second = 1, next;
    printf("Enter number of terms: ");
    scanf("%d", &n);
    
    printf("Fibonacci Series: ");
    for(int i = 0; i < n; i++) {
        printf("%d ", first);
        next = first + second;
        first = second;
        second = next;
    }
    return 0;
}