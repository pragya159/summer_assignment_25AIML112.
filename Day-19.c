
// Q1 Add Two Matrices
#include <stdio.h>

int main() {
    int rows, cols, i, j;
    
    printf("Enter number of rows: ");
    scanf("%d", &rows);
    printf("Enter number of columns: ");
    scanf("%d", &cols);
    
    int mat1, mat2, result;
    
    printf("Enter elements for Matrix 1:\n");
    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            scanf("%d", &mat1[i][j]);
        }
    }
    
    printf("Enter elements for Matrix 2:\n");
    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
            printf("Element [%d][%d]: ", i, j);
            scanf("%d", &mat2[i][j]);
        }
    }
    
    // Adding the matrices
    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
    
    printf("\nResult of Addition:\n");
    for(i = 0; i < rows; i++) {
        for(j = 0; j < cols; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}






// Q2 Subtract Two Matrices

#include <stdio.h>

int main() {
    int r, c, i, j;
    
    printf("Enter rows and columns: ");
    scanf("%d %d", &r, &c);
    
    int m1, m2, diff;
    
    printf("Enter Matrix 1 elements:\n");
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            scanf("%d", &m1[i][j]);
        }
    }
    
    printf("Enter Matrix 2 elements:\n");
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            scanf("%d", &m2[i][j]);
        }
    }
    
    // Subtracting m2 from m1
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            diff[i][j] = m1[i][j] - m2[i][j];
        }
    }
    
    printf("\nResult after Subtraction:\n");
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            printf("%d\t", diff[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}






// Q3 Transpose of a Matrix

#include <stdio.h>

int main() {
    int r, c, i, j;
    int matrix, transpose;
    
    printf("Enter rows and columns: ");
    scanf("%d %d", &r, &c);
    
    printf("Enter matrix elements:\n");
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
    
    // Transposing logic: row becomes column, column becomes row
    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            transpose[j][i] = matrix[i][j];
        }
    }
    
    printf("\nTransposed Matrix:\n");
    for(i = 0; i < c; i++) {
        for(j = 0; j < r; j++) {
            printf("%d ", transpose[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}


// Q4 Find Diagonal Sum (Principal Diagonal)

#include <stdio.h>

int main() {
    int n, i, j, diag_sum = 0;
    int mat;
    
    printf("Enter size of square matrix (N): ");
    scanf("%d", &n);
    
    printf("Enter elements:\n");
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            scanf("%d", &mat[i][j]);
        }
    }
    
    // Diagonal elements always have the same row and column index (i == j)
    for(i = 0; i < n; i++) {
        diag_sum = diag_sum + mat[i][i];
    }
    
    printf("Sum of diagonal elements is: %d\n", diag_sum);
    
    return 0;
}

