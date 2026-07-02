
// Q1 Multiply Two Matrices

#include <stdio.h>

int main() {
    int r1, c1, r2, c2, i, j, k;
    int m1, m2, res;
    
    printf("Enter rows and columns for Matrix 1: ");
    scanf("%d %d", &r1, &c1);
    printf("Enter rows and columns for Matrix 2: ");
    scanf("%d %d", &r2, &c2);
    
    if(c1 != r2) {
        printf("Multiplication not possible!\n");
        return 0;
    }
    
    printf("Enter Matrix 1 elements:\n");
    for(i = 0; i < r1; i++) {
        for(j = 0; j < c1; j++) {
            scanf("%d", &m1[i][j]);
        }
    }
    
    printf("Enter Matrix 2 elements:\n");
    for(i = 0; i < r2; i++) {
        for(j = 0; j < c2; j++) {
            scanf("%d", &m2[i][j]);
        }
    }
    
    // Multiplying logic
    for(i = 0; i < r1; i++) {
        for(j = 0; j < c2; j++) {
            res[i][j] = 0; // initialize cell to 0 before calculating
            for(k = 0; k < c1; k++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    
    printf("\nResult of Multiplication:\n");
    for(i = 0; i < r1; i++) {
        for(j = 0; j < c2; j++) {
            printf("%d ", res[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}





// Q2 Check Symmetric Matrix

#include <stdio.h>

int main() {
    int n, i, j, is_symmetric = 1; // 1 means true
    int mat;
    
    printf("Enter size of matrix (N): ");
    scanf("%d", &n);
    
    printf("Enter elements:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            scanf("%d", &mat[i][j]);
        }
    }
    
    // Check if mat[i][j] is equal to its transpose mat[j][i]
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            if(mat[i][j] != mat[j][i]) {
                is_symmetric = 0; // false
                break;
            }
        }
    }
    
    if(is_symmetric == 1) {
        printf("The matrix is Symmetric.\n");
    } else {
        printf("The matrix is NOT Symmetric.\n");
    }
    
    return 0;
}



// Q3 Find Row-wise Sum

#include <stdio.h>

int main() {
    int r, c, i, j, row_sum;
    int mat;
    
    printf("Enter rows and columns: ");
    scanf("%d %d", &r, &c);
    
    printf("Enter elements:\n");
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            scanf("%d", &mat[i][j]);
        }
    }
    
    // Row-wise tracking
    for(i = 0; i < r; i++) {
        row_sum = 0;
        for(j = 0; j < c; j++) {
            row_sum += mat[i][j];
        }
        printf("Sum of row %d = %d\n", i + 1, row_sum);
    }
    
    return 0;
}


// Q4 Find Column-wise Sum

#include <stdio.h>

int main() {
    int r, c, i, j, col_sum;
    int mat;
    
    printf("Enter rows and columns: ");
    scanf("%d %d", &r, &c);
    
    printf("Enter elements:\n");
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            scanf("%d", &mat[i][j]);
        }
    }
    
    // Column-wise tracking (Notice the loops: 'j' is outer loop)
    for(j = 0; j < c; j++) {
        col_sum = 0;
        for(i = 0; i < r; i++) {
            col_sum += mat[i][j];
        }
        printf("Sum of column %d = %d\n", j + 1, col_sum);
    }
    
    return 0;
}


