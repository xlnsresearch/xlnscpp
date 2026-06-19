# xlnscpp
XLNSCPP: a C++ package for Logarithmic Number System eXperimentation

This repository provides `xlns16.cpp` and `xlns32.cpp` along with a few programs that illustrate their use. They are based on similar math foundation (Gaussian logs, sb and db) as the Python xlns repository, but unlike the Python they use different internal storage format.

Unlike the Python xlns, here internal representation is not twos complement; it is offset by a constant (`xlns16_logsignmask` or `xlns32_logsignmask`). With the 16-bit format of `xlns16.cpp`, this is roughly similar to `bfloat16` (1 sign bit, 8 `int(log2)` bits, 7 `frac(log2)` bits). With the 32-bit format of `xlns32.cpp`, this is roughly similar to `float` (1 sign bit, 8 `int(log2)` bits, 23 `frac(log2)` bits). There is an exact representation of 0.0, but no subnormals or NaNs.

There are two ways to use this library: function calls (like `xlns16_add` or `xlns32_add`) that operate on integer representations (`typedef` as `xlns_16` or `xlns_32`) that represent the LNS value; or C++ overloaded operators that operate on an LNS class (either `xlns16_float` or `xlns32_float`).  The functions are a bit faster but overloading is easier. 

All of the global symbols used begin with either `xlns16` and `xlns32`.  There are several compile-time options indicated by defining macros before including `xlns16.cpp` and `xlns32.cpp` in the main program.  Defining `xlns16_ideal` or `xlns32_ideal` causes the Gaussian Log computation to occur as accurately as possible by doing it in floating point. Omitting this gives a cotransformation/interpolation approximation for 32-bit (at a cost of a few 100K bytes) and a LPVIP approximation for 16-bit (when `xlns16_altopt` is also defined, a less accurate LPVIP algorithm is used).  Defining `xlns16_alt` or `xlns_32alt` uses an addition algorithm that reduces branching (in the sometimes false hope of improved performance on modern architectures).  Omitting this defaults to an equivalent algorithm that runs better on older architectures (like say 8086). Defining `xlns16_table` causes conversion to/from float to occur from tables.  When `xlns16_alt` is also defined, Gaussian Log computation comes from tables.  Both of these improve speed at the cost of less than one megabyte.  The file `xlns16testcase.h` itemizes the meaningful combinations of these options to help automate the regression testing of these options.

The Python and C++ code that begin with `sb` and `db` work together to test whether ideal and LPVIP Gaussian Log computations in `xlns16.cpp` match what the Python xlns library provides.

For details on efficient summation of `xlns16` arrays, see [summation16bit.MD] .

There is a sister repository, xlnscuda, which has related routines that work on CUDA devices.

 # References

M. G. Arnold, et al. “Arithmetic cotransformations in the Real and
Complex Logarithmic Number Systems,” _IEEE Trans. Comput._, vol. 47,
no. 7, pp.777–786, July 1998.


M. G. Arnold, "LPVIP: A Low-power ROM-Less ALU for Low-Precision LNS," 
_14th International Workshop on Power and Timing Modeling, Optimization and Simulation_,
LNCS 3254, pp.675-684, Santorini, Greece, Sept. 2004.


