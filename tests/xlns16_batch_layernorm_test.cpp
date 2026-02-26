// Test file for xlns16 batch primitives, batch activations, and layernorm
// Style: data arrays + loops (matching explog_test pattern)
// Usage:  g++ -std=c++11 -O2 -I.. xlns16_batch_layernorm_test.cpp -o xlns16_batch_layernorm_test -lm && ./xlns16_batch_layernorm_test

#define xlns16_alt
#include "xlns16.cpp"

#include <cstdio>
#include <cmath>

static float relerr(float got, float expected) {
    if (fabsf(expected) < 1e-10f) return 0.0f;
    return fabsf(got - expected) / fabsf(expected) * 100.0f;
}

// ===================================================================
//  Batch Primitives
// ===================================================================

// --- batch_sub: c[i] = a[i] - b[i] ---
void test_batch_sub() {
    printf("--- xlns16_batch_sub ---\n");
    printf("  a[i]     b[i]     got       expected   err%%\n");

    float af[] = { 5.0f,  3.0f, -2.0f,  1.0f,  4.0f,  0.5f};
    float bf[] = { 2.0f,  3.0f,  1.0f, -1.0f,  0.5f,  0.5f};
    const size_t N = sizeof(af) / sizeof(af[0]);

    xlns16 a[N], b[N], c[N];
    for (size_t i = 0; i < N; i++) { a[i] = fp2xlns16(af[i]); b[i] = fp2xlns16(bf[i]); }
    xlns16_batch_sub(a, b, c, N);

    for (size_t i = 0; i < N; i++) {
        float got = xlns162fp(c[i]);
        float expected = af[i] - bf[i];
        if (fabsf(expected) < 1e-6f)
            printf("  %6.2f   %6.2f   %9.5f  %9.5f  (near zero)\n", af[i], bf[i], got, expected);
        else
            printf("  %6.2f   %6.2f   %9.5f  %9.5f  %5.2f%%\n", af[i], bf[i], got, expected, relerr(got, expected));
    }
    printf("\n");
}

// --- batch_div: c[i] = a[i] / b[i] ---
void test_batch_div() {
    printf("--- xlns16_batch_div ---\n");
    printf("  a[i]     b[i]     got       expected   err%%\n");

    float af[] = { 6.0f,  3.0f, -4.0f,  1.0f,  8.0f,  0.5f};
    float bf[] = { 2.0f,  3.0f,  2.0f, -0.5f,  4.0f,  0.25f};
    const size_t N = sizeof(af) / sizeof(af[0]);

    xlns16 a[N], b[N], c[N];
    for (size_t i = 0; i < N; i++) { a[i] = fp2xlns16(af[i]); b[i] = fp2xlns16(bf[i]); }
    xlns16_batch_div(a, b, c, N);

    for (size_t i = 0; i < N; i++) {
        float got = xlns162fp(c[i]);
        float expected = af[i] / bf[i];
        printf("  %6.2f   %6.2f   %9.5f  %9.5f  %5.2f%%\n", af[i], bf[i], got, expected, relerr(got, expected));
    }
    printf("\n");
}

// --- batch_scale: c[i] = a[i] * scalar ---
void test_batch_scale() {
    printf("--- xlns16_batch_scale ---\n");
    printf("  a[i]     scalar   got       expected   err%%\n");

    float af[] = { 1.0f, 2.0f, -3.0f, 0.5f, 4.0f, -0.25f};
    float scalar_f = 3.0f;
    const size_t N = sizeof(af) / sizeof(af[0]);

    xlns16 a[N], c[N];
    for (size_t i = 0; i < N; i++) a[i] = fp2xlns16(af[i]);
    xlns16 scalar = fp2xlns16(scalar_f);
    xlns16_batch_scale(a, scalar, c, N);

    for (size_t i = 0; i < N; i++) {
        float got = xlns162fp(c[i]);
        float expected = af[i] * scalar_f;
        printf("  %6.2f   %6.2f   %9.5f  %9.5f  %5.2f%%\n", af[i], scalar_f, got, expected, relerr(got, expected));
    }
    printf("\n");
}

// --- batch_neg: c[i] = -a[i] ---
void test_batch_neg() {
    printf("--- xlns16_batch_neg ---\n");
    printf("  a[i]     got       expected   err%%\n");

    float af[] = { 5.0f, -3.0f, 1.0f, -0.5f, 2.0f, -4.0f};
    const size_t N = sizeof(af) / sizeof(af[0]);

    xlns16 a[N], c[N];
    for (size_t i = 0; i < N; i++) a[i] = fp2xlns16(af[i]);
    xlns16_batch_neg(a, c, N);

    for (size_t i = 0; i < N; i++) {
        float got = xlns162fp(c[i]);
        float expected = -af[i];
        printf("  %6.2f   %9.5f  %9.5f  %5.2f%%\n", af[i], got, expected, relerr(got, expected));
    }
    printf("\n");
}

