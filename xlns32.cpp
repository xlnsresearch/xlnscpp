// 32-bit XLNS for C++
// copyright 1999-2025 Mark G. Arnold
// these routines 
//    demonstrated linear interpolation and cotransformation (not xlns32-ideal)
//    ran on 16-bit Turbo C/C++ (I think xlns32_arch16 revives that code)
//    were used in my PhD research and for several later papers
// they were ported to Linux gcc and g++ around 2015 on 32-bit x86
// they were ported again for 64-bit arch in 2025, with the xlns32_ideal option
// they were modified with xlns32_alt for streamlined + on modern arch w/ ovfl detect 
// they are based on similar math foundation (Gaussian logs, sb and db) as Python xlns,
//    but use different internal storage format:
//    +------+-------------------------+
//    | sign | int(log2) . frac(log2)  |
//    +------+-------------------------+
//    the int(log2) is not twos complement; it is offset (logsignmask XORed)
//    for the 32-bit format in this file, this is roughly similar to IEEE 754 float32
//    there is an exact representation of 0.0, but no subnormals or NaNs

//#define xlns32_arch16
/* PORTABLE CODE STARTS HERE*/

#include <stdio.h>
#include <stdlib.h>

// choose long (for 16-bit arch) or int (for 32- or 64-bit arch)
//   perhaps the following might work in either case:
//     typedef u_int32_t xlns32;
//     typedef int32_t xlns32_signed;

#ifdef xlns32_arch16
  typedef unsigned long xlns32;
  typedef signed long xlns32_signed;
  #define xlns32_zero          0x00000000L
  #define xlns32_scale         0x00800000L
  #define xlns32_logmask       0x7fffffffL
  #define xlns32_signmask      0x80000000L
  #define xlns32_logsignmask   0x40000000L
  #define xlns32_canonmask     0x80000000L
  #define xlns32_sqrtmask      0x20000000L
  #define xlns32_esszer        0x0cfa0000L
  #define xlns32_canonshift    31
#else
  typedef unsigned int xlns32;
  typedef signed int xlns32_signed;
  #define xlns32_zero          0x00000000
  #define xlns32_scale         0x00800000
  #define xlns32_logmask       0x7fffffff
  #define xlns32_signmask      0x80000000
  #define xlns32_logsignmask   0x40000000
  #define xlns32_canonmask     0x80000000
  #define xlns32_sqrtmask      0x20000000
  #define xlns32_esszer        0x0cfa0000
  #define xlns32_canonshift    31
#endif

// Useful constant values (pre-computed LNS representations)
// xlns32_one:     log2(1.0) = 0, so internal = 0 ^ logsignmask = 0x40000000
// xlns32_two:     log2(2.0) = 1, scaled = 0x00800000, internal = 0x00800000 ^ 0x40000000 = 0x40800000
// xlns32_half:    log2(0.5) = -1, scaled = -0x00800000 = 0xFF800000 (as signed), 
//                 but we use abs and XOR: internal = 0x3F800000
// xlns32_neg_one: same as xlns32_one but with sign bit set
#define xlns32_one          0x40000000
#define xlns32_neg_one      0xC0000000
#define xlns32_two          0x40800000
#define xlns32_neg_two      0xC0800000
#define xlns32_half         0x3F800000
#define xlns32_neg_half     0xBF800000

// Basic unary operations (macros for efficiency)
#define xlns32_sign(x)  ((x) & xlns32_signmask)
#define xlns32_neg(x)   ((x) ^ xlns32_signmask)
#define xlns32_abs(x)   ((x) & xlns32_logmask)
#define xlns32_recip(x) (xlns32_sign(x)|xlns32_abs((~x)+1))
#define xlns32_sqrt(x)   (xlns32_abs(((xlns32_signed)((x)<<1))/4)^xlns32_sqrtmask)
#define xlns32_canon(x) ((x)^(-((x)>>xlns32_canonshift)|xlns32_signmask))

// Square: x^2 (efficient in LNS: double the log)
#define xlns32_square(x) xlns32_mul((x), (x))

inline xlns32 xlns32_overflow(xlns32 xlns32_x, xlns32 xlns32_y, xlns32 xlns32_temp)
{       //printf("%ld %ld %ld\n",xlns32_temp,xlns32_x,xlns32_y);
	if (xlns32_logsignmask&xlns32_temp)
	{
		return (xlns32_signmask&(xlns32_x^xlns32_y));
	}
	else
	{
		return (xlns32_signmask&(xlns32_x^xlns32_y))| xlns32_logmask;
	}
}

