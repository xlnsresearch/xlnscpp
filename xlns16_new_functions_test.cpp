// Test file for new xlns16 functions
// This tests the batch operations, utility functions, and activation functions

#define xlns16_alt
#include "xlns16.cpp"

#include <cstdio>
#include <cmath>

void test_constants() {
    printf("=== Testing Constants (xlns16) ===\n");
    printf("xlns16_one:     %04x -> %.6f (expected 1.0)\n", xlns16_one, xlns162fp(xlns16_one));
    printf("xlns16_neg_one: %04x -> %.6f (expected -1.0)\n", xlns16_neg_one, xlns162fp(xlns16_neg_one));
    printf("xlns16_two:     %04x -> %.6f (expected 2.0)\n", xlns16_two, xlns162fp(xlns16_two));
    printf("xlns16_half:    %04x -> %.6f (expected 0.5)\n", xlns16_half, xlns162fp(xlns16_half));
    printf("\n");
}

void test_comparisons() {
    printf("=== Testing Comparison Functions (xlns16) ===\n");
    xlns16 a = fp2xlns16(3.14f);
    xlns16 b = fp2xlns16(2.71f);
    xlns16 c = fp2xlns16(-1.5f);
    
    printf("a=3.14, b=2.71, c=-1.5\n");
    printf("xlns16_gt(a, b) = %d (expected 1)\n", xlns16_gt(a, b));
    printf("xlns16_lt(a, b) = %d (expected 0)\n", xlns16_lt(a, b));
    printf("xlns16_is_negative(c) = %d (expected 1)\n", xlns16_is_negative(c));
    printf("xlns16_is_positive(a) = %d (expected 1)\n", xlns16_is_positive(a));
    printf("xlns16_is_zero(xlns16_zero) = %d (expected 1)\n", xlns16_is_zero(xlns16_zero));
    printf("xlns16_max(a, b) = %.6f (expected ~3.14)\n", xlns162fp(xlns16_max(a, b)));
    printf("xlns16_min(a, b) = %.6f (expected ~2.71)\n", xlns162fp(xlns16_min(a, b)));
    printf("\n");
}

void test_batch_operations() {
    printf("=== Testing Batch Operations (xlns16) ===\n");
    
    float src[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    xlns16 lns_arr[5];
    float dst[5];
    
    // Test batch conversion
    xlns16_batch_from_float(src, lns_arr, 5);
    xlns16_batch_to_float(lns_arr, dst, 5);
    
    printf("Batch conversion roundtrip:\n");
    for (int i = 0; i < 5; i++) {
        printf("  src[%d]=%.1f -> lns -> dst[%d]=%.6f\n", i, src[i], i, dst[i]);
    }
    
    // Test batch multiply
    xlns16 a[3] = {fp2xlns16(2.0f), fp2xlns16(3.0f), fp2xlns16(4.0f)};
    xlns16 b[3] = {fp2xlns16(5.0f), fp2xlns16(6.0f), fp2xlns16(7.0f)};
    xlns16 c[3];
    xlns16_batch_mul(a, b, c, 3);
    printf("Batch multiply: [2*5, 3*6, 4*7] = [%.1f, %.1f, %.1f] (expected 10, 18, 28)\n",
           xlns162fp(c[0]), xlns162fp(c[1]), xlns162fp(c[2]));
    
    // Test batch add
    xlns16_batch_add(a, b, c, 3);
    printf("Batch add: [2+5, 3+6, 4+7] = [%.1f, %.1f, %.1f] (expected 7, 9, 11)\n",
           xlns162fp(c[0]), xlns162fp(c[1]), xlns162fp(c[2]));
    printf("\n");
}

void test_vector_operations() {
    printf("=== Testing Vector Operations (xlns16) ===\n");
    
    // Test dot product with float inputs
    float fa[] = {1.0f, 2.0f, 3.0f, 4.0f};
    float fb[] = {4.0f, 3.0f, 2.0f, 1.0f};
    float fdot = xlns16_vec_dot_f32(fa, fb, 4);
    // Expected: 1*4 + 2*3 + 3*2 + 4*1 = 20
    printf("Float dot product = %.6f (expected ~20.0)\n", fdot);
    
    // Test sum
    xlns16 a[4] = {fp2xlns16(1.0f), fp2xlns16(2.0f), fp2xlns16(3.0f), fp2xlns16(4.0f)};
    xlns16 sum = xlns16_sum(a, 4);
    printf("Sum [1,2,3,4] = %.6f (expected ~10.0)\n", xlns162fp(sum));
    printf("\n");
}

void test_activation_functions() {
    printf("=== Testing Activation Functions (xlns16) ===\n");
    
    float test_vals[] = {-2.0f, -1.0f, 0.0f, 1.0f, 2.0f};
    
    printf("x\t\tReLU\t\tSigmoid\t\tSiLU\n");
    for (int i = 0; i < 5; i++) {
        xlns16 x = fp2xlns16(test_vals[i]);
        float fx = test_vals[i];
        
        xlns16 relu = xlns16_relu(x);
        xlns16 sigmoid = xlns16_sigmoid(x);
        xlns16 silu = xlns16_silu(x);
        
        printf("%.1f\t\t%.4f\t\t%.4f\t\t%.4f\n",
               fx, xlns162fp(relu), xlns162fp(sigmoid), xlns162fp(silu));
    }
    printf("\n");
}

int main() {
    printf("===================================================\n");
    printf("   Testing New xlns16 Functions for ggml Backend   \n");
    printf("===================================================\n\n");
    
    test_constants();
    test_comparisons();
    test_batch_operations();
    test_vector_operations();
    test_activation_functions();
    
    printf("All xlns16 tests completed!\n");
    return 0;
}
