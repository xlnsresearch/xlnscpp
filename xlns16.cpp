// 16-bit XLNS for C++
// copyright 1999-2025 Mark G. Arnold
// these routines 
//    ran on 16-bit Turbo C/C++ (the file may have CR/Lfs from that system) 
//    were used in my PhD research and for several later papers on 32-bit LNS
// they were ported to Linux gcc and g++ around 2015 on 32-bit x86
// they were ported again for 64-bit arch in 2025, 
//    modified for 16-bit similar to bfloat (see xlns32.cpp for original float-like code)
//    with the xlns16_ideal option
//    with a Mitchell LPVIP method for the non-ideal case (Apr'25 few sml err/bugs vs python lpvip)
//    with xlns16_alt for streamlined + for modern arch w/ ovfl test
//    with xlns16_table for fast table lookup conversion (fast sb db if xlns_alt and not ideal)
//      each cvt table 256Kbyte; sb db table each 5Kbyte
// they are based on similar math foundation (Gaussian logs, sb and db) as Python xlns,
//    but use different internal storage format:
//    +------+-------------------------+
//    | sign | int(log2) . frac(log2)  |
//    +------+-------------------------+
//    the int(log2) is not twos complement; it is offset (logsignmask XORed)
//    for the 16-bit format in this file, this is roughly similar to bfloat16
//    1 sign bit, 8 int(log2) bits, 7 frac(log2) bits
//    there is an exact representation of 0.0, but no subnormals or NaNs

/* PORTABLE CODE STARTS HERE*/

#include <stdio.h>
#include <stdlib.h>
  //typedef unsigned short xlns16;
  //typedef signed short xlns16_signed;
  #ifdef _WIN32
    typedef unsigned __int16 xlns16;
    typedef __int16 xlns16_signed;
  #else
    typedef u_int16_t xlns16;
    typedef int16_t xlns16_signed;
  #endif
  #define xlns16_zero          0x0000
  #define xlns16_scale         0x0080
  #define xlns16_logmask       0x7fff
  #define xlns16_signmask      0x8000
  #define xlns16_logsignmask   0x4000
  #define xlns16_canonmask     0x8000
  #define xlns16_sqrtmask      0x2000
  #define xlns16_esszer        0x0500
  #define xlns16_canonshift    15

// Useful constant values (pre-computed LNS representations)

// xlns16_one:     log2(1.0) = 0, so internal = 0 ^ logsignmask = 0x4000
// xlns16_two:     log2(2.0) = 1, scaled = 0x0080, internal = 0x0080 ^ 0x4000 = 0x4080
// xlns16_half:    log2(0.5) = -1, scaled = -0x0080, internal = 0x3F80
// xlns16_neg_one: same as xlns16_one but with sign bit set
#define xlns16_one          0x4000
#define xlns16_neg_one      0xC000
#define xlns16_two          0x4080
#define xlns16_neg_two      0xC080
#define xlns16_half         0x3F80
#define xlns16_neg_half     0xBF80

// Basic unary operations (macros for efficiency)
#define xlns16_sign(x)  ((x) & xlns16_signmask)
#define xlns16_neg(x)   ((x) ^ xlns16_signmask)
#define xlns16_abs(x)   ((x) & xlns16_logmask)
#define xlns16_recip(x) (xlns16_sign(x)|xlns16_abs((~x)+1))
#define xlns16_sqrt(x)   (xlns16_abs(((xlns16_signed)((x)<<1))/4)^xlns16_sqrtmask)
#define xlns16_canon(x) ((x)^(-((x)>>xlns16_canonshift)|xlns16_signmask))

// Square: x^2 (efficient in LNS: double the log)
#define xlns16_square(x) xlns16_mul((x), (x))

inline xlns16 xlns16_overflow(xlns16 xlns16_x, xlns16 xlns16_y, xlns16 xlns16_temp)
{       //printf("%d %d %d\n",xlns16_temp,xlns16_x,xlns16_y);
	if (xlns16_logsignmask&xlns16_temp)
	{
		return (xlns16_signmask&(xlns16_x^xlns16_y));
	}
	else
	{
		return (xlns16_signmask&(xlns16_x^xlns16_y))| xlns16_logmask;
	}
}

