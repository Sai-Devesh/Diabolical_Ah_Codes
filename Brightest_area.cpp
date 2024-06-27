#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 100
#define MAX_COLS 100

int max(int a, int b) {
    return (a > b) ? a : b;
}

void maxSubArraySum(int a[], int size, int* maxSum, int* top, int* bottom) {
    int** a1 = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        a1[i] = (int*)malloc(2 * sizeof(int));
    }
    a1[0][1] = a[0];
    a1[0][0] = 0;
    int j = 0;
    *maxSum = a1[0][1]; 
    *top = *bottom = 0;

    for (int i = 1; i < size; i++) {
        int sum = a[i] + a1[i - 1][1];
        if (a[i] > sum) {
            j = i;
            a1[i][1] = a[i];
            a1[i][0] = j;
        } else {
            j = a1[i - 1][0];
            a1[i][1] = sum;
            a1[i][0] = a1[i - 1][0]; 
        }
        if (a1[i][1] > *maxSum) {
            *maxSum = a1[i][1];
            *top = a1[j][0];
            *bottom = i;
        }
    }

    for (int i = 0; i < size; i++) {
        free(a1[i]);
    }
    free(a1);
}

void maxSubmatrixSum(int A[MAX_ROWS][MAX_COLS], int r, int c) {
    int** mat = (int**)malloc(r * sizeof(int*));
    for (int i = 0; i < r; i++) {
        mat[i] = (int*)malloc(c * sizeof(int));
        for (int j = 0; j < c; j++) {
            mat[i][j] = 0;
        }
    }

    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (j == 0)
                mat[i][j] = A[i][j];
            else
                mat[i][j] = A[i][j] + mat[i][j - 1];
        }
    }

    int maxSum = -32000;
    int top = -1, bottom = -1, left = -1, right = -1;

    for (int i = 0; i < c; i++) {
        for (int j = i; j < c; j++) {
            int v[MAX_ROWS];
            for (int k = 0; k < r; k++) {
                int m = 0;
                if (i == 0)
                    m = mat[k][j];
                else
                    m = mat[k][j] - mat[k][i - 1];
                v[k] = m;
            }
            int sum, start, end;
            maxSubArraySum(v, r, &sum, &start, &end);
            if (sum > maxSum) {
                maxSum = sum;
                top = start;
                bottom = end;
                left = i;
                right = j;
            }
        }
    }

    printf("The submatrix is:\n{\n");
    for (int i = top; i <= bottom; i++) {
        printf("{");
        for (int j = left; j <= right; j++) {
            printf("%d", A[i][j]);
            if (j < right) printf(", ");
        }
        printf("}\n");
    }
    printf("}\n");

    printf("The max sum is: %d\n", maxSum);

    for (int i = 0; i < r; i++) {
        free(mat[i]);
    }
    free(mat);
}

int main() {
    FILE *fptr;
    char filename[] = "brightest_input.txt";
    char line[100];

    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    int arr[MAX_ROWS][MAX_COLS];
    int row = 0;
    int col;

    while (fgets(line, sizeof(line), fptr)) {
        char *token = strtok(line, ",[]\n");
        col = 0;
        while (token != NULL && col < MAX_COLS) {
            arr[row][col++] = atoi(token);
            token = strtok(NULL, ",[]\n");
        }
        row++;
    }

    fclose(fptr);

    printf("Contents of the array:\n");
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            arr[i][j] -= 128;
        }
    }
    
    maxSubmatrixSum(arr, row, col); 

    return 0;
}