inline xlns32 xlns32_mul(xlns32 x, xlns32 y)
{
   xlns32 xlns32_temp;
   //xlns32_x = x;
   //xlns32_y=y;
   xlns32_temp=(xlns32_logmask&(x))+(xlns32_logmask&(y))-xlns32_logsignmask; 
   return  (xlns32_signmask&(xlns32_temp)) ? xlns32_overflow(x,y,xlns32_temp) 
                                       :(xlns32_signmask&(x^y))|xlns32_temp;
}

inline xlns32 xlns32_div(xlns32 x, xlns32 y)
{
   xlns32 xlns32_temp;
   //xlns32_x = x;
   //xlns32_y=y;
   xlns32_temp=(xlns32_logmask&(x))-(xlns32_logmask&(y))+xlns32_logsignmask; 
   return  (xlns32_signmask&(xlns32_temp)) ? xlns32_overflow(x,y,xlns32_temp) 
                                       :(xlns32_signmask&(x^y))|xlns32_temp;
}

#ifdef xlns32_ideal
  #define xlns32_sb xlns32_sb_ideal
  #define xlns32_db xlns32_db_ideal
  #include <math.h>
  inline xlns32 xlns32_sb_ideal(xlns32 z)
  {
	return ((xlns32) ((log(1+ pow(2.0, ((double) z) / xlns32_scale) )/log(2.0))*xlns32_scale+.5));
  }
  inline xlns32 xlns32_db_ideal(xlns32 z)
  {
	return ((xlns32_signed) ((log( pow(2.0, ((double) z) / xlns32_scale) - 1 )/log(2.0))*xlns32_scale+.5));
  }
#else
  #define xlns32_sb xlns32_sb_macro
  #define xlns32_db xlns32_dbtrans3

#define xlns32_tablesize     (4096*3+1000)
#define xlns32_zhmask        0x0fffc000L
#define xlns32_zlmask        0x00003fffL
#define xlns32_zhshift       14

#define xlns32_db0shift      19
#define xlns32_db1shift      10
#define xlns32_db0mask       0x0ff80000L
#define xlns32_db1mask       0x0007fc00L
#define xlns32_db2mask       0x000003ffL
#define xlns32_db0size       512
#define xlns32_db1size       512
#define xlns32_db2size       1024

#include "xlns32tbl.h"

xlns32 xlns32_z, xlns32_zh;

#define xlns32_sb_macro(z) \
(  \
((xlns32_zh=(xlns32_z=(z))>>xlns32_zhshift)>=(xlns32_tablesize-1))?xlns32_z: \
( \
 ( (xlns32_z&xlns32_zhmask)  \
   +(((xlns32)xlns32_sbhtable[xlns32_zh])<<16)+xlns32_sbltable[xlns32_zh]) \
 +( \
   ( \
    (0x4000-(xlns32_zlmask&  \
      ((xlns32_sbltable[xlns32_zh]-xlns32_sbltable[xlns32_zh+1])) \
    )) \
    *(xlns32_z&xlns32_zlmask)  \
   )>>xlns32_zhshift  \
  )  \
) \
)

//xlns32_signed xlns32_sb(xlns32_signed z)
//xlns32 xlns32_sb(xlns32 z)
//{
//	xlns32 xlns32_z, xlns32_zh;
//	return xlns32_sb_macro(z);
//}

//xlns32_signed xlns32_dbtrans3(xlns32_signed z)
xlns32 xlns32_dbtrans3(xlns32 z)
{
	xlns32 z0,z1,z2,temp2;
	z0 =  z & xlns32_db0mask;
	z1 =  z & xlns32_db1mask;
	z2 =  z & xlns32_db2mask;
	if (z1 == 0)
	{
		if (z2 == 0)
		{
			if (z0 == 0)
				return 0;
			else
				return xlns32_db0table[z0 >> xlns32_db0shift];
		}
		else if (z0 == 0)
			return xlns32_db2table[z2];
		else
			return xlns32_db2table[z2] +
			  xlns32_sb(z2 +
		      xlns32_db0table[z0>> xlns32_db0shift] - xlns32_db2table[z2]);
	}
	else if (z2 == 0)
	{
		if (z0 == 0)
			return xlns32_db1table[z1 >> xlns32_db1shift];
		else
			return xlns32_db1table[z1 >> xlns32_db1shift] +
			  xlns32_sb(z1 +
		      xlns32_db0table[z0>> xlns32_db0shift] - xlns32_db1table[z1 >> xlns32_db1shift]);
	}
	else
	{
		if (z0 == 0)
			return xlns32_db2table[z2] +
		      xlns32_sb(z2 +
			  xlns32_db1table[z1>> xlns32_db1shift] - xlns32_db2table[z2]);
		else
		{
			temp2= xlns32_sb(z1 +
				xlns32_db0table[z0>> xlns32_db0shift] - xlns32_db1table[z1 >> xlns32_db1shift]);
			return xlns32_db2table[z2] +
			  xlns32_sb(z2 +
				xlns32_db1table[z1 >> xlns32_db1shift] +
				temp2
		     - xlns32_db2table[z2]);
		}
	}
}