inline xlns16 xlns16_mul(xlns16 x, xlns16 y)
{
   xlns16 xlns16_temp;
   //xlns16_x = x;
   //xlns16_y=y;
   xlns16_temp=(xlns16_logmask&(x))+(xlns16_logmask&(y))-xlns16_logsignmask; 
   return  (xlns16_signmask&(xlns16_temp)) ? xlns16_overflow(x,y,xlns16_temp) 
                                       :(xlns16_signmask&(x^y))|xlns16_temp;
}

inline xlns16 xlns16_div(xlns16 x, xlns16 y)
{
   xlns16 xlns16_temp;
   //xlns16_x = x;
   //xlns16_y=y;
   xlns16_temp=(xlns16_logmask&(x))-(xlns16_logmask&(y))+xlns16_logsignmask; 
   return  (xlns16_signmask&(xlns16_temp)) ? xlns16_overflow(x,y,xlns16_temp) 
                                       :(xlns16_signmask&(x^y))|xlns16_temp;
}

#ifdef xlns16_ideal
  #define xlns16_sb xlns16_sb_ideal
  #define xlns16_db xlns16_db_ideal
  #include <math.h>
  inline xlns16 xlns16_sb_ideal(xlns16_signed z)
  {
	return ((xlns16) ((log(1+ pow(2.0, ((double) z) / xlns16_scale) )/log(2.0))*xlns16_scale+.5));
  }
  inline xlns16 xlns16_db_ideal(xlns16_signed z)
  {
	return ((xlns16_signed) ((log( pow(2.0, ((double) z) / xlns16_scale) - 1 )/log(2.0))*xlns16_scale+.5));
  }
#else
  #define xlns16_sb xlns16_sb_premit
  #define xlns16_db xlns16_db_premit
  #define xlns16_F 7

  #include <math.h>
  inline xlns16 xlns16_db_ideal(xlns16_signed z)  //only for singularity
  {
	return ((xlns16_signed) ((log( pow(2.0, ((double) z) / xlns16_scale) - 1 )/log(2.0))*xlns16_scale+.5));
  }
  inline xlns16 xlns16_mitch(xlns16 z)
  {
     return (((1<<xlns16_F)+(z&((1<<xlns16_F)-1)))>>(-(z>>xlns16_F)));
  }

  inline xlns16 xlns16_sb_premit_neg(xlns16_signed zi)   //was called premitchnpi(zi): assumes zi<=0
  {
    xlns16 postcond;
    xlns16 z;
    postcond = (zi <= -(3<<xlns16_F))? 0: (zi >= -(3<<(xlns16_F-2))? -1: +1);
    z = ((zi<<3) + (zi^0xffff) + 16)>>3;
    return (zi==0)?1<<xlns16_F: xlns16_mitch(z) + postcond;
    //return ((zi==0)?1<<xlns16_F: (((1<<xlns16_F)+(z&((1<<xlns16_F)-1)))>>(-(z>>xlns16_F)))+postcond );
  }

  inline xlns16 xlns16_db_premit_neg(xlns16_signed z)   //assumes zi<0
  {
    xlns16_signed precond;
    precond = (z < -(2<<xlns16_F))?
                    5<<(xlns16_F-3):                //  0.625
                    (z >> 2) + (9 << (xlns16_F-3));//  .25*zr + 9/8
    return (-z >= 1<<xlns16_F)?-xlns16_mitch(z+precond): xlns16_db_ideal(-z)+z; // use ideal for singularity
  }
  inline xlns16 xlns16_sb_premit(xlns16_signed zi)   //assumes zi>=0
  {
    return xlns16_sb_premit_neg(-zi)+zi;
  }
  inline xlns16 xlns16_db_premit(xlns16_signed z)   //assumes zi>0
  {
    return xlns16_db_premit_neg(-z)+z;
  } 
#endif


#ifdef xlns16_alt
 #ifdef xlns16_table
 #include "xlns16sbdbtbl.h"
 #endif

