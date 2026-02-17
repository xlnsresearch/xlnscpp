// Test file for new xlns32 and xlns16 functions
// This tests the batch operations, utility functions, and activation functions

#define xlns32_alt
#include "xlns32.cpp"

#include <cstdio>
#include <cmath>

void test_constants() {
    printf("=== Testing Constants ===\n");
    printf("xlns32_one:     %08x -> %.6f (expected 1.0)\n", xlns32_one, xlns322fp(xlns32_one));
    printf("xlns32_neg_one: %08x -> %.6f (expected -1.0)\n", xlns32_neg_one, xlns322fp(xlns32_neg_one));
    printf("xlns32_two:     %08x -> %.6f (expected 2.0)\n", xlns32_two, xlns322fp(xlns32_two));
    printf("xlns32_half:    %08x -> %.6f (expected 0.5)\n", xlns32_half, xlns322fp(xlns32_half));
    printf("\n");
}

void test_comparisons() {
    printf("=== Testing Comparison Functions ===\n");
    xlns32 a = fp2xlns32(3.14f);
    xlns32 b = fp2xlns32(2.71f);
    xlns32 c = fp2xlns32(-1.5f);
    
    printf("a=3.14, b=2.71, c=-1.5\n");
    printf("xlns32_gt(a, b) = %d (expected 1)\n", xlns32_gt(a, b));
    printf("xlns32_lt(a, b) = %d (expected 0)\n", xlns32_lt(a, b));
    printf("xlns32_is_negative(c) = %d (expected 1)\n", xlns32_is_negative(c));
    printf("xlns32_is_positive(a) = %d (expected 1)\n", xlns32_is_positive(a));
    printf("xlns32_is_zero(xlns32_zero) = %d (expected 1)\n", xlns32_is_zero(xlns32_zero));
    printf("xlns32_max(a, b) = %.6f (expected 3.14)\n", xlns322fp(xlns32_max(a, b)));
    printf("xlns32_min(a, b) = %.6f (expected 2.71)\n", xlns322fp(xlns32_min(a, b)));
    printf("\n");
}

