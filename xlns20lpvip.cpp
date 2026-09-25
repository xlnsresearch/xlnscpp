#ifdef xlns32_zero
#else
  #define xlns32_aicasb
  #define xlns32_ideal
  #include "xlns20.cpp"
#endif

#define xlns32_F (xlns32_canonshift-8) 
/*
#ifdef xlns32_db_ideal
#else
  inline xlns32 xlns32_db_ideal(xlns32_signed z)  //only for singularity
  {
	return ((xlns32_signed) ((log( pow(2.0, ((double) z) / xlns32_scale) - 1 )/log(2.0))*xlns32_scale+.5));
  }
#endif
*/
  inline xlns32 xlns32_mitch(xlns32 z)
  {
     return (((1<<xlns32_F)+(z&((1<<xlns32_F)-1)))>>(-(z>>xlns32_F)));
  }

//need to define xlns32_aicasb if xlns32_add_lpvip is to give accurate summation



inline xlns32 xlns32_add_lpvip(xlns32 x, xlns32 y)
{
    xlns32 minxyl, maxxy, xl, yl, usedb, adjust, adjustez;
    xlns32_signed z;
    xl = x & xlns32_logmask;
    yl = y & xlns32_logmask;
    minxyl = (yl>xl) ? xl : yl;
    maxxy  = (xl>yl) ? x  : y;
    z = minxyl - (maxxy&xlns32_logmask);
    usedb = xlns32_signmask&(x^y); 
       xlns32_signed precond = (usedb==0) ? (
                                            #ifdef xlns32_aicasb
                                              (z > -(7<<(xlns32_F-1))) ? ((-z)>>3) : (7<<(xlns32_F-4)) 
                                            #else
                                              ((-z)>>3)
                                            #endif
                                            ) :          // -.125*z 
                (z < -(2<<xlns32_F)) ? 5<<(xlns32_F-3):        //  0.625
                                (z >> 2) + (9 << (xlns32_F-3));//  .25*z + 9/8
   //printf("%i %08x %08x\n",usedb,z,precond);
       xlns32_signed postcond = (z <= -(3<<xlns32_F)) ? 0: 
                            z >= -(3<<(xlns32_F-2)) ? -(1<<(xlns32_F-6)): //6)) : 
                                                      +(1<<(xlns32_F-6)); //6));
       xlns32_signed mitch = (-z >= 1<<xlns32_F)||(usedb==0) ? xlns32_mitch(z+precond) : 
                                          -xlns32_db_ideal(-z)-z; // use ideal for singularity
       adjust = usedb ? -mitch : (z==0) ? 1<<(xlns32_F) : mitch + postcond;
      adjustez = (z < -xlns32_esszer) ? 0 : adjust; 
    return ((z==0) && usedb) ? 
                     xlns32_zero :
                     xlns32_mul(maxxy, xlns32_logsignmask + adjustez);
}

#ifdef xlns16_zero


// Vector operations (critical for ggml MUL_MAT) using xlns16 ops with xlns32 lpvip accumulation

// Sum of array elements: result = Σ a[i]
inline xlns16 xlns16_sum_lpvip32(const xlns16 *a, size_t n) {
    if (n == 0) return xlns16_zero;
    xlns32 sum = ((xlns32)a[0])<< (xlns32_canonshift-15);
    for (size_t i = 1; i < n; i++) {
        sum = xlns32_add_lpvip(sum, ((xlns32)a[i])<<(xlns32_canonshift-15));
    }
    return sum>>(xlns32_canonshift-15);
}