inline xlns16 xlns16_add(xlns16 x, xlns16 y)
{
    xlns16 minxyl, maxxy, xl, yl, usedb, adjust, adjustez;
    xlns16_signed z;
    xl = x & xlns16_logmask;
    yl = y & xlns16_logmask;
    minxyl = (yl>xl) ? xl : yl;
    maxxy  = (xl>yl) ? x  : y;
    z = minxyl - (maxxy&xlns16_logmask);
    usedb = xlns16_signmask&(x^y); 
    #ifdef xlns16_ideal
     float pm1 = usedb ? -1.0 : 1.0;
     adjust = z+((xlns16_signed)(log(pm1+pow(2.0,-((double)z)/xlns16_scale))/log(2.0)*xlns16_scale+.5));
     adjustez = (z < -xlns16_esszer) ? 0 : adjust; 
    #else
     #ifdef xlns16_table
       xlns16_signed non_ez_z = (z <= -xlns16_esszer) ? xlns16_esszer-1 : -z; 
       adjustez = usedb ? xlns16dbtbl[non_ez_z] : 
                          xlns16sbtbl[non_ez_z]; 
     #else
      #ifdef xlns16_altopt
       xlns16_signed precond = (usedb==0) ? ((-z)>>3) :          // -.125*z 
                (z < -(2<<xlns16_F)) ? 5<<(xlns16_F-3):        //  0.625
                                (z >> 2) + (9 << (xlns16_F-3));//  .25*z + 9/8
       xlns16_signed postcond = (z <= -(3<<xlns16_F)) ? 0: 
                            z >= -(3<<(xlns16_F-2)) ? -(1<<(xlns16_F-6)) : 
                                                      +(1<<(xlns16_F-6));
       xlns16_signed mitch = (-z >= 1<<xlns16_F)||(usedb==0) ? xlns16_mitch(z+precond) : 
                                          -xlns16_db_ideal(-z)-z; // use ideal for singularity
       adjust = usedb ? -mitch : (z==0) ? 1<<xlns16_F : mitch + postcond;
      #else
       adjust = usedb ? xlns16_db_premit_neg(z) : 
                        xlns16_sb_premit_neg(z); 
      #endif
      adjustez = (z < -xlns16_esszer) ? 0 : adjust; 
     #endif
    #endif
    return ((z==0) && usedb) ? 
                     xlns16_zero :
                     xlns16_mul(maxxy, xlns16_logsignmask + adjustez);
}

#else

//++++ X-X ERROR fixed

xlns16 xlns16_add(xlns16 x, xlns16 y)
{
	xlns16 t;
	xlns16_signed z;

	z = (x&xlns16_logmask) - (y&xlns16_logmask);
	if (z<0)
	{
		z = -z;
		t = x;
		x = y;
		y = t;
	}
	if (xlns16_signmask&(x^y))
	{
		if (z == 0)
			return xlns16_zero;
		if (z < xlns16_esszer)
			return xlns16_neg(y + xlns16_db(z));
		else
			return xlns16_neg(y+z);
	}
	else
	{
		return y + xlns16_sb(z);
	}
}
#endif

#define xlns16_sub(x,y) xlns16_add(x,xlns16_neg(y))

/*END OF PORTABLE CODE*/

/*START OF PORTABLE CODE THAT DEPENDS ON <math.h>*/

#include <math.h>

#ifdef xlns16_table

#include "xlns16revcvtbl.h"

inline xlns16 fp2xlns16(float x)
{
	return xlns16revcvtbl[(*(unsigned *)&x)>>15];
}

#include "xlns16cvtbl.h"

inline float xlns162fp(xlns16 x)
{
	return xlns16cvtbl[x];
}


#else

xlns16 fp2xlns16(float x)
{
	if (x==0.0)
		return(xlns16_zero);
	else if (x > 0.0)
		return xlns16_abs((xlns16_signed) ((log(x)/log(2.0))*xlns16_scale))
		       ^xlns16_logsignmask;
	else
		return (((xlns16_signed) ((log(fabs(x))/log(2.0))*xlns16_scale))
			  |xlns16_signmask)^xlns16_logsignmask;
}


float xlns162fp(xlns16 x)
{
	if (xlns16_abs(x) == xlns16_zero)
		return (0.0);
	else if (xlns16_sign(x))
		return (float) (-pow(2.0,((double) (((xlns16_signed) (xlns16_abs(x)-xlns16_logsignmask))))
					/((float) xlns16_scale)));
	else {
		return (float) (+pow(2.0,((double) (((xlns16_signed) (xlns16_abs(x)-xlns16_logsignmask))))
					/((float) xlns16_scale)));
	}
}

#endif

// Comparison and utility functions

// Check if value is zero
inline int xlns16_is_zero(xlns16 x) {
    return (xlns16_abs(x) == xlns16_zero);
}

