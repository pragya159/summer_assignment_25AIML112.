"1. Sum of digits of a number"

#include<stdio.h>
int main() {
    int n, sum = 0;
    printf("Enter a number: ");
    scanf("%d", &n);
    
    while(n != 0) {
        sum += n % 10;
        n /= 10;
    }
    printf("Sum of digits = %d\n", sum);
    return 0;
}

"2. Reverse a number"

#include<stdio.h>
int main() {
    int n, reverse = 0;
    printf("Enter a number: ");
    scanf("%d", &n);
    
    while(n != 0) {
        reverse = reverse * 10 + n % 10;
        n /= 10;
    }
    printf("Reverse = %d\n", reverse);
    return 0;
}



"3. Product of digits"

#include<stdio.h>
int main() {
    int n, product = 1;
    printf("Enter a number: ");
    scanf("%d", &n);
    
    while(n != 0) {
        product *= n % 10;
        n /= 10;
    }
    printf("Product of digits = %d\n", product);
    return 0;
}



"4. Check if a number is palindrome"

#include<stdio.h>
int main() {
    int n, original, rev = 0;
    printf("Enter a number: ");
    scanf("%d", &n);
    original = n;
    
    while(n != 0) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    
    if(original == rev)
        printf("%d is Palindrome\n", original);
    else
        printf("%d is not Palindrome\n", original);
    return 0;
}