//Monte-Carlo LNS (MCLNS) is given in doi:10.1049/iet-cta.2010.0736 (and some earlier papers cited there).
//The main purpose of MCLNS here is to sum a large sequence of xlns16 values (as in an LLM)
//without having to keep an xlns32 accumulator.  This is most successful the result is not near zero;
//it is less effective when the sum is near zero (catastrophic cancelation)
//This implementation has the advantage all ops occur in 16-bit

//MCLNS does not require quality "randomness"; the default is a max len linear feedback shift register
//alternatively, if 1 < xlns16_num_rand < 15 is defined, this code uses rand()
//  small xlns16_num_rand give poor results  

#ifdef xlns16_num_rand
        // if so, use that # bits from rand() 
#else   // otherwise need global for lin feedback shift reg 
  xlns16 xlns16_randombits= 7;  //any seed in 1 <= seed <= 65535;
#endif

inline xlns16 xlns16_monte_modifier(xlns16 x, xlns16 y)
{
    //This replacates code in xlns16_add (to obtain same z and usedb)
    xlns16 minxyl, maxxy, xl, yl, usedb, adjust, adjustez,result;
    xlns16_signed z;
    xl = x & xlns16_logmask;
    yl = y & xlns16_logmask;
    minxyl = (yl>xl) ? xl : yl;
    maxxy  = (xl>yl) ? x  : y;
    z = minxyl - (maxxy&xlns16_logmask);
    usedb = xlns16_signmask&(x^y); 

    //this chooses a random number
    #ifdef xlns16_num_rand     // use specified bits from rand() 
        xlns16 xlns16_randombits;
        xlns16_randombits = (rand()%(1<<xlns16_num_rand))<<(16-xlns16_num_rand);
    #else                     // use max len linear feedback shift reg
        xlns16 bit = xlns16_randombits & 1;
        xlns16_randombits >>= 1;
        if (bit) {
            xlns16_randombits ^= 0xB400u; // Mask for taps 16, 15, 13, 4
        }
    #endif

    //This sometimes returns +/-1 (for |z|>F) based on z (bigger z more prob result!=0) and usedb (+1 for sb;-1 for db)
    //precond=7/16 for both sb and db; sometimes bigger and sometimes smaller than ideal; good compromise
    result = (xlns16_randombits>>(16-xlns16_F)) + xlns16_mitch(z + (xlns16_F<<xlns16_F) + (7<<(xlns16_F-4))); // z+F+7/16
    result = result >> xlns16_F;
    result &= (z < -((xlns16_F)<<xlns16_F));
    if (usedb) 
        return -result;
    else
        return result; 
}


inline xlns16 xlns16_add_monte(xlns16 x, xlns16 y)
{
    xlns16 normres = xlns16_add(x,y);
    xlns16 modifier = xlns16_monte_modifier(x,y); //uses PRNG only here
    //return xlns16_add(x,y) + modifier; //xlns16_monte_modifier(x,y); //uses PRNG here
    if ((normres != x) && (normres != y)) 
        return normres;                   //best if define xlns16_ideal or xlns16_table
    else
        return normres + modifier;
}



// Vector operations (critical for ggml MUL_MAT) using MCLNS

// Sum of array elements: result = Σ a[i]
inline xlns16 xlns16_sum_monte(const xlns16 *a, size_t n) {
    if (n == 0) return xlns16_zero;
    xlns16 sum = a[0];
    for (size_t i = 1; i < n; i++) {
        sum = xlns16_add_monte(sum, a[i]);
    }
    return sum;
}

// Vector dot product: result = Σ(a[i] * b[i])
inline xlns16 xlns16_vec_dot_monte(const xlns16 *a, const xlns16 *b, size_t n) {
    if (n == 0) return xlns16_zero;
    xlns16 sum = xlns16_mul(a[0], b[0]);
    for (size_t i = 1; i < n; i++) {
        sum = xlns16_add_monte(sum, xlns16_mul(a[i], b[i]));
    }
    return sum;
}


// Softmax: exp(scale*a[i] - max) / sum(exp(scale*a[j] - max)).
// Same control flow as xlns16_softmax; only the normalization sum uses MCLNS
// (xlns16_sum_monte).
// a[i] == xlns16_neg_inf is treated as already-excluded and skips the scale
// multiply (scaling the sentinel could otherwise perturb its bit pattern).
// c may alias a (in-place): every pass only reads index i before writing
// index i, and the sum pass runs after all per-element writes complete.
inline void xlns16_softmax_monte(const xlns16 *a, xlns16 *c, size_t n,
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
    xlns16 total = xlns16_sum_monte(c, n);
    for (size_t i = 0; i < n; i++)
        c[i] = xlns16_div(c[i], total);
}

// Masked variant of xlns16_softmax_monte (same as xlns16_softmax_masked, with
// xlns16_sum_monte for normalization). mask[i] is a pre-converted xlns16 value:
// xlns16_neg_inf marks a masked-out position (forces the output to
// xlns16_neg_inf regardless of a[i]/scale), xlns16_zero means "no mask" for
// that position, anything else is treated as an additive bias (e.g. ALiBi).
inline void xlns16_softmax_masked_monte(const xlns16 *a, const xlns16 *mask, xlns16 *c,
                                         size_t n, xlns16 scale = xlns16_one) {
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
    xlns16 total = xlns16_sum_monte(c, n);
    for (size_t i = 0; i < n; i++)
        c[i] = xlns16_div(c[i], total);
}


// Layer normalization: (x - mean) / sqrt(var + eps) * gamma + beta
inline void xlns16_layernorm_monte(const xlns16 *x, xlns16 *out,
                       const xlns16 *gamma, const xlns16 *beta,
                       size_t n, float eps) {
    // compute mean
    xlns16 mean = xlns16_sum_monte(x, n);
    mean = xlns16_div(mean, fp2xlns16((float)n));
    // compute variance
    xlns16 var = xlns16_zero;
    for (size_t i = 0; i < n; i++) {
        xlns16 diff = xlns16_sub(x[i], mean);
        var = xlns16_add_monte(var, xlns16_mul(diff, diff));
    }
    var = xlns16_div(var, fp2xlns16((float)n));
    // normalize
    xlns16 inv_std = fp2xlns16(1.0f / sqrt(xlns162fp(var) + eps));
    for (size_t i = 0; i < n; i++) {
        out[i] = xlns16_mul(xlns16_sub(x[i], mean), inv_std);
        if (gamma) out[i] = xlns16_mul(out[i], gamma[i]);
        if (beta)  out[i] = xlns16_add(out[i], beta[i]);
    }
}