// Check if value is negative
inline int xlns16_is_negative(xlns16 x) {
    return (xlns16_sign(x) != 0) && !xlns16_is_zero(x);
}

// Check if value is positive
inline int xlns16_is_positive(xlns16 x) {
    return (xlns16_sign(x) == 0) && !xlns16_is_zero(x);
}

// Greater than comparison (returns 1 if a > b)
inline int xlns16_gt(xlns16 a, xlns16 b) {
    return xlns16_canon(a) > xlns16_canon(b);
}

// Less than comparison (returns 1 if a < b)
inline int xlns16_lt(xlns16 a, xlns16 b) {
    return xlns16_canon(a) < xlns16_canon(b);
}

// Equal comparison
inline int xlns16_eq(xlns16 a, xlns16 b) {
    return a == b;
}

// Greater than or equal
inline int xlns16_ge(xlns16 a, xlns16 b) {
    return xlns16_canon(a) >= xlns16_canon(b);
}

// Less than or equal
inline int xlns16_le(xlns16 a, xlns16 b) {
    return xlns16_canon(a) <= xlns16_canon(b);
}

// Maximum of two values
inline xlns16 xlns16_max(xlns16 a, xlns16 b) {
    return xlns16_gt(a, b) ? a : b;
}

// Minimum of two values
inline xlns16 xlns16_min(xlns16 a, xlns16 b) {
    return xlns16_lt(a, b) ? a : b;
}

// Copy sign from y to x (magnitude of x, sign of y)
inline xlns16 xlns16_copysign(xlns16 x, xlns16 y) {
    return xlns16_abs(x) | xlns16_sign(y);
}

// Fused multiply-add: a * b + c
inline xlns16 xlns16_fma(xlns16 a, xlns16 b, xlns16 c) {
    return xlns16_add(xlns16_mul(a, b), c);
}

// Batch conversion functions (for ggml tensor operations)

// Batch convert float array to xlns16 array
inline void xlns16_batch_from_float(const float *src, xlns16 *dst, size_t n) {
    for (size_t i = 0; i < n; i++) {
        dst[i] = fp2xlns16(src[i]);
    }
}

// Batch convert xlns16 array to float array
inline void xlns16_batch_to_float(const xlns16 *src, float *dst, size_t n) {
    for (size_t i = 0; i < n; i++) {
        dst[i] = xlns162fp(src[i]);
    }
}


// Batch element-wise operations

// Batch multiplication: c[i] = a[i] * b[i]
inline void xlns16_batch_mul(const xlns16 *a, const xlns16 *b, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_mul(a[i], b[i]);
    }
}

// Batch addition: c[i] = a[i] + b[i]
inline void xlns16_batch_add(const xlns16 *a, const xlns16 *b, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_add(a[i], b[i]);
    }
}

// Batch subtraction: c[i] = a[i] - b[i]
inline void xlns16_batch_sub(const xlns16 *a, const xlns16 *b, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_sub(a[i], b[i]);
    }
}

// Batch division: c[i] = a[i] / b[i]
inline void xlns16_batch_div(const xlns16 *a, const xlns16 *b, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_div(a[i], b[i]);
    }
}

// Batch scale: c[i] = a[i] * scalar
inline void xlns16_batch_scale(const xlns16 *a, xlns16 scalar, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_mul(a[i], scalar);
    }
}

// Batch negation: c[i] = -a[i]
inline void xlns16_batch_neg(const xlns16 *a, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_neg(a[i]);
    }
}

// Batch absolute value: c[i] = |a[i]|
inline void xlns16_batch_abs(const xlns16 *a, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_abs(a[i]);
    }
}

// Vector operations (critical for ggml MUL_MAT)

// Sum of array elements: result = Σ a[i]
inline xlns16 xlns16_sum(const xlns16 *a, size_t n) {
    if (n == 0) return xlns16_zero;
    xlns16 sum = a[0];
    for (size_t i = 1; i < n; i++) {
        sum = xlns16_add(sum, a[i]);
    }
    return sum;
}

// Vector dot product: result = Σ(a[i] * b[i])
inline xlns16 xlns16_vec_dot(const xlns16 *a, const xlns16 *b, size_t n) {
    if (n == 0) return xlns16_zero;
    xlns16 sum = xlns16_mul(a[0], b[0]);
    for (size_t i = 1; i < n; i++) {
        sum = xlns16_add(sum, xlns16_mul(a[i], b[i]));
    }
    return sum;
}

