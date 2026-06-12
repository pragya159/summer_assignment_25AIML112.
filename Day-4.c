"Day-4"

"Write a C program to print the Fibonacci series up to n terms."

#include <stdio.h>

int main() {
    int n, a = 0, b = 1, c, i;
    scanf("%d", &n);
    for(i = 0; i < n; i++) {
        printf("%d ", a);
        c = a + b;
        a = b;
        b = c;
    }
    return 0;
}

"   Write a C program to find the nth term of the Fibonacci series."

#include <stdio.h>

int main() {
    int n, a = 0, b = 1, c, i;
    scanf("%d", &n);
    if(n == 1) {
        printf("%d", a);
    } else if(n == 2) {
        printf("%d", b);
    } else {
        for(i = 3; i <= n; i++) {
            c = a + b;
            a = b;
            b = c;
        }
        printf("%d", b);
    }
    return 0;
}




"Write a program to Check Armstrong number"

#include <stdio.h>

int main() {
    int n, r, s = 0, t;
    scanf("%d", &n);
    t = n;
    while(t > 0) {
        r = t % 10;
        s += r * r * r;
        t /= 10;
    }
    if(s == n)
        printf("Armstrong");
    else
        printf("Not Armstrong");
    return 0;
}


"Write a program to Print Armstrong numbers in a range."

#include <stdio.h>

int main() {
    int l, h, n, t, r, s;
    scanf("%d %d", &l, &h);
    for(n = l; n <= h; n++) {
        s = 0;
        t = n;
        while(t > 0) {
            r = t % 10;
            s += r * r * r;
            t /= 10;
        }
        if(s == n) printf("%d ", n);
    }
    return 0;
}
