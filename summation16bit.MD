# Alternatives for summation (and related) operations in xlns16

inline xlns16 xlns16_sum_monte(const xlns16 *a, size_t n) 
inline xlns16 xlns16_vec_dot_monte(const xlns16 *a, const xlns16 *b, size_t n) 
inline void xlns16_softmax_monte(const xlns16 *a, xlns16 *c, size_t n) 
inline void xlns16_layernorm_monte(const xlns16 *x, xlns16 *out,const xlns16 *gamma, const xlns16 *beta,size_t n, float eps) 