// Vector dot product with float inputs (converts to LNS internally)
inline float xlns16_vec_dot_f32(const float *a, const float *b, size_t n) {
    if (n == 0) return 0.0f;
    xlns16 sum = xlns16_mul(fp2xlns16(a[0]), fp2xlns16(b[0]));
    for (size_t i = 1; i < n; i++) {
        xlns16 prod = xlns16_mul(fp2xlns16(a[i]), fp2xlns16(b[i]));
        sum = xlns16_add(sum, prod);
    }
    return xlns162fp(sum);
}

// Maximum element in array
inline xlns16 xlns16_max_array(const xlns16 *a, size_t n) {
    if (n == 0) return xlns16_zero;
    xlns16 maxval = a[0];
    for (size_t i = 1; i < n; i++) {
        if (xlns16_gt(a[i], maxval)) {
            maxval = a[i];
        }
    }
    return maxval;
}

// Minimum element in array
inline xlns16 xlns16_min_array(const xlns16 *a, size_t n) {
    if (n == 0) return xlns16_zero;
    xlns16 minval = a[0];
    for (size_t i = 1; i < n; i++) {
        if (xlns16_lt(a[i], minval)) {
            minval = a[i];
        }
    }
    return minval;
}

// Activation functions (for neural network operations)

// ReLU: max(0, x)
inline xlns16 xlns16_relu(xlns16 x) {
    return xlns16_is_negative(x) ? xlns16_zero : x;
}

// Batch ReLU
inline void xlns16_batch_relu(const xlns16 *a, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_relu(a[i]);
    }
}

// Sigmoid: 1 / (1 + exp(-x))
inline xlns16 xlns16_sigmoid(xlns16 x) {
    float fx = xlns162fp(x);
    float result = 1.0f / (1.0f + expf(-fx));
    return fp2xlns16(result);
}

// Batch sigmoid
inline void xlns16_batch_sigmoid(const xlns16 *a, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_sigmoid(a[i]);
    }
}

// Tanh
inline xlns16 xlns16_tanh(xlns16 x) {
    float fx = xlns162fp(x);
    float result = tanhf(fx);
    return fp2xlns16(result);
}

// Batch tanh
inline void xlns16_batch_tanh(const xlns16 *a, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_tanh(a[i]);
    }
}

// SiLU (Swish): x * sigmoid(x) = x / (1 + exp(-x))
inline xlns16 xlns16_silu(xlns16 x) {
    float fx = xlns162fp(x);
    float result = fx / (1.0f + expf(-fx));
    return fp2xlns16(result);
}

// Batch SiLU
inline void xlns16_batch_silu(const xlns16 *a, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_silu(a[i]);
    }
}

// GELU approximation
inline xlns16 xlns16_gelu(xlns16 x) {
    float fx = xlns162fp(x);
    const float sqrt_2_over_pi = 0.7978845608f;
    float inner = sqrt_2_over_pi * (fx + 0.044715f * fx * fx * fx);
    float result = 0.5f * fx * (1.0f + tanhf(inner));
    return fp2xlns16(result);
}

// Batch GELU
inline void xlns16_batch_gelu(const xlns16 *a, xlns16 *c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        c[i] = xlns16_gelu(a[i]);
    }
}

// Softmax helper: subtract max for numerical stability, then exp
inline void xlns16_softmax_exp(const xlns16 *a, xlns16 *c, size_t n) {
    xlns16 maxval = xlns16_max_array(a, n);
    for (size_t i = 0; i < n; i++) {
        float fx = xlns162fp(a[i]) - xlns162fp(maxval);
        c[i] = fp2xlns16(expf(fx));
    }
}

// exp and log in LNS

#ifdef xlns16_table
#include "xlns16exptbl.h"
#include "xlns16logtbl.h"
// exp(x) - computes e^x
inline xlns16 xlns16_exp(xlns16 x) { return xlns16exptbl[x]; }
// log(x) - computes natural log
inline xlns16 xlns16_log(xlns16 x) { return xlns16logtbl[x]; }
#else
// exp(x) - computes e^x
inline xlns16 xlns16_exp(xlns16 x) {
    float fx = xlns162fp(x);
    return fp2xlns16(expf(fx));
}

