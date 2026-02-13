// XLNS — 32-bit Logarithmic Number System for C++
// Copyright (c) 1999–2025 Mark G. Arnold
// SPDX-License-Identifier: MIT
//
// Public API header for the 32-bit LNS implementation.
//
// Bit layout:
//    +------+-------------------------+
//    | sign | int(log2) . frac(log2)  |
//    +------+-------------------------+
//      1 bit     8 bits     23 bits
//
// Roughly analogous to IEEE 754 float32 in precision.
// The int(log2) is not two's complement; it is offset (logsignmask XOR'd).
// There is an exact representation of 0.0, but no subnormals or NaNs.
//
// Compile-time options (define BEFORE including this header):
//   xlns32_ideal   — Use math.h log/pow for sb/db (most accurate, slowest)
//   xlns32_alt     — Alternative addition algorithm with less branching
//   xlns32_arch16  — Use unsigned long/signed long for 16-bit architectures

#ifndef XLNS32_H
#define XLNS32_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

// ============================================================================
// Type definitions
// ============================================================================

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

// ============================================================================
// Utility macros
// ============================================================================

#define xlns32_sign(x)   ((x) & xlns32_signmask)
#define xlns32_neg(x)    ((x) ^ xlns32_signmask)
#define xlns32_abs(x)    ((x) & xlns32_logmask)
#define xlns32_recip(x)  (xlns32_sign(x)|xlns32_abs((~x)+1))
#define xlns32_sqrt(x)   (xlns32_abs(((xlns32_signed)((x)<<1))/4)^xlns32_sqrtmask)
#define xlns32_canon(x)  ((x)^(-((x)>>xlns32_canonshift)|xlns32_signmask))
#define xlns32_sub(x,y)  xlns32_add(x,xlns32_neg(y))

// ============================================================================
// Core arithmetic — declarations
// ============================================================================

xlns32 xlns32_overflow(xlns32 xlns32_x, xlns32 xlns32_y, xlns32 xlns32_temp);
xlns32 xlns32_mul(xlns32 x, xlns32 y);
xlns32 xlns32_div(xlns32 x, xlns32 y);
xlns32 xlns32_add(xlns32 x, xlns32 y);

// ============================================================================
// Gaussian logarithm declarations
// ============================================================================

#ifdef xlns32_ideal
  #define xlns32_sb xlns32_sb_ideal
  #define xlns32_db xlns32_db_ideal
  xlns32 xlns32_sb_ideal(xlns32 z);
  xlns32 xlns32_db_ideal(xlns32 z);
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

  xlns32 xlns32_dbtrans3(xlns32 z);
#endif

// ============================================================================
// Conversion functions
// ============================================================================

xlns32 fp2xlns32(float x);
float xlns322fp(xlns32 x);

// ============================================================================
// Operator-overloaded wrapper class
// ============================================================================

class xlns32_float {
    xlns32 x;
 public:
    friend xlns32_float operator+(xlns32_float, xlns32_float);
    friend xlns32_float operator+(float, xlns32_float);
    friend xlns32_float operator+(xlns32_float, float);
    friend xlns32_float operator-(xlns32_float, xlns32_float);
    friend xlns32_float operator-(float, xlns32_float);
    friend xlns32_float operator-(xlns32_float, float);
    friend xlns32_float operator*(xlns32_float, xlns32_float);
    friend xlns32_float operator*(float, xlns32_float);
    friend xlns32_float operator*(xlns32_float, float);
    friend xlns32_float operator/(xlns32_float, xlns32_float);
    friend xlns32_float operator/(float, xlns32_float);
    friend xlns32_float operator/(xlns32_float, float);
    xlns32_float operator=(float);
    friend xlns32 xlns32_internal(xlns32_float);
    friend float xlns32_2float(xlns32_float);
    friend xlns32_float float2xlns32_(float);
    friend std::ostream& operator<<(std::ostream&, xlns32_float);
    friend xlns32_float operator-(xlns32_float);
    friend xlns32_float operator+=(xlns32_float&, xlns32_float);
    friend xlns32_float operator+=(xlns32_float&, float);
    friend xlns32_float operator-=(xlns32_float&, xlns32_float);
    friend xlns32_float operator-=(xlns32_float&, float);
    friend xlns32_float operator*=(xlns32_float&, xlns32_float);
    friend xlns32_float operator*=(xlns32_float&, float);
    friend xlns32_float operator/=(xlns32_float&, xlns32_float);
    friend xlns32_float operator/=(xlns32_float&, float);
    friend xlns32_float sin(xlns32_float);
    friend xlns32_float cos(xlns32_float);
    friend xlns32_float exp(xlns32_float);
    friend xlns32_float log(xlns32_float);
    friend xlns32_float atan(xlns32_float);
    friend xlns32_float abs(xlns32_float);
    friend xlns32_float sqrt(xlns32_float);
    friend int operator==(xlns32_float arg1, xlns32_float arg2)
      { return (arg1.x == arg2.x); }
    friend int operator!=(xlns32_float arg1, xlns32_float arg2)
      { return (arg1.x != arg2.x); }
    friend int operator<=(xlns32_float arg1, xlns32_float arg2)
      { return (xlns32_canon(arg1.x)<=xlns32_canon(arg2.x)); }
    friend int operator>=(xlns32_float arg1, xlns32_float arg2)
      { return (xlns32_canon(arg1.x)>=xlns32_canon(arg2.x)); }
    friend int operator<(xlns32_float arg1, xlns32_float arg2)
      { return (xlns32_canon(arg1.x)<xlns32_canon(arg2.x)); }
    friend int operator>(xlns32_float arg1, xlns32_float arg2)
      { return (xlns32_canon(arg1.x)>xlns32_canon(arg2.x)); }
    friend int operator==(xlns32_float arg1, float arg2);
    friend int operator!=(xlns32_float arg1, float arg2);
    friend int operator<=(xlns32_float arg1, float arg2);
    friend int operator>=(xlns32_float arg1, float arg2);
    friend int operator<(xlns32_float arg1, float arg2);
    friend int operator>(xlns32_float arg1, float arg2);
};

// ============================================================================
// Cache and access functions — declarations
// ============================================================================

// Cache-related variables (defined in xlns32.cpp)
extern long xlns32_misses;
extern long xlns32_hits;

xlns32 xlns32_internal(xlns32_float y);
float xlns32_2float(xlns32_float y);
xlns32_float float2xlns32_(float y);

// ============================================================================
// Math function overloads — declarations
// ============================================================================

xlns32_float sin(xlns32_float x);
xlns32_float cos(xlns32_float x);
xlns32_float exp(xlns32_float x);
xlns32_float log(xlns32_float x);
xlns32_float atan(xlns32_float x);
xlns32_float sqrt(xlns32_float x);
xlns32_float abs(xlns32_float x);

#endif // XLNS32_H