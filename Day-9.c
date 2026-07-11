
/*
1 Write a program to print the reverse star pattern.
*****
****
***
**
*	
*/
#include <stdio.h>

int main() {
    int i, j;
    for (i = 5; i >= 1; i--) {
        for (j = 1; j <= i; j++) {
            printf("*");
        }
        printf("\n");
    }
    return 0;
}



/*
2 Write a program to print the reverse number triangle.
12345

1234

123

12

1
*/

#include <stdio.h>

int main() {
    int i, j;
    for (i = 5; i >= 1; i--) {
        for (j = 1; j <= i; j++) {
            printf("%d", j);
        }
        printf("\n");
    }
    return 0;
}



/*
3 Write a program to print the repeated character pattern.

A
BB
CCC
DDDD
EEEEE
*/

#include <stdio.h>

int main() {
    int i, j;
    char ch = 'A';
    for (i = 1; i <= 5; i++) {
        for (j = 1; j <= i; j++) {
            printf("%c", ch);
        }
        ch++;
        printf("\n");
    }
    return 0;
}


/*
4 Write a program to print a hollow square pattern.

*****

*   *

*   *

*   *

***** 
*/

#include <stdio.h>

int main() {
    int i, j;
    for (i = 1; i <= 5; i++) {
        for (j = 1; j <= 5; j++) {
            if (i == 1 || i == 5 || j == 1 || j == 5) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    return 0;
}