#endif

#ifdef xlns32_alt

inline xlns32 xlns32_add(xlns32 x, xlns32 y)
{
    xlns32 minxyl, maxxy, xl, yl, usedb; 
    xlns32_signed adjust, adjustez;
    xlns32_signed z;
    xl = x & xlns32_logmask;
    yl = y & xlns32_logmask;
    minxyl = (yl>xl) ? xl : yl;
    maxxy  = (xl>yl) ? x  : y;
    z = minxyl - (maxxy&xlns32_logmask);
    usedb = xlns32_signmask&(x^y); 
    #ifdef xlns32_ideal
     float pm1 = usedb ? -1.0 : 1.0;
     //adjust = log(1.0 + pm1*pow(2.0,((double)z)/xlns32_scale))/log(2.0)*xlns32_scale+.5;
     adjust = z+((xlns32_signed)(log(pm1+pow(2.0,-((double)z)/xlns32_scale))/log(2.0)*xlns32_scale+.5));
    #else
     adjust = usedb ? z + ((xlns32_signed)xlns32_db(-z)) : 
                      z + ((xlns32_signed)xlns32_sb(-z)); 
    #endif
    adjustez = (z < -xlns32_esszer) ? 0 : adjust; 
    //printf("z=%d %d %d\n",z,adjust,adjustez);
    return ((z==0) && usedb) ? 
                     xlns32_zero :
                     xlns32_mul(maxxy, xlns32_logsignmask + adjustez);
}

#else

//++++ X-X ERROR fixed

xlns32 xlns32_add(xlns32 x, xlns32 y)
{
	xlns32 t;
	xlns32_signed z;

	z = (x&xlns32_logmask) - (y&xlns32_logmask);
	if (z<0)
	{
		z = -z;
		t = x;
		x = y;
		y = t;
	}
	if (xlns32_signmask&(x^y))
	{
		if (z == 0)
			return xlns32_zero;
		if (z < xlns32_esszer)
			return xlns32_neg(y + xlns32_db(z));
		else
			return xlns32_neg(y+z);
	}
	else
	{
		return y + xlns32_sb(z);
	}
}
#endif

#define xlns32_sub(x,y) xlns32_add(x,xlns32_neg(y))

/*END OF PORTABLE CODE*/

/*START OF PORTABLE CODE THAT DEPENDS ON <math.h>*/

#include <math.h>

xlns32 fp2xlns32(float x)
{
	if (x==0.0)
		return(xlns32_zero);
	else if (x > 0.0)
		return xlns32_abs((xlns32_signed) ((log(x)/log(2.0))*xlns32_scale))
		       ^xlns32_logsignmask;
	else
		return (((xlns32_signed) ((log(fabs(x))/log(2.0))*xlns32_scale))
			  |xlns32_signmask)^xlns32_logsignmask;
}

float xlns322fp(xlns32 x)
{
	if (xlns32_abs(x) == xlns32_zero)
		return (0.0);
	else if (xlns32_sign(x))
		return (float) (-pow(2.0,((double) (((xlns32_signed) (xlns32_abs(x)-xlns32_logsignmask))))
					/((float) xlns32_scale)));
	else {
		return (float) (+pow(2.0,((double) (((xlns32_signed) (xlns32_abs(x)-xlns32_logsignmask))))
					/((float) xlns32_scale)));
	//else if (xlns32_sign(x))
	//	return (float) (-pow(2.0,((double) ((xlns32_signed) xlns32_abs(x^xlns32_logsignmask)<<1)/2)
	//				/((float) xlns32_scale)));
	//else {
	//	return (float) (+pow(2.0,((double) ((xlns32_signed) (x^xlns32_logsignmask)<<1)/2)
	//				/((float) xlns32_scale)));
	}
}

