// Test file for xlns16 scalar math and vector operations
// Functions: square, fma, tanh, gelu, vec_dot, max_array, min_array
// Style: data arrays + loops (matching explog_test pattern)
// Usage:  g++ -std=c++11 -O2 -I.. xlns16_scalar_vector_test.cpp -o xlns16_scalar_vector_test -lm && ./xlns16_scalar_vector_test

#define xlns16_alt
#include "xlns16.cpp"

#include <cstdio>
#include <cmath>

static float relerr(float got, float expected) {
    if (fabsf(expected) < 1e-10f) return 0.0f;
    return fabsf(got - expected) / fabsf(expected) * 100.0f;
}

// -----------------------------------------------------------
//  xlns16_square
// -----------------------------------------------------------
void test_square() {
    printf("--- xlns16_square ---\n");
    printf("  x        got       expected   err%%\n");

    float xs[] = {0.5f, 1.0f, 2.0f, 3.0f, -3.0f, 4.0f, -5.0f, 0.25f};
    for (float x : xs) {
        float got = xlns162fp(xlns16_square(fp2xlns16(x)));
        float expected = x * x;
        printf("  %6.2f  %9.5f  %9.5f  %5.2f%%\n", x, got, expected, relerr(got, expected));
    }
    printf("\n");
}

// -----------------------------------------------------------
//  xlns16_fma: a*b + c
// -----------------------------------------------------------
void test_fma() {
    printf("--- xlns16_fma(a, b, c) = a*b + c ---\n");
    printf("  a       b       c       got       expected   err%%\n");

    struct { float a, b, c; } cases[] = {
        {  2.0f,  3.0f,  4.0f },   // 6 + 4 = 10
        {  1.0f,  1.0f,  0.0f },   // 1 + 0 = 1
        { -2.0f,  3.0f,  1.0f },   // -6 + 1 = -5
        {  0.5f,  4.0f, -1.0f },   // 2 - 1 = 1
        {  3.0f, -2.0f,  8.0f },   // -6 + 8 = 2
        {  4.0f,  4.0f,  1.0f },   // 16 + 1 = 17
        {  0.25f, 8.0f,  0.5f },   // 2 + 0.5 = 2.5
        { -1.0f, -1.0f, -1.0f },   // 1 - 1 = 0
    };

    for (auto& t : cases) {
        xlns16 xa = fp2xlns16(t.a), xb = fp2xlns16(t.b), xc = fp2xlns16(t.c);
        float got = xlns162fp(xlns16_fma(xa, xb, xc));
        float expected = t.a * t.b + t.c;
        if (fabsf(expected) < 1e-6f)
            printf("  %5.2f  %5.2f  %5.2f  %9.5f  %9.5f  (near zero)\n", t.a, t.b, t.c, got, expected);
        else
            printf("  %5.2f  %5.2f  %5.2f  %9.5f  %9.5f  %5.2f%%\n", t.a, t.b, t.c, got, expected, relerr(got, expected));
    }
    printf("\n");
}

// -----------------------------------------------------------
//  xlns16_tanh
// -----------------------------------------------------------
void test_tanh() {
    printf("--- xlns16_tanh ---\n");
    printf("  x        got       expected   err%%\n");

    float xs[] = {-4.0f, -2.0f, -1.0f, -0.5f, 0.0f, 0.5f, 1.0f, 2.0f, 4.0f};
    for (float x : xs) {
        float got = xlns162fp(xlns16_tanh(fp2xlns16(x)));
        float expected = tanhf(x);
        if (fabsf(expected) < 1e-10f)
            printf("  %6.1f  %9.5f  %9.5f  (near zero)\n", x, got, expected);
        else
            printf("  %6.1f  %9.5f  %9.5f  %5.2f%%\n", x, got, expected, relerr(got, expected));
    }
    printf("\n");
}

// -----------------------------------------------------------
//  xlns16_gelu
// -----------------------------------------------------------
void test_gelu() {
    printf("--- xlns16_gelu ---\n");
    printf("  x        got       expected   err%%\n");

    float xs[] = {-4.0f, -2.0f, -1.0f, -0.5f, 0.0f, 0.5f, 1.0f, 2.0f, 4.0f};
    for (float x : xs) {
        float got = xlns162fp(xlns16_gelu(fp2xlns16(x)));
        const float sqrt_2_over_pi = 0.7978845608f;
        float inner = sqrt_2_over_pi * (x + 0.044715f * x * x * x);
        float expected = 0.5f * x * (1.0f + tanhf(inner));
        if (fabsf(expected) < 1e-10f)
            printf("  %6.1f  %9.5f  %9.5f  (near zero)\n", x, got, expected);
        else
            printf("  %6.1f  %9.5f  %9.5f  %5.2f%%\n", x, got, expected, relerr(got, expected));
    }
    printf("\n");
}

