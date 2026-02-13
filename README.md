# XLNSCPP

**A C++ library for Logarithmic Number System eXperimentation.**

XLNSCPP provides 16-bit and 32-bit Logarithmic Number System (LNS) arithmetic — based on the same Gaussian-log foundation (sb/db) as the [Python xlns](https://github.com/xlnsresearch/xlns) package — but with a different, hardware-friendly internal storage format.

---

## Features

- **CMake-native library target** (`xlns::xlns`) with install + `find_package()` support
- **16-bit LNS** (`xlns16`) — roughly similar to `bfloat16` (1 sign + 8 int-log₂ + 7 frac-log₂)
- **32-bit LNS** (`xlns32`) — roughly similar to IEEE 754 `float` (1 sign + 8 int-log₂ + 23 frac-log₂)
- **Two APIs**: low-level C functions (`xlns16_add`, `xlns32_mul`, …) and C++ operator-overloaded classes (`xlns16_float`, `xlns32_float`)
- **Multiple sb/db strategies**: ideal (math.h), LPVIP approximation, cotransformation/interpolation, full table lookup
- **Compile-time configuration** via `#define` macros
- **Test, benchmark, and CLI validation tools** included

---

## Repository Structure

```
xlnscpp/
├── CMakeLists.txt              Root build file
├── .gitignore
├── .clang-format
├── LICENSE
├── README.md
│
├── include/xlns/               Public headers
│   ├── xlns.h                  Convenience header (includes both)
│   ├── xlns16.h                16-bit API declarations
│   ├── xlns32.h                32-bit API declarations
│   └── tables/                 Precomputed lookup tables
│       ├── xlns16_cvtbl.h       LNS16 → float conversion table
│       ├── xlns16_revcvtbl.h    float → LNS16 conversion table
│       ├── xlns16_sbdbtbl.h     16-bit sb/db direct lookup
│       └── xlns32_tbl.h         32-bit sb interpolation tables
│
├── src/                        Library implementations
│   ├── xlns16.cpp
│   └── xlns32.cpp
│
├── test/                       Test programs & benchmarks
│   ├── CMakeLists.txt
│   ├── unit/
│   │   ├── xlns16_test.cpp      16-bit arithmetic test suite
│   │   ├── xlns32_test.cpp      32-bit arithmetic test suite
│   │   ├── xlns_both_test.cpp   Cross-library coexistence test
│   │   └── xlns16_testcase.h    Compile-time test config matrix
│   ├── functional/
│   │   ├── xlns16_funtest.cpp   Interactive 16-bit math functions
│   │   └── xlns32_funtest.cpp   Interactive 32-bit math functions
│   └── benchmark/
│       └── time16_benchmark.cpp Performance benchmark
│
├── tools/                      CLI tools for Gaussian log validation
│   ├── CMakeLists.txt
│   ├── sb16.cpp                  Ideal sb computation
│   ├── db16.cpp                  Ideal db computation
│   ├── sbmit16.cpp               LPVIP sb computation
│   └── dbmit16.cpp               LPVIP db computation
│
├── scripts/                    Python cross-validation scripts
│   ├── sbtest.py                 Compare ideal sb (C++ vs Python)
│   ├── dbtest.py                 Compare ideal db (C++ vs Python)
│   ├── sblptest.py               Compare LPVIP sb (C++ vs Python)
│   └── dblptest.py               Compare LPVIP db (C++ vs Python)
│
└── cmake/modules/              CMake packaging support
    └── xlnsConfig.cmake.in

```

---

## Quick Start

### Using the library

```cpp
// my_program.cpp
#include <iostream>
#include <xlns/xlns32.h>

int main() {
    xlns32_float a = 3.14f;
    xlns32_float b = 2.71f;
    std::cout << "a + b = " << (a + b) << std::endl;
    return 0;
}
```

```cmake
# CMakeLists.txt
add_executable(my_program my_program.cpp)
target_link_libraries(my_program PRIVATE xlns::xlns)
```

### Building with CMake

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

This builds all tests, tools, and benchmarks by default. Control what gets built with:

```bash
cmake -B build \
    -DXLNS_BUILD_TESTS=ON \
    -DXLNS_BUILD_TOOLS=ON \
    -DXLNS_BUILD_BENCHMARKS=ON
```

### Running tests

```bash
./build/test/xlns16_test
./build/test/xlns32_test
./build/test/xlns_both_test
```

### Running benchmarks

```bash
./build/test/time16_benchmark
```

### Cross-validation with Python xlns

```bash
# Build the CLI tools first
cmake --build build --target sb16 db16 sbmit16 dbmit16

# Run from the scripts/ directory
cd scripts
python sbtest.py
python dbtest.py
```

---

## Compile-Time Configuration

XLNS behavior is selected at compile time via macros in `xlns16.h` / `xlns32.h`.

Important: when using the CMake library target (`xlns::xlns`), those macros must be applied to the library build itself (for example via `target_compile_definitions(xlns PUBLIC ...)` in your superproject), not only in consumer `.cpp` files.

### 16-bit (`xlns16.h` / `xlns16.cpp`)

| Macro | Effect |
|-------|--------|
| `xlns16_ideal` | Use math.h for sb/db (most accurate) |
| `xlns16_alt` | Streamlined addition for modern architectures |
| `xlns16_altopt` | Less accurate but fewer-op LPVIP (requires `xlns16_alt`) |
| `xlns16_table` | Table-based conversion and sb/db (fastest, ~1 MB) |

### 32-bit (`xlns32.h` / `xlns32.cpp`)

| Macro | Effect |
|-------|--------|
| `xlns32_ideal` | Use math.h for sb/db (most accurate) |
| `xlns32_alt` | Streamlined addition for modern architectures |
| `xlns32_arch16` | Use `long` types for 16-bit architectures |

### Recommended combinations

| Use case | Defines |
|----------|---------|
| Maximum accuracy | `xlns16_ideal` / `xlns32_ideal` |
| Best speed (16-bit) | `xlns16_alt` + `xlns16_table` |
| Good speed, no tables | `xlns16_alt` |
| Legacy architecture | (none — defaults to original algorithm) |

---

## Using as a CMake Dependency

### Via `add_subdirectory`

```cmake
add_subdirectory(xlnscpp)
target_link_libraries(my_target PRIVATE xlns::xlns)
```

### Via `FetchContent`

```cmake
include(FetchContent)
FetchContent_Declare(xlnscpp
    GIT_REPOSITORY https://github.com/xlnsresearch/xlnscpp.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(xlnscpp)
target_link_libraries(my_target PRIVATE xlns::xlns)
```

### Via `find_package` (after install)

```bash
cmake --install build --prefix /usr/local
```

```cmake
find_package(xlns REQUIRED)
target_link_libraries(my_target PRIVATE xlns::xlns)
```

---

## Internal Representation

Unlike the Python xlns, internal representation is **not** two's complement — it is offset by a constant (`logsignmask`):

```
+------+-------------------------+
| sign | int(log₂) . frac(log₂) |
+------+-------------------------+
```

- **16-bit**: 1 sign + 8 int + 7 frac (similar to bfloat16)
- **32-bit**: 1 sign + 8 int + 23 frac (similar to float32)
- Exact representation of 0.0; no subnormals or NaNs

For implementation details, see comments in [src/xlns16.cpp](src/xlns16.cpp) and [src/xlns32.cpp](src/xlns32.cpp).

---

## Sister Projects

- **[xlns](https://github.com/xlnsresearch/xlns)** — Python LNS library
- **[xlnscuda](https://github.com/xlnsresearch/xlnscuda)** — CUDA device routines

---

## References

M. G. Arnold, et al. "Arithmetic cotransformations in the Real and Complex Logarithmic Number Systems," *IEEE Trans. Comput.*, vol. 47, no. 7, pp. 777–786, July 1998.

M. G. Arnold, "LPVIP: A Low-power ROM-Less ALU for Low-Precision LNS," *14th International Workshop on Power and Timing Modeling, Optimization and Simulation*, LNCS 3254, pp. 675–684, Santorini, Greece, Sept. 2004.

---

## License

MIT License — Copyright 1999–2025 Mark G. Arnold. See [LICENSE](LICENSE) for details.