// Vector dot product: result = Σ(a[i] * b[i])
inline xlns16 xlns16_vec_dot_lpvip32(const xlns16 *a, const xlns16 *b, size_t n) {
    if (n == 0) return xlns16_zero;
    xlns32 sum = ((xlns32)xlns16_mul(a[0], b[0]))<<
(xlns32_canonshift-15);
    for (size_t i = 1; i < n; i++) {
        sum = xlns32_add_lpvip(sum, ((xlns32)xlns16_mul(a[i], b[i]))<< (xlns32_canonshift-15);
    }
    return sum>> (xlns32_canonshift-15);
}

// Layer normalization: (x - mean) / sqrt(var + eps) * gamma + beta
inline void xlns16_layernorm_lpvip32(const xlns16 *x, xlns16 *out,
                       const xlns16 *gamma, const xlns16 *beta,
                       size_t n, float eps) {
    // compute mean
    xlns16 mean = xlns16_sum_lpvip32(x, n);
    mean = xlns16_div(mean, fp2xlns16((float)n));
    // compute variance
    xlns32 var = xlns32_zero;
    for (size_t i = 0; i < n; i++) {
        xlns16 diff = xlns16_sub(x[i], mean);
        var = xlns32_add_lpvip(var, ((xlns32)xlns16_mul(diff, diff))<< (xlns32_canonshift-15);
    }
    var = xlns32_div(var, fp2xlns32((float)n));
    // normalize
    xlns16 inv_std = fp2xlns16(1.0f / sqrt(xlns322fp(var) + eps));
    for (size_t i = 0; i < n; i++) {
        out[i] = xlns16_mul(xlns16_sub(x[i], mean), inv_std);
        if (gamma) out[i] = xlns16_mul(out[i], gamma[i]);
        if (beta)  out[i] = xlns16_add(out[i], beta[i]);
    }
}

// RMSNorm: dst[i] = x[i] / sqrt(mean(x^2) + eps).
// Same control flow as xlns16_rms_norm. Only the sum-of-squares reduction uses LPVIP
// (xlns32_add_lpvip), matching layernorm variance accumulation.
inline void xlns16_rms_norm_lpvip32(const xlns16 *x, xlns16 *dst, size_t n, xlns16 eps) {
    if (n == 0) return;
    xlns32 sum_sq = xlns32_zero;
    for (size_t i = 0; i < n; i++)
        sum_sq = xlns32_add_lpvip(sum_sq, ((xlns32)xlns16_square(x[i])) << (xlns32_canonshift-15);
    xlns16 mean = xlns16_mul(sum_sq >> (xlns32_canonshift-15), fp2xlns16(1.0f / (float)n));
    xlns16 inv_rms = xlns16_recip(xlns16_sqrt(xlns16_add(mean, eps)));
    for (size_t i = 0; i < n; i++)
        dst[i] = xlns16_mul(x[i], inv_rms);
}

// Softmax: exp(scale*a[i] - max) / sum(exp(scale*a[j] - max)).
// Same control flow as xlns16_softmax. Scale, mask bias, and max-sub use
// plain xlns16 ops; only the normalization sum uses xlns16_sum_lpvip32.
// a[i] == xlns16_neg_inf is treated as already-excluded and skips the scale
// multiply. c may alias a (in-place).
inline void xlns16_softmax_lpvip32(const xlns16 *a, xlns16 *c, size_t n,
                                    xlns16 scale = xlns16_one) {
    if (n == 0) return;
    xlns16 maxval = xlns16_neg_inf;
    for (size_t i = 0; i < n; i++) {
        xlns16 v = a[i];
        if (v != xlns16_neg_inf) v = xlns16_mul(v, scale);
        c[i] = v;
        if (xlns16_gt(c[i], maxval)) maxval = c[i];
    }
    for (size_t i = 0; i < n; i++)
        c[i] = xlns16_exp(xlns16_sub(c[i], maxval));
    xlns16 total = xlns16_sum_lpvip32(c, n);
    for (size_t i = 0; i < n; i++)
        c[i] = xlns16_div(c[i], total);
}

// Masked variant of xlns16_softmax_lpvip32. mask[i] is a pre-converted xlns16
// value: xlns16_neg_inf marks a masked-out position, xlns16_zero means "no
// mask", anything else is an additive bias (e.g. ALiBi).
inline void xlns16_softmax_masked_lpvip32(const xlns16 *a, const xlns16 *mask,
                                           xlns16 *c, size_t n,
                                           xlns16 scale = xlns16_one) {
    if (n == 0) return;
    xlns16 maxval = xlns16_neg_inf;
    for (size_t i = 0; i < n; i++) {
        xlns16 v = a[i];
        if (v != xlns16_neg_inf) {
            v = xlns16_mul(v, scale);
            if (mask[i] == xlns16_neg_inf) v = xlns16_neg_inf;
            else if (!xlns16_is_zero(mask[i])) v = xlns16_add(v, mask[i]);
        }
        c[i] = v;
        if (xlns16_gt(c[i], maxval)) maxval = c[i];
    }
    for (size_t i = 0; i < n; i++)
        c[i] = xlns16_exp(xlns16_sub(c[i], maxval));
    xlns16 total = xlns16_sum_lpvip32(c, n);
    for (size_t i = 0; i < n; i++)
        c[i] = xlns16_div(c[i], total);
}


#endif