// Comparison and utility functions

// Check if value is zero
inline int xlns32_is_zero(xlns32 x) {
    return (xlns32_abs(x) == xlns32_zero);
}

// Check if value is negative
inline int xlns32_is_negative(xlns32 x) {
    return (xlns32_sign(x) != 0) && !xlns32_is_zero(x);
}

// Check if value is positive
inline int xlns32_is_positive(xlns32 x) {
    return (xlns32_sign(x) == 0) && !xlns32_is_zero(x);
}

// Greater than comparison (returns 1 if a > b)
inline int xlns32_gt(xlns32 a, xlns32 b) {
    return xlns32_canon(a) > xlns32_canon(b);
}

// Less than comparison (returns 1 if a < b)
inline int xlns32_lt(xlns32 a, xlns32 b) {
    return xlns32_canon(a) < xlns32_canon(b);
}

// Equal comparison
inline int xlns32_eq(xlns32 a, xlns32 b) {
    return a == b;
}

// Greater than or equal
inline int xlns32_ge(xlns32 a, xlns32 b) {
    return xlns32_canon(a) >= xlns32_canon(b);
}

// Less than or equal
inline int xlns32_le(xlns32 a, xlns32 b) {
    return xlns32_canon(a) <= xlns32_canon(b);
}

// Maximum of two values
inline xlns32 xlns32_max(xlns32 a, xlns32 b) {
    return xlns32_gt(a, b) ? a : b;
}

// Minimum of two values
inline xlns32 xlns32_min(xlns32 a, xlns32 b) {
    return xlns32_lt(a, b) ? a : b;
}

// Copy sign from y to x (magnitude of x, sign of y)
inline xlns32 xlns32_copysign(xlns32 x, xlns32 y) {
    return xlns32_abs(x) | xlns32_sign(y);
}

// Fused multiply-add: a * b + c
inline xlns32 xlns32_fma(xlns32 a, xlns32 b, xlns32 c) {
    return xlns32_add(xlns32_mul(a, b), c);
}

// Batch conversion functions (for ggml tensor operations)

// Batch convert float array to xlns32 array
inline void xlns32_batch_from_float(const float *src, xlns32 *dst, size_t n) {
    for (size_t i = 0; i < n; i++) {
        dst[i] = fp2xlns32(src[i]);
    }
}

// Batch convert xlns32 array to float array
inline void xlns32_batch_to_float(const xlns32 *src, float *dst, size_t n) {
    for (size_t i = 0; i < n; i++) {
        dst[i] = xlns322fp(src[i]);
    }
}

// Batch element-wise operations

// Batch multiplication: c[i] = a[i] * b[i]
inline void xlns32_batch_mul(const xlns32 *a, const xlns32 *b, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_mul(a[i], b[i]);
    }
}

// Batch addition: c[i] = a[i] + b[i]
inline void xlns32_batch_add(const xlns32 *a, const xlns32 *b, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_add(a[i], b[i]);
    }
}

// Batch subtraction: c[i] = a[i] - b[i]
inline void xlns32_batch_sub(const xlns32 *a, const xlns32 *b, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_sub(a[i], b[i]);
    }
}

// Batch division: c[i] = a[i] / b[i]
inline void xlns32_batch_div(const xlns32 *a, const xlns32 *b, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_div(a[i], b[i]);
    }
}

// Batch scale: c[i] = a[i] * scalar
inline void xlns32_batch_scale(const xlns32 *a, xlns32 scalar, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_mul(a[i], scalar);
    }
}

// Batch negation: c[i] = -a[i]
inline void xlns32_batch_neg(const xlns32 *a, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_neg(a[i]);
    }
}

// Batch absolute value: c[i] = |a[i]|
inline void xlns32_batch_abs(const xlns32 *a, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_abs(a[i]);
    }
}

// Vector operations (critical for ggml MUL_MAT)

// Sum of array elements: result = Σ a[i]
inline xlns32 xlns32_sum(const xlns32 *a, size_t n) {
    if (n == 0) return xlns32_zero;
    xlns32 sum = a[0];
    for (size_t i = 1; i < n; i++) {
        sum = xlns32_add(sum, a[i]);
    }
    return sum;
}

// Vector dot product: result = Σ(a[i] * b[i])
inline xlns32 xlns32_vec_dot(const xlns32 *a, const xlns32 *b, size_t n) {
    if (n == 0) return xlns32_zero;
    xlns32 sum = xlns32_mul(a[0], b[0]);
    for (size_t i = 1; i < n; i++) {
        sum = xlns32_add(sum, xlns32_mul(a[i], b[i]));
    }
    return sum;
}

