"convert Decimal to Binary"

#include <stdio.h>

int main() {
    int num, binary = 0, remainder, place = 1;

    printf("Enter a decimal number: ");
    scanf("%d", &num);

    while (num > 0) {
        remainder = num % 2;
        binary = binary + (remainder * place);
        num = num / 2;
        place = place * 10;
    }

    printf("Binary equivalent: %d\n", binary);

    return 0;
}


"Convert Binary to Decimal"


#include <stdio.h>

int main() {
    int num, decimal = 0, remainder, base = 1;

    printf("Enter a binary number: ");
    scanf("%d", &num);

    while (num > 0) {
        remainder = num % 10;
        decimal = decimal + (remainder * base);
        num = num / 10;
        base = base * 2;
    }

    printf("Decimal equivalent: %d\n", decimal);

    return 0;
}


"Count Set Bits in a Number"

#include <stdio.h>

int main() {
    int num, count = 0;

    printf("Enter a number: ");
    scanf("%d", &num);

    while (num > 0) {
        if (num % 2 == 1) {
            count++;
        }
        num = num / 2;
    }

    printf("Number of set bits: %d\n", count);

    return 0;
}



"Find $x^n$ Without Using pow()"


#include <stdio.h>

int main() {
    int x, n, i;
    long long result = 1;

    printf("Enter base (x): ");
    scanf("%d", &x);
    printf("Enter exponent (n): ");
    scanf("%d", &n);

    for (i = 1; i <= n; i++) {
        result = result * x;
    }

    printf("%d^%d = %lld\n", x, n, result);

    return 0;
}