// -----------------------------------------------------------
//  xlns16_vec_dot (native xlns16 arrays)
// -----------------------------------------------------------
void test_vec_dot() {
    printf("--- xlns16_vec_dot ---\n");
    printf("  case                           got       expected   err%%\n");

    struct {
        const char *label;
        float a[4]; float b[4]; size_t n;
    } cases[] = {
        { "[1,2,3,4]·[4,3,2,1]",  {1,2,3,4}, {4,3,2,1}, 4 },
        { "[1,1,1,1]·[2,2,2,2]",  {1,1,1,1}, {2,2,2,2}, 4 },
        { "[0.5,2]·[4,0.25]",     {0.5f,2,0,0}, {4,0.25f,0,0}, 2 },
        { "[-1,3]·[2,-2]",        {-1,3,0,0}, {2,-2,0,0}, 2 },
    };

    for (auto& t : cases) {
        xlns16 a[4], b[4];
        for (size_t i = 0; i < t.n; i++) { a[i] = fp2xlns16(t.a[i]); b[i] = fp2xlns16(t.b[i]); }
        float got = xlns162fp(xlns16_vec_dot(a, b, t.n));
        float expected = 0.0f;
        for (size_t i = 0; i < t.n; i++) expected += t.a[i] * t.b[i];
        if (fabsf(expected) < 1e-6f)
            printf("  %-30s  %9.5f  %9.5f  (near zero)\n", t.label, got, expected);
        else
            printf("  %-30s  %9.5f  %9.5f  %5.2f%%\n", t.label, got, expected, relerr(got, expected));
    }
    printf("\n");
}

// -----------------------------------------------------------
//  xlns16_max_array
// -----------------------------------------------------------
void test_max_array() {
    printf("--- xlns16_max_array ---\n");
    printf("  array                          got       expected   err%%\n");

    struct {
        const char *label;
        float vals[6]; size_t n; float expected;
    } cases[] = {
        { "[1, 3, 2, 5, 4]",         {1,3,2,5,4,0},  5,  5.0f },
        { "[-1, -3, -2, -5, -4]",    {-1,-3,-2,-5,-4,0}, 5, -1.0f },
        { "[0.5, 2, 1, 4, 0.25]",    {0.5f,2,1,4,0.25f,0}, 5,  4.0f },
        { "[-2, 3, -1, 0.5, 8, 1]",  {-2,3,-1,0.5f,8,1}, 6,  8.0f },
    };

    for (auto& t : cases) {
        xlns16 arr[6];
        for (size_t i = 0; i < t.n; i++) arr[i] = fp2xlns16(t.vals[i]);
        float got = xlns162fp(xlns16_max_array(arr, t.n));
        printf("  %-30s  %9.5f  %9.5f  %5.2f%%\n", t.label, got, t.expected, relerr(got, t.expected));
    }
    printf("\n");
}

// -----------------------------------------------------------
//  xlns16_min_array
// -----------------------------------------------------------
void test_min_array() {
    printf("--- xlns16_min_array ---\n");
    printf("  array                          got       expected   err%%\n");

    struct {
        const char *label;
        float vals[6]; size_t n; float expected;
    } cases[] = {
        { "[1, 3, 2, 5, 4]",         {1,3,2,5,4,0},  5,  1.0f },
        { "[-1, -3, -2, -5, -4]",    {-1,-3,-2,-5,-4,0}, 5, -5.0f },
        { "[0.5, 2, 1, 4, 0.25]",    {0.5f,2,1,4,0.25f,0}, 5,  0.25f },
        { "[-2, 3, -1, 0.5, 8, 1]",  {-2,3,-1,0.5f,8,1}, 6, -2.0f },
    };

    for (auto& t : cases) {
        xlns16 arr[6];
        for (size_t i = 0; i < t.n; i++) arr[i] = fp2xlns16(t.vals[i]);
        float got = xlns162fp(xlns16_min_array(arr, t.n));
        printf("  %-30s  %9.5f  %9.5f  %5.2f%%\n", t.label, got, t.expected, relerr(got, t.expected));
    }
    printf("\n");
}

int main() {
    printf("=============================================================\n");
    printf("  xlns16 scalar math / vector operations test                \n");
    printf("=============================================================\n\n");

    test_square();
    test_fma();
    test_tanh();
    test_gelu();
    test_vec_dot();
    test_max_array();
    test_min_array();

    printf("All tests done.\n");
    return 0;
}