// --- batch_abs: c[i] = |a[i]| ---
void test_batch_abs() {
    printf("--- xlns16_batch_abs ---\n");
    printf("  a[i]     got       expected   err%%\n");

    float af[] = { 5.0f, -3.0f, 1.0f, -0.5f, 2.0f, -4.0f};
    const size_t N = sizeof(af) / sizeof(af[0]);

    xlns16 a[N], c[N];
    for (size_t i = 0; i < N; i++) a[i] = fp2xlns16(af[i]);
    xlns16_batch_abs(a, c, N);

    for (size_t i = 0; i < N; i++) {
        float got = xlns162fp(c[i]);
        float expected = fabsf(af[i]);
        printf("  %6.2f   %9.5f  %9.5f  %5.2f%%\n", af[i], got, expected, relerr(got, expected));
    }
    printf("\n");
}

// ===================================================================
//  Batch Activations
// ===================================================================

// Shared input array for all batch activation tests
static float act_inputs[] = {-4.0f, -2.0f, -1.0f, -0.5f, 0.0f, 0.5f, 1.0f, 2.0f, 4.0f};
static const size_t ACT_N = sizeof(act_inputs) / sizeof(act_inputs[0]);

// --- batch_relu ---
void test_batch_relu() {
    printf("--- xlns16_batch_relu ---\n");
    printf("  x        got       expected   err%%\n");

    xlns16 a[ACT_N], c[ACT_N];
    for (size_t i = 0; i < ACT_N; i++) a[i] = fp2xlns16(act_inputs[i]);
    xlns16_batch_relu(a, c, ACT_N);

    for (size_t i = 0; i < ACT_N; i++) {
        float got = xlns162fp(c[i]);
        float expected = fmaxf(0.0f, act_inputs[i]);
        if (fabsf(expected) < 1e-10f)
            printf("  %6.1f  %9.5f  %9.5f  (zero)\n", act_inputs[i], got, expected);
        else
            printf("  %6.1f  %9.5f  %9.5f  %5.2f%%\n", act_inputs[i], got, expected, relerr(got, expected));
    }
    printf("\n");
}

// --- batch_sigmoid ---
void test_batch_sigmoid() {
    printf("--- xlns16_batch_sigmoid ---\n");
    printf("  x        got       expected   err%%\n");

    xlns16 a[ACT_N], c[ACT_N];
    for (size_t i = 0; i < ACT_N; i++) a[i] = fp2xlns16(act_inputs[i]);
    xlns16_batch_sigmoid(a, c, ACT_N);

    for (size_t i = 0; i < ACT_N; i++) {
        float got = xlns162fp(c[i]);
        float expected = 1.0f / (1.0f + expf(-act_inputs[i]));
        printf("  %6.1f  %9.5f  %9.5f  %5.2f%%\n", act_inputs[i], got, expected, relerr(got, expected));
    }
    printf("\n");
}

// --- batch_tanh ---
void test_batch_tanh() {
    printf("--- xlns16_batch_tanh ---\n");
    printf("  x        got       expected   err%%\n");

    xlns16 a[ACT_N], c[ACT_N];
    for (size_t i = 0; i < ACT_N; i++) a[i] = fp2xlns16(act_inputs[i]);
    xlns16_batch_tanh(a, c, ACT_N);

    for (size_t i = 0; i < ACT_N; i++) {
        float got = xlns162fp(c[i]);
        float expected = tanhf(act_inputs[i]);
        if (fabsf(expected) < 1e-10f)
            printf("  %6.1f  %9.5f  %9.5f  (near zero)\n", act_inputs[i], got, expected);
        else
            printf("  %6.1f  %9.5f  %9.5f  %5.2f%%\n", act_inputs[i], got, expected, relerr(got, expected));
    }
    printf("\n");
}