// Vector dot product with float inputs (converts to LNS internally)
inline float xlns32_vec_dot_f32(const float *a, const float *b, size_t n) {
    if (n == 0) return 0.0f;
    xlns32 sum = xlns32_mul(fp2xlns32(a[0]), fp2xlns32(b[0]));
    for (size_t i = 1; i < n; i++) {
        xlns32 prod = xlns32_mul(fp2xlns32(a[i]), fp2xlns32(b[i]));
        sum = xlns32_add(sum, prod);
    }
    return xlns322fp(sum);
}

// Maximum element in array
inline xlns32 xlns32_max_array(const xlns32 *a, size_t n) {
    if (n == 0) return xlns32_zero;
    xlns32 maxval = a[0];
    for (size_t i = 1; i < n; i++) {
        if (xlns32_gt(a[i], maxval)) {
            maxval = a[i];
        }
    }
    return maxval;
}

// Minimum element in array
inline xlns32 xlns32_min_array(const xlns32 *a, size_t n) {
    if (n == 0) return xlns32_zero;
    xlns32 minval = a[0];
    for (size_t i = 1; i < n; i++) {
        if (xlns32_lt(a[i], minval)) {
            minval = a[i];
        }
    }
    return minval;
}

// Activation functions (for neural network operations)

// ReLU: max(0, x)
inline xlns32 xlns32_relu(xlns32 x) {
    return xlns32_is_negative(x) ? xlns32_zero : x;
}

// Batch ReLU
inline void xlns32_batch_relu(const xlns32 *a, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_relu(a[i]);
    }
}

// Sigmoid: 1 / (1 + exp(-x))
// Note: Uses float conversion for exp() then converts back
inline xlns32 xlns32_sigmoid(xlns32 x) {
    float fx = xlns322fp(x);
    float result = 1.0f / (1.0f + expf(-fx));
    return fp2xlns32(result);
}

// Batch sigmoid
inline void xlns32_batch_sigmoid(const xlns32 *a, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_sigmoid(a[i]);
    }
}

// Tanh: (exp(x) - exp(-x)) / (exp(x) + exp(-x))
inline xlns32 xlns32_tanh(xlns32 x) {
    float fx = xlns322fp(x);
    float result = tanhf(fx);
    return fp2xlns32(result);
}

// Batch tanh
inline void xlns32_batch_tanh(const xlns32 *a, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_tanh(a[i]);
    }
}

// SiLU (Swish): x * sigmoid(x) = x / (1 + exp(-x))
inline xlns32 xlns32_silu(xlns32 x) {
    float fx = xlns322fp(x);
    float result = fx / (1.0f + expf(-fx));
    return fp2xlns32(result);
}

// Batch SiLU
inline void xlns32_batch_silu(const xlns32 *a, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_silu(a[i]);
    }
}

// GELU approximation: 0.5 * x * (1 + tanh(sqrt(2/pi) * (x + 0.044715 * x^3)))
inline xlns32 xlns32_gelu(xlns32 x) {
    float fx = xlns322fp(x);
    const float sqrt_2_over_pi = 0.7978845608f;  // sqrt(2/pi)
    float inner = sqrt_2_over_pi * (fx + 0.044715f * fx * fx * fx);
    float result = 0.5f * fx * (1.0f + tanhf(inner));
    return fp2xlns32(result);
}

// Batch GELU
inline void xlns32_batch_gelu(const xlns32 *a, xlns32 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns32_gelu(a[i]);
    }
}

// Softmax helper: subtract max for numerical stability, then exp
// Note: This is a building block; full softmax requires normalization
inline void xlns32_softmax_exp(const xlns32 *a, xlns32 *c, size_t n) {
    // Find max for numerical stability
    xlns32 maxval = xlns32_max_array(a, n);
    for (size_t i = 0; i < n; i++) {
        // c[i] = exp(a[i] - max)
        float fx = xlns322fp(a[i]) - xlns322fp(maxval);
        c[i] = fp2xlns32(expf(fx));
    }
}

// exp and log in LNS (optimized versions)

// Note: These can potentially be optimized further since LNS 
// stores log2(x), so exp2/log2 can be more efficient

// exp(x) - computes e^x
inline xlns32 xlns32_exp(xlns32 x) {
    float fx = xlns322fp(x);
    return fp2xlns32(expf(fx));
}

