/*
Q1 Write a program to print a star pyramid.
    *
   ***
  *****
 *******
*********
*/

#include <stdio.h>

int main() {
    int i, j, k;
    for (i = 1; i <= 5; i++) {
        for (j = 1; j <= 5 - i; j++) {
            printf(" ");
        }
        for (k = 1; k <= (2 * i - 1); k++) {
            printf("*");
        }
        printf("\n");
    }
    return 0;
}



/*
Q2 Write a program to print a reverse pyramid.
*********
 *******
  *****
   ***
    *
*/

#include <stdio.h>

int main() {
    int i, j, k;
    for (i = 5; i >= 1; i--) {
        for (j = 1; j <= 5 - i; j++) {
            printf(" ");
        }
        for (k = 1; k <= (2 * i - 1); k++) {
            printf("*");
        }
        printf("\n");
    }
    return 0;
}


/*
Q3 Write a program to print a number pyramid.
    1
   121
  12321
 1234321
123454321
*/

#include <stdio.h>

int main() {
    int i, j, k;
    for (i = 1; i <= 5; i++) {
        for (j = 1; j <= 5 - i; j++) {
            printf(" ");
        }
        for (k = 1; k <= i; k++) {
            printf("%d", k);
        }
        for (k = i - 1; k >= 1; k--) {
            printf("%d", k);
        }
        printf("\n");
    }
    return 0;
}


/*
Q4 Write a program to print a character pyramid.
    A
   ABA
  ABCBA
 ABCDCBA
ABCDEDCBA
*/

#include <stdio.h>

int main() {
    int i, j, k;
    for (i = 1; i <= 5; i++) {
        for (j = 1; j <= 5 - i; j++) {
            printf(" ");
        }
        for (k = 0; k < i; k++) {
            printf("%c", 'A' + k);
        }
        for (k = i - 2; k >= 0; k--) {
            printf("%c", 'A' + k);
        }
        printf("\n");
    }
    return 0;
}