// log(x) - computes natural log
inline xlns16 xlns16_log(xlns16 x) {
    float fx = xlns162fp(x);
    if (fx <= 0.0f) return xlns16_zero;
    return fp2xlns16(logf(fx));
}
#endif

// exp2(x) - computes 2^x
inline xlns16 xlns16_exp2(xlns16 x) {
    float fx = xlns162fp(x);
    return fp2xlns16(exp2f(fx));
}

// log2(x) - computes log base 2
inline xlns16 xlns16_log2(xlns16 x) {
    float fx = xlns162fp(x);
    if (fx <= 0.0f) return xlns16_zero;
    return fp2xlns16(log2f(fx));
}

// pow(base, exp) - computes base^exp
inline xlns16 xlns16_pow(xlns16 base, xlns16 exponent) {
    float fbase = xlns162fp(base);
    float fexp = xlns162fp(exponent);
    if (fbase <= 0.0f) return xlns16_zero;
    return fp2xlns16(powf(fbase, fexp));
}


// Full softmax: exp(a[i]-max) / sum(exp(a[j]-max)), using LNS primitives
inline void xlns16_softmax(const xlns16 *a, xlns16 *c, size_t n) {
    if (n == 0) return;
    xlns16 maxval = xlns16_max_array(a, n);
    for (size_t i = 0; i < n; i++)
        c[i] = xlns16_exp(xlns16_sub(a[i], maxval));
    xlns16 total = xlns16_sum(c, n);
    for (size_t i = 0; i < n; i++)
        c[i] = xlns16_div(c[i], total);
}


// Layer normalization: (x - mean) / sqrt(var + eps) * gamma + beta
inline void xlns16_layernorm(const xlns16 *x, xlns16 *out,
                       const xlns16 *gamma, const xlns16 *beta,
                       size_t n, float eps) {
    // compute mean
    xlns16 mean = xlns16_sum(x, n);
    mean = xlns16_div(mean, fp2xlns16((float)n));
    // compute variance
    xlns16 var = xlns16_zero;
    for (size_t i = 0; i < n; i++) {
        xlns16 diff = xlns16_sub(x[i], mean);
        var = xlns16_add(var, xlns16_mul(diff, diff));
    }
    var = xlns16_div(var, fp2xlns16((float)n));
    // normalize
    xlns16 inv_std = fp2xlns16(1.0f / sqrtf(xlns162fp(var) + eps));
    for (size_t i = 0; i < n; i++) {
        out[i] = xlns16_mul(xlns16_sub(x[i], mean), inv_std);
        if (gamma) out[i] = xlns16_mul(out[i], gamma[i]);
        if (beta)  out[i] = xlns16_add(out[i], beta[i]);
    }
}

/*END OF PORTABLE CODE THAT DEPENDS ON <math.h>*/




#include <iostream>