// log(x) - computes natural log
inline xlns32 xlns32_log(xlns32 x) {
    float fx = xlns322fp(x);
    if (fx <= 0.0f) return xlns32_zero;  // Handle invalid input
    return fp2xlns32(logf(fx));
}

// exp2(x) - computes 2^x (efficient in LNS: just scale the value)
// In LNS, if y = log2(val), then 2^x * val has log2 = y + x
// So this shifts the log representation
inline xlns32 xlns32_exp2(xlns32 x) {
    float fx = xlns322fp(x);
    return fp2xlns32(exp2f(fx));
}

// log2(x) - computes log base 2 (very efficient in LNS)
inline xlns32 xlns32_log2(xlns32 x) {
    float fx = xlns322fp(x);
    if (fx <= 0.0f) return xlns32_zero;
    return fp2xlns32(log2f(fx));
}

// pow(base, exp) - computes base^exp
inline xlns32 xlns32_pow(xlns32 base, xlns32 exponent) {
    float fbase = xlns322fp(base);
    float fexp = xlns322fp(exponent);
    if (fbase <= 0.0f) return xlns32_zero;
    return fp2xlns32(powf(fbase, fexp));
}



// Full softmax: exp(a[i]-max) / sum(exp(a[j]-max)), using LNS primitives
inline void xlns32_softmax(const xlns32 *a, xlns32 *c, size_t n) {
    if (n == 0) return;
    xlns32 maxval = xlns32_max_array(a, n);
    for (size_t i = 0; i < n; i++)
        c[i] = xlns32_exp(xlns32_sub(a[i], maxval));
    xlns32 total = xlns32_sum(c, n);
    for (size_t i = 0; i < n; i++)
        c[i] = xlns32_div(c[i], total);
}


// Layer normalization: (x - mean) / sqrt(var + eps) * gamma + beta
inline void xlns32_layernorm(const xlns32 *x, xlns32 *out,
                       const xlns32 *gamma, const xlns32 *beta,
                       size_t n, float eps) {
    // compute mean
    xlns32 mean = xlns32_sum(x, n);
    mean = xlns32_div(mean, fp2xlns32((float)n));
    // compute variance
    xlns32 var = xlns32_zero;
    for (size_t i = 0; i < n; i++) {
        xlns32 diff = xlns32_sub(x[i], mean);
        var = xlns32_add(var, xlns32_mul(diff, diff));
    }
    var = xlns32_div(var, fp2xlns32((float)n));
    // normalize
    xlns32 inv_std = fp2xlns32(1.0f / sqrtf(xlns322fp(var) + eps));
    for (size_t i = 0; i < n; i++) {
        out[i] = xlns32_mul(xlns32_sub(x[i], mean), inv_std);
        if (gamma) out[i] = xlns32_mul(out[i], gamma[i]);
        if (beta)  out[i] = xlns32_add(out[i], beta[i]);
    }
}

/*END OF PORTABLE CODE THAT DEPENDS ON <math.h>*/




#include <iostream>

