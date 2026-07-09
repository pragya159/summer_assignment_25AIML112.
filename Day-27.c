// Q1 Create student record management system

#include <stdio.h>

struct Student {
    int roll;
    char name;
    float marks;
};

int main() {
    int n, i;
    struct Student s;
    
    printf("Enter number of students: ");
    scanf("%d", &n);
    
    for (i = 0; i < n; i++) {
        printf("\nEnter details for Student %d:\n", i + 1);
        printf("Roll Number: ");
        scanf("%d", &s[i].roll);
        printf("Name: ");
        scanf("%s", s[i].name);
        printf("Marks: ");
        scanf("%f", &s[i].marks);
    }
    
    printf("\n--- Student Records ---\n");
    for (i = 0; i < n; i++) {
        printf("Roll: %d, Name: %s, Marks: %.2f\n", s[i].roll, s[i].name, s[i].marks);
    }
    
    return 0;
}


// Q2 Create employee management system

#include <stdio.h>

struct Employee {
    int id;
    char name;
    char dept;
};

int main() {
    int n, i;
    struct Employee emp;
    
    printf("Enter number of employees: ");
    scanf("%d", &n);
    
    for (i = 0; i < n; i++) {
        printf("\nEnter details for Employee %d:\n", i + 1);
        printf("ID: ");
        scanf("%d", &emp[i].id);
        printf("Name: ");
        scanf("%s", emp[i].name);
        printf("Department: ");
        scanf("%s", emp[i].dept);
    }
    
    printf("\n--- Employee List ---\n");
    for (i = 0; i < n; i++) {
        printf("ID: %d | Name: %s | Dept: %s\n", emp[i].id, emp[i].name, emp[i].dept);
    }
    
    return 0;
}



// Q3Create salary management system

#include <stdio.h>

struct Staff {
    char name;
    float basic;
    float allowance;
    float gross;
};

int main() {
    int n, i;
    struct Staff members;
    
    printf("Enter number of staff members: ");
    scanf("%d", &n);
    
    for (i = 0; i < n; i++) {
        printf("\nEnter details for %d:\n", i + 1);
        printf("Name: ");
        scanf("%s", members[i].name);
        printf("Basic Salary: ");
        scanf("%f", &members[i].basic);
        printf("Allowance: ");
        scanf("%f", &members[i].allowance);
        
        members[i].gross = members[i].basic + members[i].allowance;
    }
    
    printf("\n--- Salary Slip Summary ---\n");
    for (i = 0; i < n; i++) {
        printf("Name: %s | Gross Salary: %.2f\n", members[i].name, members[i].gross);
    }
    
    return 0;
}



// Q4 Create marksheet generation system

#include <stdio.h>

int main() {
    char name;
    int m1, m2, m3, total;
    float percentage;
    
    printf("Enter student name: ");
    scanf("%s", name);
    printf("Enter marks for Subject 1, 2, 3: ");
    scanf("%d %d %d", &m1, &m2, &m3);
    
    total = m1 + m2 + m3;
    percentage = total / 3.0;
    
    printf("\n--------------------------\n");
    printf("          MARKSHEET          \n");
    printf("----------------------------\n");
    printf("Name: %s\n", name);
    printf("Subject 1: %d\n", m1);
    printf("Subject 2: %d\n", m2);
    printf("Subject 3: %d\n", m3);
    printf("-----------------------------\n");
    printf("Total: %d | Percentage: %f\n", total, percentage);
    printf("-----------------------------\n");
    
    return 0;
}

