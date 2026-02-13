// XLNS — 16-bit Logarithmic Number System for C++
// Copyright (c) 1999–2025 Mark G. Arnold
// SPDX-License-Identifier: MIT
//
// Public API header for the 16-bit LNS implementation.
//
// Bit layout:
//    +------+-------------------------+
//    | sign | int(log2) . frac(log2)  |
//    +------+-------------------------+
//      1 bit     8 bits      7 bits
//
// Roughly analogous to bfloat16 in precision.
// The int(log2) is not two's complement; it is offset (logsignmask XOR'd).
// There is an exact representation of 0.0, but no subnormals or NaNs.
//
// Compile-time options (define BEFORE including this header):
//   xlns16_ideal   — Use math.h log/pow for sb/db (most accurate, slowest)
//   xlns16_alt     — Alternative addition algorithm with less branching
//   xlns16_altopt  — Further simplified LPVIP (within alt, slightly less accurate)
//   xlns16_table   — Use precomputed lookup tables for conversion and sb/db

#ifndef XLNS16_H
#define XLNS16_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

// ============================================================================
// Type definitions
// ============================================================================

#ifdef _WIN32
  typedef unsigned __int16 xlns16;
  typedef __int16 xlns16_signed;
#else
  #include <cstdint>
  typedef uint16_t xlns16;
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

// ============================================================================
// Utility macros
// ============================================================================

#define xlns16_sign(x)   ((x) & xlns16_signmask)
#define xlns16_neg(x)    ((x) ^ xlns16_signmask)
#define xlns16_abs(x)    ((x) & xlns16_logmask)
#define xlns16_recip(x)  (xlns16_sign(x)|xlns16_abs((~x)+1))
#define xlns16_sqrt(x)   (xlns16_abs(((xlns16_signed)((x)<<1))/4)^xlns16_sqrtmask)
#define xlns16_canon(x)  ((x)^(-((x)>>xlns16_canonshift)|xlns16_signmask))
#define xlns16_sub(x,y)  xlns16_add(x,xlns16_neg(y))

// ============================================================================
// Core arithmetic — declarations
// ============================================================================

xlns16 xlns16_overflow(xlns16 xlns16_x, xlns16 xlns16_y, xlns16 xlns16_temp);
xlns16 xlns16_mul(xlns16 x, xlns16 y);
xlns16 xlns16_div(xlns16 x, xlns16 y);
xlns16 xlns16_add(xlns16 x, xlns16 y);

// ============================================================================
// Gaussian logarithm declarations
// ============================================================================

#ifdef xlns16_ideal
  #define xlns16_sb xlns16_sb_ideal
  #define xlns16_db xlns16_db_ideal
  xlns16 xlns16_sb_ideal(xlns16_signed z);
  xlns16 xlns16_db_ideal(xlns16_signed z);
#else
  #define xlns16_sb xlns16_sb_premit
  #define xlns16_db xlns16_db_premit
  #define xlns16_F 7

  xlns16 xlns16_db_ideal(xlns16_signed z);  // needed for singularity
  xlns16 xlns16_mitch(xlns16 z);
  xlns16 xlns16_sb_premit_neg(xlns16_signed zi);
  xlns16 xlns16_db_premit_neg(xlns16_signed z);
  xlns16 xlns16_sb_premit(xlns16_signed zi);
  xlns16 xlns16_db_premit(xlns16_signed z);
#endif

// ============================================================================
// Conversion functions
// ============================================================================

xlns16 fp2xlns16(float x);
float xlns162fp(xlns16 x);

// ============================================================================
// Operator-overloaded wrapper class
// ============================================================================

class xlns16_float {
    xlns16 x;
 public:
    friend xlns16_float operator+(xlns16_float, xlns16_float);
    friend xlns16_float operator+(float, xlns16_float);
    friend xlns16_float operator+(xlns16_float, float);
    friend xlns16_float operator-(xlns16_float, xlns16_float);
    friend xlns16_float operator-(float, xlns16_float);
    friend xlns16_float operator-(xlns16_float, float);
    friend xlns16_float operator*(xlns16_float, xlns16_float);
    friend xlns16_float operator*(float, xlns16_float);
    friend xlns16_float operator*(xlns16_float, float);
    friend xlns16_float operator/(xlns16_float, xlns16_float);
    friend xlns16_float operator/(float, xlns16_float);
    friend xlns16_float operator/(xlns16_float, float);
    xlns16_float operator=(float);
    friend xlns16 xlns16_internal(xlns16_float);
    friend float xlns16_2float(xlns16_float);
    friend xlns16_float float2xlns16_(float);
    friend std::ostream& operator<<(std::ostream&, xlns16_float);
    friend xlns16_float operator-(xlns16_float);
    friend xlns16_float operator+=(xlns16_float&, xlns16_float);
    friend xlns16_float operator+=(xlns16_float&, float);
    friend xlns16_float operator-=(xlns16_float&, xlns16_float);
    friend xlns16_float operator-=(xlns16_float&, float);
    friend xlns16_float operator*=(xlns16_float&, xlns16_float);
    friend xlns16_float operator*=(xlns16_float&, float);
    friend xlns16_float operator/=(xlns16_float&, xlns16_float);
    friend xlns16_float operator/=(xlns16_float&, float);
    friend xlns16_float sin(xlns16_float);
    friend xlns16_float cos(xlns16_float);
    friend xlns16_float exp(xlns16_float);
    friend xlns16_float log(xlns16_float);
    friend xlns16_float atan(xlns16_float);
    friend xlns16_float abs(xlns16_float);
    friend xlns16_float sqrt(xlns16_float);
    friend int operator==(xlns16_float arg1, xlns16_float arg2)
      { return (arg1.x == arg2.x); }
    friend int operator!=(xlns16_float arg1, xlns16_float arg2)
      { return (arg1.x != arg2.x); }
    friend int operator<=(xlns16_float arg1, xlns16_float arg2)
      { return (xlns16_canon(arg1.x)<=xlns16_canon(arg2.x)); }
    friend int operator>=(xlns16_float arg1, xlns16_float arg2)
      { return (xlns16_canon(arg1.x)>=xlns16_canon(arg2.x)); }
    friend int operator<(xlns16_float arg1, xlns16_float arg2)
      { return (xlns16_canon(arg1.x)<xlns16_canon(arg2.x)); }
    friend int operator>(xlns16_float arg1, xlns16_float arg2)
      { return (xlns16_canon(arg1.x)>xlns16_canon(arg2.x)); }
    friend int operator==(xlns16_float arg1, float arg2);
    friend int operator!=(xlns16_float arg1, float arg2);
    friend int operator<=(xlns16_float arg1, float arg2);
    friend int operator>=(xlns16_float arg1, float arg2);
    friend int operator<(xlns16_float arg1, float arg2);
    friend int operator>(xlns16_float arg1, float arg2);
};

// ============================================================================
// Cache and access functions — declarations
// ============================================================================

xlns16 xlns16_internal(xlns16_float y);
float xlns16_2float(xlns16_float y);
xlns16_float float2xlns16_(float y);

// ============================================================================
// Math function overloads — declarations
// ============================================================================

xlns16_float sin(xlns16_float x);
xlns16_float cos(xlns16_float x);
xlns16_float exp(xlns16_float x);
xlns16_float log(xlns16_float x);
xlns16_float atan(xlns16_float x);
xlns16_float sqrt(xlns16_float x);
xlns16_float abs(xlns16_float x);

#endif // XLNS16_H