class xlns32_float {
    xlns32 x;
 public:
    friend xlns32_float operator+(xlns32_float , xlns32_float );
    friend xlns32_float operator+(float, xlns32_float );
    friend xlns32_float operator+(xlns32_float , float);
    friend xlns32_float operator-(xlns32_float , xlns32_float );
    friend xlns32_float operator-(float, xlns32_float );
    friend xlns32_float operator-(xlns32_float , float);
    friend xlns32_float operator*(xlns32_float , xlns32_float );
    friend xlns32_float operator*(float, xlns32_float );
    friend xlns32_float operator*(xlns32_float , float);
    friend xlns32_float operator/(xlns32_float , xlns32_float );
    friend xlns32_float operator/(float, xlns32_float );
    friend xlns32_float operator/(xlns32_float , float);
    xlns32_float operator=(float);
    friend xlns32 xlns32_internal(xlns32_float );
    friend float xlns32_2float(xlns32_float );
    friend xlns32_float float2xlns32_(float);
    friend std::ostream& operator<<(std::ostream&, xlns32_float );
    friend xlns32_float operator-(xlns32_float);
    friend xlns32_float operator+=(xlns32_float &, xlns32_float);
    friend xlns32_float operator+=(xlns32_float &, float);
    friend xlns32_float operator-=(xlns32_float &, xlns32_float);
    friend xlns32_float operator-=(xlns32_float &, float);
    friend xlns32_float operator*=(xlns32_float &, xlns32_float);
    friend xlns32_float operator*=(xlns32_float &, float);
    friend xlns32_float operator/=(xlns32_float &, xlns32_float);
    friend xlns32_float operator/=(xlns32_float &, float);
    friend xlns32_float sin(xlns32_float);
    friend xlns32_float cos(xlns32_float);
    friend xlns32_float exp(xlns32_float);
    friend xlns32_float log(xlns32_float);
    friend xlns32_float atan(xlns32_float);
    friend xlns32_float abs(xlns32_float);
    friend xlns32_float sqrt(xlns32_float);
    friend xlns32_float operator-(xlns32_float);
    friend int operator==(xlns32_float arg1, xlns32_float arg2)
      {
       return (arg1.x == arg2.x);
      }
    friend int operator!=(xlns32_float arg1, xlns32_float arg2)
      {
       return (arg1.x != arg2.x);
      }
    friend int operator<=(xlns32_float arg1, xlns32_float arg2)
      {
       return (xlns32_canon(arg1.x)<=xlns32_canon(arg2.x));
      }
    friend int operator>=(xlns32_float arg1, xlns32_float arg2)
      {
       return (xlns32_canon(arg1.x)>=xlns32_canon(arg2.x));
      }
    friend int operator<(xlns32_float arg1, xlns32_float arg2)
      {
       return (xlns32_canon(arg1.x)<xlns32_canon(arg2.x));
      }
    friend int operator>(xlns32_float arg1, xlns32_float arg2)
      {
       return (xlns32_canon(arg1.x)>xlns32_canon(arg2.x));
      }
    friend int operator==(xlns32_float arg1, float arg2);
    friend int operator!=(xlns32_float arg1, float arg2);
    friend int operator<=(xlns32_float arg1, float arg2);
    friend int operator>=(xlns32_float arg1, float arg2);
    friend int operator<(xlns32_float arg1, float arg2);
    friend int operator>(xlns32_float arg1, float arg2);
  };




/*access function for internal representation*/

xlns32 xlns32_internal(xlns32_float y) {
    return y.x;
}


float xlns32_2float(xlns32_float y) {
	return xlns322fp(y.x);
}

#define xlns32_cachesize 1024
xlns32 xlns32_cachecontent[xlns32_cachesize];
float xlns32_cachetag[xlns32_cachesize];
long xlns32_misses=0;
long xlns32_hits=0;
#define xlns32_cacheon 1

xlns32_float float2xlns32_(float y) {
	xlns32_float z;
	unsigned char * fpbyte;
	int addr;
	fpbyte=(unsigned char *)(&y);
	addr = (fpbyte[2])^(fpbyte[3]<<2);
	if ((xlns32_cachetag[addr] ==  y)&&xlns32_cacheon)
	{
//	  printf("hit  %f  %02x %02x %02x %02x addr=%d\n",y, fpbyte[0],fpbyte[1],fpbyte[2],fpbyte[3],addr);
	  z.x = xlns32_cachecontent[addr];
	  xlns32_hits++;
	}
	else
	{
//	  printf("miss %f  %02x %02x %02x %02x addr=%d\n",y, fpbyte[0],fpbyte[1],fpbyte[2],fpbyte[3],addr);
	  z.x = fp2xlns32(y);
	  xlns32_cachecontent[addr] = z.x;
	  xlns32_cachetag[addr] = y;
	  xlns32_misses++;
	}
//	getchar();
	return z;
}


/*overload stream output << operator*/

//#include <ostream>
std::ostream& operator<< (std::ostream& s, xlns32_float  y) {
    return s << xlns32_2float(y);
}

xlns32_float operator-(xlns32_float arg1) {
   xlns32_float z;
   z.x=xlns32_neg(arg1.x);
   return z;
}



xlns32_float operator+(xlns32_float arg1, xlns32_float arg2) {
   xlns32_float z;
   z.x=xlns32_add(arg1.x,arg2.x);
   return z;
}

xlns32_float operator-(xlns32_float arg1, xlns32_float arg2) {
   xlns32_float z;
   z.x=xlns32_sub(arg1.x,arg2.x);
   return z;
}

xlns32_float operator*(xlns32_float arg1, xlns32_float arg2) {
   xlns32_float z;
   z.x=xlns32_mul(arg1.x,arg2.x);
   return z;
}

xlns32_float operator/(xlns32_float arg1, xlns32_float arg2) {
   xlns32_float z;
   z.x=xlns32_div(arg1.x,arg2.x);
   return z;
}


