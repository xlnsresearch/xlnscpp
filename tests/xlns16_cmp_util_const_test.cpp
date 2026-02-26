// Test file for xlns16 comparison, utility, and constant functions
// Style: data arrays + loops (matching explog_test pattern)
// Usage:  g++ -std=c++11 -O2 xlns16_cmp_util_const_test.cpp -o xlns16_cmp_util_const_test -lm && ./xlns16_cmp_util_const_test

#define xlns16_alt
#include "xlns16.cpp"

#include <cstdio>
#include <cmath>

static float relerr(float got, float expected) {
    if (fabsf(expected) < 1e-10f) return 0.0f;
    return fabsf(got - expected) / fabsf(expected) * 100.0f;
}

// -----------------------------------------------------------
//  Comparison: xlns16_eq
// -----------------------------------------------------------
void test_eq() {
    printf("--- xlns16_eq ---\n");
    printf("  a       b       got  expected  err\n");

    struct { float a, b; int expected; } cases[] = {
        {  3.0f,  3.0f,  1 },
        {  5.0f,  3.0f,  0 },
        {  3.0f,  5.0f,  0 },
        { -2.0f,  1.0f,  0 },
        {  1.0f, -2.0f,  0 },
        { -4.0f, -4.0f,  1 },
        { -1.0f, -3.0f,  0 },
        {  0.5f,  0.5f,  1 },
        {  2.0f,  4.0f,  0 },
    };

    for (auto& c : cases) {
        int got = xlns16_eq(fp2xlns16(c.a), fp2xlns16(c.b));
        printf("  %6.1f  %6.1f   %d    %d         %d\n",
               c.a, c.b, got, c.expected, got - c.expected);
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Comparison: xlns16_ge
// -----------------------------------------------------------
void test_ge() {
    printf("--- xlns16_ge ---\n");
    printf("  a       b       got  expected  err\n");

    struct { float a, b; int expected; } cases[] = {
        {  3.0f,  3.0f,  1 },
        {  5.0f,  3.0f,  1 },
        {  3.0f,  5.0f,  0 },
        { -2.0f,  1.0f,  0 },
        {  1.0f, -2.0f,  1 },
        { -4.0f, -4.0f,  1 },
        { -1.0f, -3.0f,  1 },
        {  0.5f,  0.5f,  1 },
        {  2.0f,  4.0f,  0 },
    };

    for (auto& c : cases) {
        int got = xlns16_ge(fp2xlns16(c.a), fp2xlns16(c.b));
        printf("  %6.1f  %6.1f   %d    %d         %d\n",
               c.a, c.b, got, c.expected, got - c.expected);
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Comparison: xlns16_le
// -----------------------------------------------------------
void test_le() {
    printf("--- xlns16_le ---\n");
    printf("  a       b       got  expected  err\n");

    struct { float a, b; int expected; } cases[] = {
        {  3.0f,  3.0f,  1 },
        {  5.0f,  3.0f,  0 },
        {  3.0f,  5.0f,  1 },
        { -2.0f,  1.0f,  1 },
        {  1.0f, -2.0f,  0 },
        { -4.0f, -4.0f,  1 },
        { -1.0f, -3.0f,  0 },
        {  0.5f,  0.5f,  1 },
        {  2.0f,  4.0f,  1 },
    };

    for (auto& c : cases) {
        int got = xlns16_le(fp2xlns16(c.a), fp2xlns16(c.b));
        printf("  %6.1f  %6.1f   %d    %d         %d\n",
               c.a, c.b, got, c.expected, got - c.expected);
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Utility: copysign
// -----------------------------------------------------------
void test_copysign() {
    printf("--- xlns16_copysign(mag, sgn) ---\n");
    printf("  mag      sgn      got       expected   err%%\n");

    struct { float mag, sgn, expected; } cases[] = {
        {  5.0f,  3.0f,  5.0f },
        {  5.0f, -3.0f, -5.0f },
        { -5.0f,  3.0f,  5.0f },
        { -5.0f, -3.0f, -5.0f },
        {  1.0f, -0.5f, -1.0f },
        {  0.5f,  2.0f,  0.5f },
        {  2.0f, -1.0f, -2.0f },
        {  4.0f,  4.0f,  4.0f },
    };

    for (auto& c : cases) {
        float got = xlns162fp(xlns16_copysign(fp2xlns16(c.mag), fp2xlns16(c.sgn)));
        printf("  %6.1f   %6.1f   %9.5f  %9.5f  %5.2f%%\n",
               c.mag, c.sgn, got, c.expected, relerr(got, c.expected));
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Utility: canon (total ordering check)
// -----------------------------------------------------------
void test_canon() {
    printf("--- xlns16_canon total ordering ---\n");
    printf("  v_lo     v_hi     canon(lo)<canon(hi)  expected\n");

    // ascending: each pair should satisfy canon(lo) < canon(hi)
    float ordered[] = {-4.0f, -2.0f, -1.0f, -0.5f, 0.5f, 1.0f, 2.0f, 4.0f};
    int n = sizeof(ordered) / sizeof(ordered[0]);

    for (int i = 0; i < n - 1; i++) {
        xlns16 lo = fp2xlns16(ordered[i]);
        xlns16 hi = fp2xlns16(ordered[i + 1]);
        int result = (xlns16_canon(lo) < xlns16_canon(hi)) ? 1 : 0;
        printf("  %6.1f   %6.1f        %d                  1        err=%d\n",
               ordered[i], ordered[i + 1], result, result - 1);
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Utility: sign
// -----------------------------------------------------------
void test_sign() {
    printf("--- xlns16_sign ---\n");
    printf("  x          got_hex    expected_hex   match\n");

    struct { float val; unsigned short exp_sign; } cases[] = {
        {  5.0f, 0x0000 },
        { -5.0f, 0x8000 },
        {  1.0f, 0x0000 },
        { -0.5f, 0x8000 },
        {  0.25f, 0x0000 },
        { -8.0f, 0x8000 },
    };

    for (auto& c : cases) {
        xlns16 x = fp2xlns16(c.val);
        unsigned short got = xlns16_sign(x);
        printf("  %6.2f     0x%04X     0x%04X         %s\n",
               c.val, got, c.exp_sign, (got == c.exp_sign) ? "yes" : "NO");
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Constants: one, neg_one, two, neg_two, half, neg_half, zero
// -----------------------------------------------------------
void test_constants() {
    printf("--- xlns16 constants ---\n");
    printf("  constant          hex      got        expected   err%%\n");

    struct { xlns16 val; float expected; const char *name; } consts[] = {
        { xlns16_one,       1.0f,  "xlns16_one" },
        { xlns16_neg_one,  -1.0f,  "xlns16_neg_one" },
        { xlns16_two,       2.0f,  "xlns16_two" },
        { xlns16_neg_two,  -2.0f,  "xlns16_neg_two" },
        { xlns16_half,      0.5f,  "xlns16_half" },
        { xlns16_neg_half, -0.5f,  "xlns16_neg_half" },
        { xlns16_zero,      0.0f,  "xlns16_zero" },
    };

    for (auto& c : consts) {
        float got = xlns162fp(c.val);
        printf("  %-18s 0x%04X  %9.5f  %9.5f  %5.2f%%\n",
               c.name, (unsigned short)c.val, got, c.expected, relerr(got, c.expected));
    }
    printf("\n");

    // Bit-level: neg(pos) == neg_const
    printf("  neg consistency check:\n");
    printf("  operation            computed   const      match\n");
    struct { xlns16 pos; xlns16 neg_c; const char *label; } pairs[] = {
        { xlns16_one,  xlns16_neg_one,  "neg(one)  vs neg_one" },
        { xlns16_two,  xlns16_neg_two,  "neg(two)  vs neg_two" },
        { xlns16_half, xlns16_neg_half, "neg(half) vs neg_half" },
    };
    for (auto& p : pairs) {
        xlns16 computed = xlns16_neg(p.pos);
        printf("  %-20s  0x%04X     0x%04X     %s\n",
               p.label, (unsigned short)computed, (unsigned short)p.neg_c,
               (computed == p.neg_c) ? "yes" : "NO");
    }
    printf("\n");
}

// -----------------------------------------------------------
//  Overflow: test via extreme mul/div that trigger overflow
// -----------------------------------------------------------
void test_overflow() {
    printf("--- xlns16_overflow (via extreme mul/div) ---\n");
    printf("  operation                    got_hex    expected_hex   result         match\n");

    // Construct xlns16 values with near-max log magnitude to force overflow
    xlns16 huge_pos  = 0x7F00;   // very large positive
    xlns16 huge_neg  = 0xFF00;   // very large negative (sign bit set)
    xlns16 tiny_pos  = 0x0100;   // very small positive

    // expected_hex: overflow => sign|logmask, underflow => sign|0
    struct { const char *label; xlns16 a; xlns16 b; bool is_mul; xlns16 expected; const char *exp_tag; } cases[] = {
        { "huge * huge  (overflow)",    huge_pos, huge_pos, true,  0x7FFF, "overflow->max"  },
        { "-huge * huge (overflow)",    huge_neg, huge_pos, true,  0xFFFF, "overflow->max"  },
        { "tiny / huge  (underflow)",   tiny_pos, huge_pos, false, 0x0000, "underflow->0"   },
        { "huge / tiny  (overflow)",    huge_pos, tiny_pos, false, 0x7FFF, "overflow->max"  },
        { "2 * 4        (normal)",      fp2xlns16(2.0f), fp2xlns16(4.0f), true,  fp2xlns16(8.0f), "normal" },
        { "8 / 2        (normal)",      fp2xlns16(8.0f), fp2xlns16(2.0f), false, fp2xlns16(4.0f), "normal" },
    };

    for (auto& c : cases) {
        xlns16 result = c.is_mul ? xlns16_mul(c.a, c.b) : xlns16_div(c.a, c.b);
        const char *got_tag =
            (result == 0x0000 || result == 0x8000)        ? "underflow->0" :
            ((result & xlns16_logmask) == xlns16_logmask) ? "overflow->max" : "normal";
        printf("  %-28s  0x%04X     0x%04X       %-14s %s\n",
               c.label, (unsigned short)result, (unsigned short)c.expected,
               got_tag, (result == c.expected) ? "yes" : "NO");
    }
    printf("\n");
}

int main() {
    printf("=============================================================\n");
    printf("  xlns16 comparisons / utility / constants test              \n");
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