void test_batch_operations() {
    printf("=== Testing Batch Operations ===\n");
    
    float src[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    xlns32 lns_arr[5];
    float dst[5];
    
    // Test batch conversion
    xlns32_batch_from_float(src, lns_arr, 5);
    xlns32_batch_to_float(lns_arr, dst, 5);
    
    printf("Batch conversion roundtrip:\n");
    for (int i = 0; i < 5; i++) {
        printf("  src[%d]=%.1f -> lns -> dst[%d]=%.6f\n", i, src[i], i, dst[i]);
    }
    
    // Test batch multiply
    xlns32 a[3] = {fp2xlns32(2.0f), fp2xlns32(3.0f), fp2xlns32(4.0f)};
    xlns32 b[3] = {fp2xlns32(5.0f), fp2xlns32(6.0f), fp2xlns32(7.0f)};
    xlns32 c[3];
    xlns32_batch_mul(a, b, c, 3);
    printf("Batch multiply: [2*5, 3*6, 4*7] = [%.1f, %.1f, %.1f] (expected 10, 18, 28)\n",
           xlns322fp(c[0]), xlns322fp(c[1]), xlns322fp(c[2]));
    
    // Test batch add
    xlns32_batch_add(a, b, c, 3);
    printf("Batch add: [2+5, 3+6, 4+7] = [%.1f, %.1f, %.1f] (expected 7, 9, 11)\n",
           xlns322fp(c[0]), xlns322fp(c[1]), xlns322fp(c[2]));
    
    // Test batch scale
    xlns32 scalar = fp2xlns32(2.0f);
    xlns32_batch_scale(a, scalar, c, 3);
    printf("Batch scale by 2: [2*2, 3*2, 4*2] = [%.1f, %.1f, %.1f] (expected 4, 6, 8)\n",
           xlns322fp(c[0]), xlns322fp(c[1]), xlns322fp(c[2]));
    printf("\n");
}

void test_vector_operations() {
    printf("=== Testing Vector Operations ===\n");
    
    // Test dot product
    xlns32 a[4] = {fp2xlns32(1.0f), fp2xlns32(2.0f), fp2xlns32(3.0f), fp2xlns32(4.0f)};
    xlns32 b[4] = {fp2xlns32(4.0f), fp2xlns32(3.0f), fp2xlns32(2.0f), fp2xlns32(1.0f)};
    
    xlns32 dot = xlns32_vec_dot(a, b, 4);
    // Expected: 1*4 + 2*3 + 3*2 + 4*1 = 4 + 6 + 6 + 4 = 20
    printf("Dot product [1,2,3,4] · [4,3,2,1] = %.6f (expected 20.0)\n", xlns322fp(dot));
    
    // Test with float inputs
    float fa[] = {1.0f, 2.0f, 3.0f, 4.0f};
    float fb[] = {4.0f, 3.0f, 2.0f, 1.0f};
    float fdot = xlns32_vec_dot_f32(fa, fb, 4);
    printf("Float dot product = %.6f (expected 20.0)\n", fdot);
    
    // Test sum
    xlns32 sum = xlns32_sum(a, 4);
    printf("Sum [1,2,3,4] = %.6f (expected 10.0)\n", xlns322fp(sum));
    
    // Test max/min array
    xlns32 maxval = xlns32_max_array(a, 4);
    xlns32 minval = xlns32_min_array(a, 4);
    printf("Max = %.6f (expected 4.0), Min = %.6f (expected 1.0)\n", 
           xlns322fp(maxval), xlns322fp(minval));
    printf("\n");
}

void test_activation_functions() {
    printf("=== Testing Activation Functions ===\n");
    
    float test_vals[] = {-2.0f, -1.0f, 0.0f, 1.0f, 2.0f};
    
    printf("x\t\tReLU\t\tSigmoid\t\tTanh\t\tSiLU\t\tGELU\n");
    for (int i = 0; i < 5; i++) {
        xlns32 x = fp2xlns32(test_vals[i]);
        float fx = test_vals[i];
        
        xlns32 relu = xlns32_relu(x);
        xlns32 sigmoid = xlns32_sigmoid(x);
        xlns32 tanh_val = xlns32_tanh(x);
        xlns32 silu = xlns32_silu(x);
        xlns32 gelu = xlns32_gelu(x);
        
        // Expected values
        float exp_relu = fmaxf(0.0f, fx);
        float exp_sigmoid = 1.0f / (1.0f + expf(-fx));
        float exp_tanh = tanhf(fx);
        float exp_silu = fx / (1.0f + expf(-fx));
        
        printf("%.1f\t\t%.4f\t\t%.4f\t\t%.4f\t\t%.4f\t\t%.4f\n",
               fx, xlns322fp(relu), xlns322fp(sigmoid), 
               xlns322fp(tanh_val), xlns322fp(silu), xlns322fp(gelu));
    }
    printf("\n");
}

void test_fma() {
    printf("=== Testing FMA (Fused Multiply-Add) ===\n");
    xlns32 a = fp2xlns32(2.0f);
    xlns32 b = fp2xlns32(3.0f);
    xlns32 c = fp2xlns32(4.0f);
    
    xlns32 result = xlns32_fma(a, b, c);  // 2*3 + 4 = 10
    printf("fma(2, 3, 4) = 2*3 + 4 = %.6f (expected 10.0)\n", xlns322fp(result));
    printf("\n");
}

void test_square() {
    printf("=== Testing Square ===\n");
    xlns32 x = fp2xlns32(5.0f);
    xlns32 sq = xlns32_square(x);
    printf("square(5) = %.6f (expected 25.0)\n", xlns322fp(sq));
    
    x = fp2xlns32(-3.0f);
    sq = xlns32_square(x);
    printf("square(-3) = %.6f (expected 9.0)\n", xlns322fp(sq));
    printf("\n");
}

int main() {
    printf("   Testing New xlns32 Functions for ggml Backend   \n");
    
    test_constants();
    test_comparisons();
    test_batch_operations();
    test_vector_operations();
    test_activation_functions();
    test_fma();
    test_square();
    
    printf("All tests completed!\n");
    return 0;
}