/*operators with auto type conversion*/

xlns32_float operator+(float arg1, xlns32_float arg2) {
   return float2xlns32_(arg1)+arg2;
}

xlns32_float operator+(xlns32_float arg1, float arg2) {
   return arg1+float2xlns32_(arg2);
}


xlns32_float operator-(float arg1, xlns32_float arg2) {
   return float2xlns32_(arg1)-arg2;
}

xlns32_float operator-(xlns32_float arg1, float arg2) {
   return arg1-float2xlns32_(arg2);
}

xlns32_float operator*(float arg1, xlns32_float arg2) {
   return float2xlns32_(arg1)*arg2;
}

xlns32_float operator*(xlns32_float arg1, float arg2) {
   return arg1*float2xlns32_(arg2);
}


xlns32_float operator/(float arg1, xlns32_float arg2) {
   return float2xlns32_(arg1)/arg2;
}

xlns32_float operator/(xlns32_float arg1, float arg2) {
   return arg1/float2xlns32_(arg2);
}

/*comparisons with conversion seems not to inline OK*/

int operator==(xlns32_float arg1, float arg2)
      {
       return arg1 == float2xlns32_(arg2);
      }
int operator!=(xlns32_float arg1, float arg2)
      {
       return arg1 != float2xlns32_(arg2);
      }
int operator<=(xlns32_float arg1, float arg2)
      {
       return arg1<=float2xlns32_(arg2);
      }
int operator>=(xlns32_float arg1, float arg2)
      {
       return arg1>=float2xlns32_(arg2);
      }
int operator<(xlns32_float arg1, float arg2)
      {
       return arg1<float2xlns32_(arg2);
      }
int operator>(xlns32_float arg1, float arg2)
      {
       return arg1>float2xlns32_(arg2);
      }

/*With and without convert:  +=, -=, *=, and /= */

xlns32_float operator+=(xlns32_float & arg1, xlns32_float arg2) {
   arg1 = arg1+arg2;
   return arg1;
}

xlns32_float operator+=(xlns32_float & arg1, float arg2) {
   arg1 = arg1+float2xlns32_(arg2);
   return arg1;
}



xlns32_float operator-=(xlns32_float & arg1, xlns32_float arg2) {
   arg1 = arg1-arg2;
   return arg1;
}

xlns32_float operator-=(xlns32_float & arg1, float arg2) {
   arg1 = arg1-float2xlns32_(arg2);
   return arg1;
}


xlns32_float operator*=(xlns32_float & arg1, xlns32_float arg2) {
   arg1 = arg1*arg2;
   return arg1;
}

xlns32_float operator*=(xlns32_float & arg1, float arg2) {
   arg1 = arg1*float2xlns32_(arg2);
   return arg1;
}


xlns32_float operator/=(xlns32_float & arg1, xlns32_float arg2) {
   arg1 = arg1/arg2;
   return arg1;
}

xlns32_float operator/=(xlns32_float & arg1, float arg2) {
   arg1 = arg1/float2xlns32_(arg2);
   return arg1;
}



/*assignment with type conversion*/


//maybe should use cache here
xlns32_float xlns32_float::operator=(float rvalue) {
//   x = fp2xlns32(rvalue);
     x = float2xlns32_(rvalue).x;
   return *this;
}




// functions computed ideally by convert to/from FP


inline xlns32_float sin(xlns32_float x)
{ 
	return float2xlns32_(sin(xlns32_2float(x))); 
}

inline xlns32_float cos(xlns32_float x)
{ 
	return float2xlns32_(cos(xlns32_2float(x))); 
}

// exp and log can be implemented more efficiently in LNS but 
// this is just cookie cutter ideal implementation at present

inline xlns32_float exp(xlns32_float x)
{ 
	return float2xlns32_(exp(xlns32_2float(x))); 
}

inline xlns32_float log(xlns32_float x)
{ 
	return float2xlns32_(log(xlns32_2float(x))); 
}

inline xlns32_float atan(xlns32_float x)
{ 
	return float2xlns32_(atan(xlns32_2float(x))); 
}

// the following have efficient macro implementations

inline xlns32_float sqrt(xlns32_float x)
{ 
	xlns32_float result;
	result.x = xlns32_sqrt(x.x); 
	return result; 
}

inline xlns32_float abs(xlns32_float x)
{ 
	xlns32_float result;
	result.x = xlns32_abs(x.x); 
	return result; 
}


