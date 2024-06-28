/*
Author: Annepu Sai Devesh
Date: March 11 2024
Given a text file containing a rectangular matrix representing the pixel values in an image 
(in the range 0 to 255). Designing an algorithm to print the brightest area in the image. 
The definition of the brightest area is a rectangular submatrix of A[][] of maximum sum, 
where A[][] is obtained by subtracting 128 from each pixel value in the original image. 
Also used Kadane's algorithm as a subroutine to find the brightest area.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants for maximum rows and columns
#define MAX_ROWS 100
#define MAX_COLS 100

// Function to find the maximum of two integers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Function to find the maximum sum subarray using Kadane's algorithm
void maxSubArraySum(int a[], int size, int* maxSum, int* top, int* bottom) {
    /* Allocate memory for intermediate array a1[], where a1[i] = (j, a j...i) (ordered pair) where j is the starting index for which we
    do have a subarray ending at index i of maximum sum (a j...i).*/
    int** a1 = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        a1[i] = (int*)malloc(2 * sizeof(int));
    }

    // Initialize the first element
    a1[0][1] = a[0];
    a1[0][0] = 0;
    int j = 0;
    *maxSum = a1[0][1];
    *top = *bottom = 0;

    // Iterate over the array to find maximum sum subarray
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

    // Free allocated memory
    for (int i = 0; i < size; i++) {
        free(a1[i]);
    }
    free(a1);
}

// Function to find the maximum sum submatrix
void maxSubmatrixSum(int A[MAX_ROWS][MAX_COLS], int r, int c) {
    // Allocate memory for auxiliary matrix
    int** mat = (int**)malloc(r * sizeof(int*));
    for (int i = 0; i < r; i++) {
        mat[i] = (int*)malloc(c * sizeof(int));
        for (int j = 0; j < c; j++) {
            mat[i][j] = 0;
        }
    }

    // Build auxiliary matrix
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (j == 0)
                mat[i][j] = A[i][j];
            else
                mat[i][j] = A[i][j] + mat[i][j - 1];
        }
    }

    int maxSum = -32000;  // Initialize maxSum with a small value
    int top = -1, bottom = -1, left = -1, right = -1;

    // Iterate over all column pairs
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
            // Find the maximum sum subarray for the current column pair
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

    // Print the resulting submatrix and its sum
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

    // Free allocated memory
    for (int i = 0; i < r; i++) {
        free(mat[i]);
    }
    free(mat);
}

int main() {
    FILE *fptr;
    char filename[] = "brightest_input.txt";
    char line[100];

    // Open the input file
    fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    int arr[MAX_ROWS][MAX_COLS];
    int row = 0;
    int col;

    // Read the file line by line and store the values in the array
    while (fgets(line, sizeof(line), fptr)) {
        char *token = strtok(line, ",[]\n");
        col = 0;
        while (token != NULL && col < MAX_COLS) {
            arr[row][col++] = atoi(token);
            token = strtok(NULL, ",[]\n");
        }
        row++;
    }

    // Close the file
    fclose(fptr);

    // Print the contents of the array
    printf("Contents of the array:\n");
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }

    // Adjust the array values by subtracting 128
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            arr[i][j] -= 128;
        }
    }
    
    // Find and print the maximum sum submatrix
    maxSubmatrixSum(arr, row, col); 

    return 0;
}
