# Alternatives for summation (and related) operations in xlns16

`xlns16.cpp` provides functions that include performing the summation of an array of `xlns16` values:

`xlns16_sum(a,n)` returns the `xlns16` sum of the `n` elements of the `xlns16` array, `a`.

`xlns16_vec_dot(a,b,n)` returns the `xlns16` dot product of the `xlns16` arrays, `a` and `b`.

`xlns16_softmax(a,c,n)` computes the softmax of `a` and outputs it in `c`. 

`xlns16_layernorm(x,out,gamma,beta,n,eps)` computes the layernorm of `x` as `out`.

The above are implemented sequentially using `xlns16_add`.  A similar set of operations are available in `xlns32.cpp`

The problem with the above is that when the `xlns16` array being summed is long, it is likely the accumulator will become stuck at a highly incorrect value because the absolute value of partial sum is much larger that the individual element being added.  In such a case, the most accurate result that `xlns16_add` can return is the unchanged value of the partial sum.  To overcome this, one could sum such `xlns16` values using an `xlns32` accumulator, but that is very expensive.  

Other alternatives are provided that are (on average) more accurate the `xlns16`-only accumulation, but which are more hardware-efficient than pure `xlns32` summation.  One alternative uses `xlns32lpvip.cpp` summation (less costly than `xlns32`), which takes the `xlns16` array, computes the approximate 32-bit sum, and returns it as `xlns16`.  This approach keeps full 32-bit accumulation, but is approximate: 

`xlns16_sum_lpvip32(a,n)` 

`xlns16_vec_dot_lpvip32(a,b,n)` 

`xlns16_softmax_lpvip32(a,c,n)` 

`xlns16_layernorm_lpvip32(x,out,gamma,beta,n,eps)` 

The other approach uses Monte-Carlo LNS (MCLNS), which is given in doi:10.1049/iet-cta.2010.0736 (and some earlier papers cited there).   The main purpose of MCLNS  is to sum a large sequence of `xlns16` values without having to keep an `xlns32` accumulator, which will make the hardware more efficient.  This is most successful the result is not near zero;
it is less effective when the sum is near zero (catastrophic cancelation).   There are two alternative implementations of the following functions (`xlns16monte.cpp` and `xlns16monte_bylpvip.cpp`; the latter needs `xlns32lpvip.cpp` included first; the former does not need this):


`xlns16_sum_monte(a,n)` 

`xlns16_vec_dot_monte(a,b,n)` 

`xlns16_softmax_monte(a,c,n)` 

`xlns16_layernorm_monte(x,out,gamma,beta,n,eps)` 

There is a test routine, `test16lpvip32monte.cpp` that illustrates these functions.
