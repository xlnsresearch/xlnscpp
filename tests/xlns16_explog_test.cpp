// Test file for xlns16 exp, log, exp2, log2, pow, and softmax functions
// Float round-trip (baseline) implementation
// Usage:  g++ -std=c++11 -Dxlns16_alt xlns16_explog_test.cpp -o xlns16_explog_test && ./xlns16_explog_test

#define xlns16_alt
#include "../xlns16.cpp"

#include <cstdio>
#include <cmath>

static float relerr(float got, float expected) {
    if (fabsf(expected) < 1e-10f) return 0.0f;
    return fabsf(got - expected) / fabsf(expected) * 100.0f;
}

void test_exp() {
    printf("--- xlns16_exp(x) vs expf(x) ---\n");
    printf("  x       got       expected   err%%\n");
    float xs[] = {-4.0f, -2.0f, -1.0f, -0.5f, 0.0f, 0.5f, 1.0f, 2.0f, 4.0f};
    for (float x : xs) {
        xlns16 r = xlns16_exp(fp2xlns16(x));
        float got = xlns162fp(r);
        float exp_val = expf(x);
        printf("%6.1f  %9.5f  %9.5f  %5.2f%%\n", x, got, exp_val, relerr(got, exp_val));
    }
    printf("\n");
}

void test_log() {
    printf("--- xlns16_log(x) vs logf(x) ---\n");
    printf("  x       got       expected   err%%\n");
    float xs[] = {0.01f, 0.1f, 0.5f, 1.0f, 2.0f, 4.0f, 10.0f, 100.0f};
    for (float x : xs) {
        xlns16 r = xlns16_log(fp2xlns16(x));
        float got = xlns162fp(r);
        float log_val = logf(x);
        if (fabsf(log_val) < 1e-6f)
            printf("%6.2f  %9.5f  %9.5f  (exact zero)\n", x, got, log_val);
        else
            printf("%6.2f  %9.5f  %9.5f  %5.2f%%\n", x, got, log_val, relerr(got, log_val));
    }
    printf("\n");
}

void test_exp2() {
    printf("--- xlns16_exp2(x) vs exp2f(x) ---\n");
    printf("  x       got       expected   err%%\n");
    float xs[] = {-4.0f, -2.0f, -1.0f, -0.5f, 0.0f, 0.5f, 1.0f, 2.0f, 4.0f};
    for (float x : xs) {
        xlns16 r = xlns16_exp2(fp2xlns16(x));
        float got = xlns162fp(r);
        float exp2_val = exp2f(x);
        printf("%6.1f  %9.5f  %9.5f  %5.2f%%\n", x, got, exp2_val, relerr(got, exp2_val));
    }
    printf("\n");
}

void test_log2() {
    printf("--- xlns16_log2(x) vs log2f(x) ---\n");
    printf("  x       got       expected   err%%\n");
    float xs[] = {0.01f, 0.1f, 0.5f, 1.0f, 2.0f, 4.0f, 8.0f, 16.0f};
    for (float x : xs) {
        xlns16 r = xlns16_log2(fp2xlns16(x));
        float got = xlns162fp(r);
        float log2_val = log2f(x);
        if (fabsf(log2_val) < 1e-6f)
            printf("%6.2f  %9.5f  %9.5f  (exact zero)\n", x, got, log2_val);
        else
            printf("%6.2f  %9.5f  %9.5f  %5.2f%%\n", x, got, log2_val, relerr(got, log2_val));
    }
    printf("\n");
}

void test_pow() {
    printf("--- xlns16_pow(base, exp) vs powf(base, exp) ---\n");
    printf("  base   exp    got       expected   err%%\n");
    struct { float b, e; } cases[] = {
        {2.0f, 0.5f}, {2.0f, 2.0f}, {2.0f, 10.0f},
        {3.0f, 2.0f}, {3.0f, 3.0f},
        {10.0f, 2.0f}, {10.0f, 3.0f},
        {0.5f, 2.0f}, {0.5f, 0.5f},
    };
    for (auto& c : cases) {
        xlns16 r = xlns16_pow(fp2xlns16(c.b), fp2xlns16(c.e));
        float got = xlns162fp(r);
        float pw = powf(c.b, c.e);
        printf("  %5.2f  %4.1f  %9.5f  %9.5f  %5.2f%%\n", c.b, c.e, got, pw, relerr(got, pw));
    }
    printf("\n");
}

void test_softmax_exp() {
    printf("--- xlns16_softmax_exp ---\n");
    printf("Input: [1, 2, 3, 4]\n");
    xlns16 in[4] = {fp2xlns16(1.0f), fp2xlns16(2.0f), fp2xlns16(3.0f), fp2xlns16(4.0f)};
    xlns16 out[4];
    xlns16_softmax_exp(in, out, 4);

    float expected[4] = {expf(-3.0f), expf(-2.0f), expf(-1.0f), expf(0.0f)};
    printf("  idx   got       expected   err%%\n");
    for (int i = 0; i < 4; i++) {
        float got = xlns162fp(out[i]);
        printf("  [%d]   %9.5f  %9.5f  %5.2f%%\n", i, got, expected[i], relerr(got, expected[i]));
    }
    printf("\n");
}

void test_softmax() {
    printf("--- xlns16_softmax ---\n");
    printf("Input: [1, 2, 3, 4]\n");
    xlns16 in[4] = {fp2xlns16(1.0f), fp2xlns16(2.0f), fp2xlns16(3.0f), fp2xlns16(4.0f)};
    xlns16 out[4];
    xlns16_softmax(in, out, 4);

    // Compute expected softmax
    float e1 = expf(1), e2 = expf(2), e3 = expf(3), e4 = expf(4);
    float sum = e1 + e2 + e3 + e4;
    float expected[4] = {e1/sum, e2/sum, e3/sum, e4/sum};
    
    printf("  idx   got       expected   err%%\n");
    for (int i = 0; i < 4; i++) {
        float got = xlns162fp(out[i]);
        printf("  [%d]   %9.5f  %9.5f  %5.2f%%\n", i, got, expected[i], relerr(got, expected[i]));
    }
    printf("\n");
}

int main() {
    printf("=============================================================\n");
    printf("  xlns16 exp / log / exp2 / log2 / pow / softmax test        \n");
    printf("  Float round-trip (baseline) implementation                 \n");
    printf("=============================================================\n\n");

    test_exp();
    test_log();
    test_exp2();
    test_log2();
    test_pow();
    test_softmax_exp();
    test_softmax();

    printf("All tests done.\n");
    return 0;
}