// --- batch_silu ---
void test_batch_silu() {
    printf("--- xlns16_batch_silu ---\n");
    printf("  x        got       expected   err%%\n");

    xlns16 a[ACT_N], c[ACT_N];
    for (size_t i = 0; i < ACT_N; i++) a[i] = fp2xlns16(act_inputs[i]);
    xlns16_batch_silu(a, c, ACT_N);

    for (size_t i = 0; i < ACT_N; i++) {
        float got = xlns162fp(c[i]);
        float expected = act_inputs[i] / (1.0f + expf(-act_inputs[i]));
        if (fabsf(expected) < 1e-10f)
            printf("  %6.1f  %9.5f  %9.5f  (near zero)\n", act_inputs[i], got, expected);
        else
            printf("  %6.1f  %9.5f  %9.5f  %5.2f%%\n", act_inputs[i], got, expected, relerr(got, expected));
    }
    printf("\n");
}

// --- batch_gelu ---
void test_batch_gelu() {
    printf("--- xlns16_batch_gelu ---\n");
    printf("  x        got       expected   err%%\n");

    xlns16 a[ACT_N], c[ACT_N];
    for (size_t i = 0; i < ACT_N; i++) a[i] = fp2xlns16(act_inputs[i]);
    xlns16_batch_gelu(a, c, ACT_N);

    for (size_t i = 0; i < ACT_N; i++) {
        float x = act_inputs[i];
        float got = xlns162fp(c[i]);
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

// ===================================================================
//  Layer Normalization
// ===================================================================
void test_layernorm() {
    printf("--- xlns16_layernorm ---\n");

    // --- Case 1: no gamma/beta (pure normalization) ---
    printf("  Case 1: no gamma, no beta\n");
    printf("  x[i]     got       expected   err%%\n");

    float xf[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    const size_t N = sizeof(xf) / sizeof(xf[0]);
    float eps = 1e-5f;

    // compute expected in float
    float mean_f = 0.0f;
    for (size_t i = 0; i < N; i++) mean_f += xf[i];
    mean_f /= N;
    float var_f = 0.0f;
    for (size_t i = 0; i < N; i++) var_f += (xf[i] - mean_f) * (xf[i] - mean_f);
    var_f /= N;
    float inv_std = 1.0f / sqrtf(var_f + eps);

    float expected[N];
    for (size_t i = 0; i < N; i++) expected[i] = (xf[i] - mean_f) * inv_std;

    xlns16 x16[N], out16[N];
    for (size_t i = 0; i < N; i++) x16[i] = fp2xlns16(xf[i]);
    xlns16_layernorm(x16, out16, nullptr, nullptr, N, eps);

    for (size_t i = 0; i < N; i++) {
        float got = xlns162fp(out16[i]);
        if (fabsf(expected[i]) < 1e-6f)
            printf("  %6.2f   %9.5f  %9.5f  (near zero)\n", xf[i], got, expected[i]);
        else
            printf("  %6.2f   %9.5f  %9.5f  %5.2f%%\n", xf[i], got, expected[i], relerr(got, expected[i]));
    }
    printf("\n");

    // --- Case 2: with gamma and beta ---
    printf("  Case 2: with gamma and beta\n");
    printf("  x[i]     gamma    beta     got       expected   err%%\n");

    float gf[] = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
    float betaf[] = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f};

    float expected2[N];
    for (size_t i = 0; i < N; i++) expected2[i] = (xf[i] - mean_f) * inv_std * gf[i] + betaf[i];

    xlns16 gamma16[N], beta16[N], out2[N];
    for (size_t i = 0; i < N; i++) { gamma16[i] = fp2xlns16(gf[i]); beta16[i] = fp2xlns16(betaf[i]); }
    xlns16_layernorm(x16, out2, gamma16, beta16, N, eps);

    for (size_t i = 0; i < N; i++) {
        float got = xlns162fp(out2[i]);
        if (fabsf(expected2[i]) < 1e-6f)
            printf("  %6.2f   %5.2f    %5.2f   %9.5f  %9.5f  (near zero)\n", xf[i], gf[i], betaf[i], got, expected2[i]);
        else
            printf("  %6.2f   %5.2f    %5.2f   %9.5f  %9.5f  %5.2f%%\n", xf[i], gf[i], betaf[i], got, expected2[i], relerr(got, expected2[i]));
    }
    printf("\n");
}

int main() {
    printf("=============================================================\n");
    printf("  xlns16 batch primitives / activations / layernorm test     \n");
    printf("=============================================================\n\n");

    // Batch primitives
    test_batch_sub();
    test_batch_div();
    test_batch_scale();
    test_batch_neg();
    test_batch_abs();

    // Batch activations
    test_batch_relu();
    test_batch_sigmoid();
    test_batch_tanh();
    test_batch_silu();
    test_batch_gelu();

    // Normalization
    test_layernorm();

    printf("All tests done.\n");
    return 0;
}
