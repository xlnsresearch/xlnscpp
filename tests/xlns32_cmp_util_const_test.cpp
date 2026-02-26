// Test file for xlns32 comparison, utility, and constant functions
// Style: data arrays + loops (matching explog_test pattern)
// Usage:  g++ -std=c++11 -O2 xlns32_cmp_util_const_test.cpp -o xlns32_cmp_util_const_test -lm && ./xlns32_cmp_util_const_test

#define xlns32_alt
#include "../xlns32.cpp"

#include <cstdio>
#include <cmath>

static float relerr(float got, float expected) {
    if (fabsf(expected) < 1e-10f) return 0.0f;
    return fabsf(got - expected) / fabsf(expected) * 100.0f;
}

// -----------------------------------------------------------
//  Comparison: xlns32_eq
// -----------------------------------------------------------
void test_eq() {
    printf("--- xlns32_eq ---\n");
    printf("  a        b        got  expected  err\n");

    struct { float a, b; int expected; } cases[] = {
        {  3.14f,  3.14f,  1 },
        {  5.0f,   3.0f,   0 },
        {  3.0f,   5.0f,   0 },
        { -2.0f,   1.0f,   0 },
        {  1.0f,  -2.0f,   0 },
        { -4.0f,  -4.0f,   1 },
        { -1.0f,  -3.0f,   0 },
        {  0.5f,   0.5f,   1 },
        {  2.0f,   4.0f,   0 },
        { 100.0f, -0.01f,  0 },
    };

    for (auto& c : cases) {
        int got = xlns32_eq(fp2xlns32(c.a), fp2xlns32(c.b));
        printf("  %7.2f  %7.2f   %d    %d         %d\n",
               c.a, c.b, got, c.expected, got - c.expected);
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Comparison: xlns32_ge
// -----------------------------------------------------------
void test_ge() {
    printf("--- xlns32_ge ---\n");
    printf("  a        b        got  expected  err\n");

    struct { float a, b; int expected; } cases[] = {
        {  3.14f,  3.14f,  1 },
        {  5.0f,   3.0f,   1 },
        {  3.0f,   5.0f,   0 },
        { -2.0f,   1.0f,   0 },
        {  1.0f,  -2.0f,   1 },
        { -4.0f,  -4.0f,   1 },
        { -1.0f,  -3.0f,   1 },
        {  0.5f,   0.5f,   1 },
        {  2.0f,   4.0f,   0 },
        { 100.0f, -0.01f,  1 },
    };

    for (auto& c : cases) {
        int got = xlns32_ge(fp2xlns32(c.a), fp2xlns32(c.b));
        printf("  %7.2f  %7.2f   %d    %d         %d\n",
               c.a, c.b, got, c.expected, got - c.expected);
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Comparison: xlns32_le
// -----------------------------------------------------------
void test_le() {
    printf("--- xlns32_le ---\n");
    printf("  a        b        got  expected  err\n");

    struct { float a, b; int expected; } cases[] = {
        {  3.14f,  3.14f,  1 },
        {  5.0f,   3.0f,   0 },
        {  3.0f,   5.0f,   1 },
        { -2.0f,   1.0f,   1 },
        {  1.0f,  -2.0f,   0 },
        { -4.0f,  -4.0f,   1 },
        { -1.0f,  -3.0f,   0 },
        {  0.5f,   0.5f,   1 },
        {  2.0f,   4.0f,   1 },
        { 100.0f, -0.01f,  0 },
    };

    for (auto& c : cases) {
        int got = xlns32_le(fp2xlns32(c.a), fp2xlns32(c.b));
        printf("  %7.2f  %7.2f   %d    %d         %d\n",
               c.a, c.b, got, c.expected, got - c.expected);
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Utility: copysign
// -----------------------------------------------------------
void test_copysign() {
    printf("--- xlns32_copysign(mag, sgn) ---\n");
    printf("  mag       sgn       got          expected     err%%\n");

    struct { float mag, sgn, expected; } cases[] = {
        {  5.0f,   3.0f,   5.0f },
        {  5.0f,  -3.0f,  -5.0f },
        { -5.0f,   3.0f,   5.0f },
        { -5.0f,  -3.0f,  -5.0f },
        {  1.0f,  -0.5f,  -1.0f },
        {  0.5f,   2.0f,   0.5f },
        {  7.25f, -1.0f,  -7.25f },
        {100.0f,   0.1f, 100.0f },
    };

    for (auto& c : cases) {
        float got = xlns322fp(xlns32_copysign(fp2xlns32(c.mag), fp2xlns32(c.sgn)));
        printf("  %7.2f   %7.2f   %12.7f  %12.7f  %6.4f%%\n",
               c.mag, c.sgn, got, c.expected, relerr(got, c.expected));
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Utility: canon (total ordering check)
// -----------------------------------------------------------
void test_canon() {
    printf("--- xlns32_canon total ordering ---\n");
    printf("  v_lo      v_hi      canon(lo)<canon(hi)  expected\n");

    float ordered[] = {-100.0f, -4.0f, -2.0f, -1.0f, -0.01f, 0.01f, 1.0f, 2.0f, 4.0f, 100.0f};
    int n = sizeof(ordered) / sizeof(ordered[0]);

    for (int i = 0; i < n - 1; i++) {
        xlns32 lo = fp2xlns32(ordered[i]);
        xlns32 hi = fp2xlns32(ordered[i + 1]);
        int result = (xlns32_canon(lo) < xlns32_canon(hi)) ? 1 : 0;
        printf("  %8.2f  %8.2f       %d                  1        err=%d\n",
               ordered[i], ordered[i + 1], result, result - 1);
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Utility: sign
// -----------------------------------------------------------
void test_sign() {
    printf("--- xlns32_sign ---\n");
    printf("  x           got_hex       expected_hex    match\n");

    struct { float val; unsigned int exp_sign; } cases[] = {
        {   5.0f,  0x00000000 },
        {  -5.0f,  0x80000000 },
        {   1.0f,  0x00000000 },
        {  -0.5f,  0x80000000 },
        { 100.0f,  0x00000000 },
        {  -0.01f, 0x80000000 },
        {   0.25f, 0x00000000 },
    };

    for (auto& c : cases) {
        xlns32 x = fp2xlns32(c.val);
        unsigned int got = xlns32_sign(x);
        printf("  %8.2f    0x%08X    0x%08X      %s\n",
               c.val, got, c.exp_sign, (got == c.exp_sign) ? "yes" : "NO");
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Constants
// -----------------------------------------------------------
void test_constants() {
    printf("--- xlns32 constants ---\n");
    printf("  constant              hex          got           expected      err%%\n");

    struct { xlns32 val; float expected; const char *name; } consts[] = {
        { xlns32_one,       1.0f,  "xlns32_one" },
        { xlns32_neg_one,  -1.0f,  "xlns32_neg_one" },
        { xlns32_two,       2.0f,  "xlns32_two" },
        { xlns32_neg_two,  -2.0f,  "xlns32_neg_two" },
        { xlns32_half,      0.5f,  "xlns32_half" },
        { xlns32_neg_half, -0.5f,  "xlns32_neg_half" },
        { xlns32_zero,      0.0f,  "xlns32_zero" },
    };

    for (auto& c : consts) {
        float got = xlns322fp(c.val);
        printf("  %-20s  0x%08X  %12.7f  %12.7f  %6.4f%%\n",
               c.name, (unsigned int)c.val, got, c.expected, relerr(got, c.expected));
    }
    printf("\n");

    // Bit-level: neg(pos) == neg_const
    printf("  neg consistency check:\n");
    printf("  operation              computed      const         match\n");
    struct { xlns32 pos; xlns32 neg_c; const char *label; } pairs[] = {
        { xlns32_one,  xlns32_neg_one,  "neg(one)  vs neg_one" },
        { xlns32_two,  xlns32_neg_two,  "neg(two)  vs neg_two" },
        { xlns32_half, xlns32_neg_half, "neg(half) vs neg_half" },
    };
    for (auto& p : pairs) {
        xlns32 computed = xlns32_neg(p.pos);
        printf("  %-22s  0x%08X    0x%08X    %s\n",
               p.label, (unsigned int)computed, (unsigned int)p.neg_c,
               (computed == p.neg_c) ? "yes" : "NO");
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Overflow: test via extreme mul/div that trigger overflow
// -----------------------------------------------------------
void test_overflow() {
    printf("--- xlns32_overflow (via extreme mul/div) ---\n");
    printf("  operation                        got_hex       expected_hex    result         match\n");

    // Construct xlns32 values with near-max log magnitude to force overflow
    xlns32 huge_pos  = 0x7F000000;   // very large positive
    xlns32 huge_neg  = 0xFF000000;   // very large negative
    xlns32 tiny_pos  = 0x01000000;   // very small positive

    struct { const char *label; xlns32 a; xlns32 b; bool is_mul; xlns32 expected; const char *exp_tag; } cases[] = {
        { "huge * huge  (overflow)",      huge_pos, huge_pos, true,  0x7FFFFFFF, "overflow->max"  },
        { "-huge * huge (overflow)",      huge_neg, huge_pos, true,  0xFFFFFFFF, "overflow->max"  },
        { "tiny / huge  (underflow)",     tiny_pos, huge_pos, false, 0x00000000, "underflow->0"   },
        { "huge / tiny  (overflow)",      huge_pos, tiny_pos, false, 0x7FFFFFFF, "overflow->max"  },
        { "2 * 4        (normal)",        fp2xlns32(2.0f), fp2xlns32(4.0f), true,  fp2xlns32(8.0f), "normal" },
        { "8 / 2        (normal)",        fp2xlns32(8.0f), fp2xlns32(2.0f), false, fp2xlns32(4.0f), "normal" },
    };

    for (auto& c : cases) {
        xlns32 result = c.is_mul ? xlns32_mul(c.a, c.b) : xlns32_div(c.a, c.b);
        const char *got_tag =
            (result == 0x00000000 || result == 0x80000000) ? "underflow->0" :
            ((result & xlns32_logmask) == xlns32_logmask)  ? "overflow->max" : "normal";
        printf("  %-32s  0x%08X    0x%08X  %-14s %s\n",
               c.label, (unsigned int)result, (unsigned int)c.expected,
               got_tag, (result == c.expected) ? "yes" : "NO");
    }
    printf("\n");
}

int main() {
    printf("=============================================================\n");
    printf("  xlns32 comparisons / utility / constants test              \n");
    printf("=============================================================\n\n");

    test_eq();
    test_ge();
    test_le();
    test_copysign();
    test_canon();
    test_sign();
    test_overflow();
    test_constants();

    printf("All tests done.\n");
    return 0;
}
