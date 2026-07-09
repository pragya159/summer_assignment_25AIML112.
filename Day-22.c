// Q1 "Check Palindrome String"

#include <stdio.h>

int main() {
    char str;
    int len = 0, i, is_palindrome = 1; // 1 means true
    
    printf("Enter a string: ");
    scanf("%s", str);
    
    // Calculate length
    while (str[len] != '\0') {
        len++;
    }
    
    // Compare characters from both ends moving inward
    for (i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            is_palindrome = 0; // false
            break;
        }
    }
    
    if (is_palindrome == 1) {
        printf("The string is a palindrome.\n");
    } else {
        printf("The string is NOT a palindrome.\n");
    }
    
    return 0;
}
