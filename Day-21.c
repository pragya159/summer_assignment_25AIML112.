
// Q1 Find String Length without strlen()
#include <stdio.h>

int main() {
    char str;
    int length = 0;
    
    printf("Enter a string: ");
    scanf("%s", str); // Takes a single word input
    
    // Loop until we find the null character '\0' which marks the end of a string
    while (str[length] != '\0') {
        length++;
    }
    
    printf("Length of the string is: %d\n", length);
    
    return 0;
}


// Q2 Reverse a String
#include <stdio.h>

int main() {
    char str, temp;
    int i = 0, j = 0;
    
    printf("Enter a string: ");
    scanf("%s", str);
    
    // First, find the length of the string
    while (str[j] != '\0') {
        j++;
    }
    j--; 
    
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
    
    printf("Reversed string: %s\n", str);
    
    return 0;
}


// Q3 Count Vowels and Consonants
#include <stdio.h>

int main() {
    char str;
    int vowels = 0, consonants = 0, i = 0;
    
    printf("Enter a string: ");
    scanf("%s", str);
    
    while (str[i] != '\0') {
        char ch = str[i];
        
        // Check if it is an alphabet character
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            // Check for vowels
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
                ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U') {
                vowels++;
            } else {
                consonants++;
            }
        }
        i++;
    }
    
    printf("Vowels: %d\n", vowels);
    printf("Consonants: %d\n", consonants);
    
    return 0;
}


// Q4 Convert Lowercase to Uppercase

#include <stdio.h>

int main() {
    char str;
    int i = 0;
    
    printf("Enter a lowercase string: ");
    scanf("%s", str);
    
    while (str[i] != '\0') {
        
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 32;
        }
        i++;
    }
    
    printf("Uppercase string: %s\n", str);
    
    return 0;
}