class xlns16_float {
    xlns16 x;
 public:
    friend xlns16_float operator+(xlns16_float , xlns16_float );
    friend xlns16_float operator+(float, xlns16_float );
    friend xlns16_float operator+(xlns16_float , float);
    friend xlns16_float operator-(xlns16_float , xlns16_float );
    friend xlns16_float operator-(float, xlns16_float );
    friend xlns16_float operator-(xlns16_float , float);
    friend xlns16_float operator*(xlns16_float , xlns16_float );
    friend xlns16_float operator*(float, xlns16_float );
    friend xlns16_float operator*(xlns16_float , float);
    friend xlns16_float operator/(xlns16_float , xlns16_float );
    friend xlns16_float operator/(float, xlns16_float );
    friend xlns16_float operator/(xlns16_float , float);
    xlns16_float operator=(float);
    friend xlns16 xlns16_internal(xlns16_float );
    friend float xlns16_2float(xlns16_float );
    friend xlns16_float float2xlns16_(float);
    friend std::ostream& operator<<(std::ostream&, xlns16_float );
    friend xlns16_float operator-(xlns16_float);
    friend xlns16_float operator+=(xlns16_float &, xlns16_float);
    friend xlns16_float operator+=(xlns16_float &, float);
    friend xlns16_float operator-=(xlns16_float &, xlns16_float);
    friend xlns16_float operator-=(xlns16_float &, float);
    friend xlns16_float operator*=(xlns16_float &, xlns16_float);
    friend xlns16_float operator*=(xlns16_float &, float);
    friend xlns16_float operator/=(xlns16_float &, xlns16_float);
    friend xlns16_float operator/=(xlns16_float &, float);
    friend xlns16_float sin(xlns16_float);
    friend xlns16_float cos(xlns16_float);
    friend xlns16_float exp(xlns16_float);
    friend xlns16_float log(xlns16_float);
    friend xlns16_float atan(xlns16_float);
    friend xlns16_float abs(xlns16_float);
    friend xlns16_float sqrt(xlns16_float);
//    friend xlns16_float operator-(xlns16_float);
    friend int operator==(xlns16_float arg1, xlns16_float arg2)
      {
       return (arg1.x == arg2.x);
      }
    friend int operator!=(xlns16_float arg1, xlns16_float arg2)
      {
       return (arg1.x != arg2.x);
      }
    friend int operator<=(xlns16_float arg1, xlns16_float arg2)
      {
       return (xlns16_canon(arg1.x)<=xlns16_canon(arg2.x));
      }
    friend int operator>=(xlns16_float arg1, xlns16_float arg2)
      {
       return (xlns16_canon(arg1.x)>=xlns16_canon(arg2.x));
      }
    friend int operator<(xlns16_float arg1, xlns16_float arg2)
      {
       return (xlns16_canon(arg1.x)<xlns16_canon(arg2.x));
      }
    friend int operator>(xlns16_float arg1, xlns16_float arg2)
      {
       return (xlns16_canon(arg1.x)>xlns16_canon(arg2.x));
      }
    friend int operator==(xlns16_float arg1, float arg2);
    friend int operator!=(xlns16_float arg1, float arg2);
    friend int operator<=(xlns16_float arg1, float arg2);
    friend int operator>=(xlns16_float arg1, float arg2);
    friend int operator<(xlns16_float arg1, float arg2);
    friend int operator>(xlns16_float arg1, float arg2);
  };




/*access function for internal representation*/

xlns16 xlns16_internal(xlns16_float y) {
    return y.x;
}


float xlns16_2float(xlns16_float y) {
	return xlns162fp(y.x);
}

#define xlns16_cachesize 1024
xlns16 xlns16_cachecontent[xlns16_cachesize];
float xlns16_cachetag[xlns16_cachesize];
long xlns16_misses=0;
long xlns16_hits=0;
#define xlns16_cacheon 0// off for table

xlns16_float float2xlns16_(float y) {
	xlns16_float z;
	unsigned char * fpbyte;
	int addr;
	fpbyte=(unsigned char *)(&y);
	addr = (fpbyte[2])^(fpbyte[3]<<2);
	if ((xlns16_cachetag[addr] ==  y)&&xlns16_cacheon)
	{
//	  printf("hit  %f  %02x %02x %02x %02x addr=%d\n",y, fpbyte[0],fpbyte[1],fpbyte[2],fpbyte[3],addr);
	  z.x = xlns16_cachecontent[addr];
	  xlns16_hits++;
	}
	else
	{
//	  printf("miss %f  %02x %02x %02x %02x addr=%d\n",y, fpbyte[0],fpbyte[1],fpbyte[2],fpbyte[3],addr);
	  z.x = fp2xlns16(y);
	  xlns16_cachecontent[addr] = z.x;
	  xlns16_cachetag[addr] = y;
	  xlns16_misses++;
	}
//	getchar();
	return z;
}


/*overload stream output << operator*/

//#include <ostream>
std::ostream& operator<< (std::ostream& s, xlns16_float  y) {
    return s << xlns16_2float(y);
}

xlns16_float operator-(xlns16_float arg1) {
   xlns16_float z;
   z.x=xlns16_neg(arg1.x);
   return z;
}



xlns16_float operator+(xlns16_float arg1, xlns16_float arg2) {
   xlns16_float z;
   z.x=xlns16_add(arg1.x,arg2.x);
   return z;
}

xlns16_float operator-(xlns16_float arg1, xlns16_float arg2) {
   xlns16_float z;
   z.x=xlns16_sub(arg1.x,arg2.x);
   return z;
}

xlns16_float operator*(xlns16_float arg1, xlns16_float arg2) {
   xlns16_float z;
   z.x=xlns16_mul(arg1.x,arg2.x);
   return z;
}

xlns16_float operator/(xlns16_float arg1, xlns16_float arg2) {
   xlns16_float z;
   z.x=xlns16_div(arg1.x,arg2.x);
   return z;
}


/*operators with auto type conversion*/

