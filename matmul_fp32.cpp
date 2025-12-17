#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Matrix multiplication: C = A × B
// A is M×K, B is K×N, C is M×N
void matmul_fp32(const float* A, const float* B, float* C, int M, int K, int N) {
    for (int i = 0; i < M; i++) {           // For each row of A
        for (int j = 0; j < N; j++) {       // For each column of B
            float sum = 0.0f;
            for (int k = 0; k < K; k++) {   // Sum of products
                sum += A[i * K + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

// Helper to print a matrix
void print_matrix(const char* name, const float* M, int rows, int cols) {
    printf("%s (%dx%d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.4f ", M[i * cols + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    // Test case matching ggml example dimension
    const int M = 4;  // Rows in A and C
    const int K = 3;  // Cols in A, rows in B
    const int N = 2;  // Cols in B and C

    // Allocate matrices
    float* A = (float*)malloc(M * K * sizeof(float));
    float* B = (float*)malloc(K * N * sizeof(float));
    float* C = (float*)malloc(M * N * sizeof(float));

    // Initialize matrix A (4×3)
    // Row-major order: [row0, row1, row2, row3]
    float A_data[12] = {
        1.0f, 2.0f, 3.0f,
        4.0f, 5.0f, 6.0f,
        7.0f, 8.0f, 9.0f,
        10.0f, 11.0f, 12.0f
    };
    for (int i = 0; i < M * K; i++) A[i] = A_data[i];

    // Initialize matrix B (3×2)
    float B_data[6] = {
        1.0f, 2.0f,
        3.0f, 4.0f,
        5.0f, 6.0f
    };
    for (int i = 0; i < K * N; i++) B[i] = B_data[i];

    // Initialize C to zero
    for (int i = 0; i < M * N; i++) C[i] = 0.0f;

    printf("=== Floating-Point 32-bit Matrix Multiplication ===\n\n");

    // Print input matrices
    print_matrix("Matrix A", A, M, K);
    print_matrix("Matrix B", B, K, N);

    // Perform matrix multiplication
    clock_t start = clock();
    matmul_fp32(A, B, C, M, K, N);
    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

    // Print result
    print_matrix("Matrix C = A × B", C, M, N);

    printf("Computation time: %.6f ms\n\n", time_taken);

    // Verify one result manually:
    // C[0,0] = A[0,0]*B[0,0] + A[0,1]*B[1,0] + A[0,2]*B[2,0]
    //        = 1*1 + 2*3 + 3*5 = 1 + 6 + 15 = 22
    printf("Verification:\n");
    printf("C[0,0] = A[0,0]*B[0,0] + A[0,1]*B[1,0] + A[0,2]*B[2,0]\n");
    printf("       = %.1f*%.1f + %.1f*%.1f + %.1f*%.1f\n",
           A[0], B[0], A[1], B[2], A[2], B[4]);
    printf("       = %.1f + %.1f + %.1f = %.1f\n",
           A[0]*B[0], A[1]*B[2], A[2]*B[4], C[0]);
    printf("Expected: 22.0, Got: %.1f %s\n\n",
           C[0], (C[0] == 22.0f) ? "✓" : "✗");

    // Test with larger matrices for performance
    printf("=== Performance Test with 64×64 matrices ===\n");
    const int SIZE = 64;
    float* A_large = (float*)malloc(SIZE * SIZE * sizeof(float));
    float* B_large = (float*)malloc(SIZE * SIZE * sizeof(float));
    float* C_large = (float*)malloc(SIZE * SIZE * sizeof(float));

    // Initialize with random-ish values
    for (int i = 0; i < SIZE * SIZE; i++) {
        A_large[i] = (float)(i % 10) / 10.0f;
        B_large[i] = (float)((i + 7) % 10) / 10.0f;
    }

    start = clock();
    matmul_fp32(A_large, B_large, C_large, SIZE, SIZE, SIZE);
    end = clock();
    time_taken = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;

    printf("64×64 matrix multiplication time: %.3f ms\n", time_taken);
    printf("Sample result C_large[0,0] = %.4f\n\n", C_large[0]);

    // Cleanup
    free(A);
    free(B);
    free(C);
    free(A_large);
    free(B_large);
    free(C_large);
    return 0;
}
