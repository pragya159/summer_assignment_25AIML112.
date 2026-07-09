// Q1 Check string rotation

#include <stdio.h>
#include <string.h>

int main() {
    char str1, str2, temp;
    
    printf("Enter first string: ");
    scanf("%s", str1);
    printf("Enter second string: ");
    scanf("%s", str2);
    
    if (strlen(str1) != strlen(str2)) {
        printf("Not a rotation\n");
        return 0;
    }
    
    strcpy(temp, str1);
    strcat(temp, str1);
    
    if (strstr(temp, str2) != NULL) {
        printf("String is a rotation\n");
    } else {
        printf("Not a rotation\n");
    }
    
    return 0;
}


// Q2 Compress a string

#include <stdio.h>

int main() {
    char str;
    int i, count = 1;
    
    printf("Enter string: ");
    scanf("%s", str);
    
    printf("Compressed string: ");
    for (i = 0; str[i] != '\0'; i++) {
        if (str[i] == str[i + 1]) {
            count++;
        } else {
            printf("%c%d", str[i], count);
            count = 1;
        }
    }
    printf("\n");
    
    return 0;
}



// Q3 Find longest word

#include <stdio.h>

int main() {
    char str;
    int i = 0, len = 0, maxLen = 0, maxIdx = 0;
    
    printf("Enter a sentence: ");
    scanf(" %[^\n]", str);
    
    while (1) {
        if (str[i] == ' ' || str[i] == '\0') {
            if (len > maxLen) {
                maxLen = len;
                maxIdx = i - len;
            }
            len = 0;
            if (str[i] == '\0') {
                break;
            }
        } else {
            len++;
        }
        i++;
    }
    
    printf("Longest word: ");
    for (i = maxIdx; i < maxIdx + maxLen; i++) {
        printf("%c", str[i]);
    }
    printf("\nLength: %d\n", maxLen);
    
    return 0;
}


// Q4 Remove duplicate characters

#include <stdio.h>

int main() {
    char str, res;
    int count = {0};
    int i, j = 0;
    
    printf("Enter string: ");
    scanf("%s", str);
    
    for (i = 0; str[i] != '\0'; i++) {
        if (count[(unsigned char)str[i]] == 0) {
            count[(unsigned char)str[i]] = 1;
            res[j] = str[i];
            j++;
        }
    }
    res[j] = '\0';
    
    printf("String after removing duplicates: %s\n", res);
    
    return 0;
}