xlns16_float operator+(float arg1, xlns16_float arg2) {
   return float2xlns16_(arg1)+arg2;
}

xlns16_float operator+(xlns16_float arg1, float arg2) {
   return arg1+float2xlns16_(arg2);
}


xlns16_float operator-(float arg1, xlns16_float arg2) {
   return float2xlns16_(arg1)-arg2;
}

xlns16_float operator-(xlns16_float arg1, float arg2) {
   return arg1-float2xlns16_(arg2);
}

xlns16_float operator*(float arg1, xlns16_float arg2) {
   return float2xlns16_(arg1)*arg2;
}

xlns16_float operator*(xlns16_float arg1, float arg2) {
   return arg1*float2xlns16_(arg2);
}


xlns16_float operator/(float arg1, xlns16_float arg2) {
   return float2xlns16_(arg1)/arg2;
}

xlns16_float operator/(xlns16_float arg1, float arg2) {
   return arg1/float2xlns16_(arg2);
}

/*comparisons with conversion seems not to inline OK*/

int operator==(xlns16_float arg1, float arg2)
      {
       return arg1 == float2xlns16_(arg2);
      }
int operator!=(xlns16_float arg1, float arg2)
      {
       return arg1 != float2xlns16_(arg2);
      }
int operator<=(xlns16_float arg1, float arg2)
      {
       return arg1<=float2xlns16_(arg2);
      }
int operator>=(xlns16_float arg1, float arg2)
      {
       return arg1>=float2xlns16_(arg2);
      }
int operator<(xlns16_float arg1, float arg2)
      {
       return arg1<float2xlns16_(arg2);
      }
int operator>(xlns16_float arg1, float arg2)
      {
       return arg1>float2xlns16_(arg2);
      }

/*With and without convert:  +=, -=, *=, and /= */

xlns16_float operator+=(xlns16_float & arg1, xlns16_float arg2) {
   arg1 = arg1+arg2;
   return arg1;
}

xlns16_float operator+=(xlns16_float & arg1, float arg2) {
   arg1 = arg1+float2xlns16_(arg2);
   return arg1;
}



xlns16_float operator-=(xlns16_float & arg1, xlns16_float arg2) {
   arg1 = arg1-arg2;
   return arg1;
}

xlns16_float operator-=(xlns16_float & arg1, float arg2) {
   arg1 = arg1-float2xlns16_(arg2);
   return arg1;
}


xlns16_float operator*=(xlns16_float & arg1, xlns16_float arg2) {
   arg1 = arg1*arg2;
   return arg1;
}

xlns16_float operator*=(xlns16_float & arg1, float arg2) {
   arg1 = arg1*float2xlns16_(arg2);
   return arg1;
}


xlns16_float operator/=(xlns16_float & arg1, xlns16_float arg2) {
   arg1 = arg1/arg2;
   return arg1;
}

xlns16_float operator/=(xlns16_float & arg1, float arg2) {
   arg1 = arg1/float2xlns16_(arg2);
   return arg1;
}



/*assignment with type conversion*/


//maybe should use cache here
xlns16_float xlns16_float::operator=(float rvalue) {
//   x = fp2xlns16(rvalue);
     x = float2xlns16_(rvalue).x;
   return *this;
}



// functions computed ideally by convert to/from FP


inline xlns16_float sin(xlns16_float x)
{ 
	return float2xlns16_(sin(xlns16_2float(x))); 
}

inline xlns16_float cos(xlns16_float x)
{ 
	return float2xlns16_(cos(xlns16_2float(x))); 
}

// exp and log can be implemented more efficiently in LNS but 
// this is just cookie cutter ideal implementation at present

inline xlns16_float exp(xlns16_float x)
{ 
	return float2xlns16_(exp(xlns16_2float(x))); 
}

inline xlns16_float log(xlns16_float x)
{ 
	return float2xlns16_(log(xlns16_2float(x))); 
}

inline xlns16_float atan(xlns16_float x)
{ 
	return float2xlns16_(atan(xlns16_2float(x))); 
}

// the following have efficient macro implementations

inline xlns16_float sqrt(xlns16_float x)
{ 
	xlns16_float result;
	result.x = xlns16_sqrt(x.x); 
	return result; 
}

inline xlns16_float abs(xlns16_float x)
{ 
	xlns16_float result;
	result.x = xlns16_abs(x.x); 
	return result; 
}

