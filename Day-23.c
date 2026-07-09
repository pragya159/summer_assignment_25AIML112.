// Q1 Find first non-repeating character
#include <stdio.h>

int main() {
    char str;
    int count = {0};
    int i;
    
    printf("Enter string: ");
    scanf("%s", str);
    
    for (i = 0; str[i] != '\0'; i++) {
        count[(unsigned char)str[i]]++;
    }
    
    int found = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (count[(unsigned char)str[i]] == 1) {
            printf("First non-repeating character: %c\n", str[i]);
            found = 1;
            break;
        }
    }
    
    if (found == 0) {
        printf("No non-repeating character found\n");
    }
    
    return 0;
}


// Q2Find first repeating character
#include <stdio.h>

int main() {
    char str;
    int count = {0};
    int i, found = 0;
    
    printf("Enter string: ");
    scanf("%s", str);
    
    for (i = 0; str[i] != '\0'; i++) {
        if (count[(unsigned char)str[i]] == 1) {
            printf("First repeating character: %c\n", str[i]);
            found = 1;
            break;
        }
        count[(unsigned char)str[i]]++;
    }
    
    if (found == 0) {
        printf("No repeating character found\n");
    }
    
    return 0;
}


// Q3 Check anagram strings

#include <stdio.h>

int main() {
    char str1, str2;
    int count1 = {0};
    int count2 = {0};
    int i, isAnagram = 1;
    
    printf("Enter first string: ");
    scanf("%s", str1);
    printf("Enter second string: ");
    scanf("%s", str2);
    
    for (i = 0; str1[i] != '\0'; i++) {
        count1[(unsigned char)str1[i]]++;
    }
    for (i = 0; str2[i] != '\0'; i++) {
        count2[(unsigned char)str2[i]]++;
    }
    
    for (i = 0; i < 256; i++) {
        if (count1[i] != count2[i]) {
            isAnagram = 0;
            break;
        }
    }
    
    if (isAnagram == 1) {
        printf("Strings are anagrams\n");
    } else {
        printf("Strings are not anagrams\n");
    }
    
    return 0;
}



// Q4 Find maximum occurring character
C
#include <stdio.h>

int main() {
    char str;
    int count = {0};
    int i;
    char maxChar;
    int maxCount = 0;
    
    printf("Enter string: ");
    scanf("%s", str);
    
    for (i = 0; str[i] != '\0'; i++) {
        count[(unsigned char)str[i]]++;
    }
    
    for (i = 0; i < 256; i++) {
        if (count[i] > maxCount) {
            maxCount = count[i];
            maxChar = (char)i;
        }
    }
    
    printf("Maximum occurring character: %c (%d times)\n", maxChar, maxCount);
    
    return 0;
}

