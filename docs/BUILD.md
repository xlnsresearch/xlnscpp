# Building xlnscpp - Detailed Guide

## Prerequisites

### Required
- C++ compiler with C++17 support:
  - GCC 7.0 or later
  - Clang 5.0 or later
  - MSVC 2017 or later
- CMake 3.15 or later

### Optional
- Python 3.6+ (for validation tests)
- Ninja build system (faster builds)
- ccache (faster rebuilds)

## Platform-Specific Instructions

### Linux (Ubuntu/Debian)
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential cmake python3

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)
ctest
```

### macOS
```bash
# Install dependencies (using Homebrew)
brew install cmake

# Build
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)
ctest
```

### Windows (MSVC)
```cmd
REM Using Visual Studio Developer Command Prompt
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
ctest -C Release
```

### Windows (MinGW)
```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
ctest
```

## Advanced Build Options

### Compiler Selection
```bash
# Use specific compiler
cmake -DCMAKE_CXX_COMPILER=clang++ ..

# Use specific compiler version
cmake -DCMAKE_CXX_COMPILER=g++-11 ..
```

### Optimization Levels
```bash
# Maximum optimization
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native" ..

# Debug with sanitizers
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address" ..
```

### Using Ninja
```bash
cmake -G Ninja ..
ninja
```

### Cross-Compilation

Example for ARM:
```bash
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/arm-toolchain.cmake ..
```

## Understanding Build Configurations

### XLNS16 Options

**xlns16_ideal**: Most accurate, slowest. Uses full floating-point arithmetic for Gaussian Log computation.

**xlns16_table**: Fast, uses ~1MB memory. Table-based conversion to/from float.

**xlns16_alt**: Reduced branching algorithm. May be faster on modern CPUs.

**xlns16_altopt**: Less accurate LPVIP algorithm. Faster but lower precision.

### XLNS32 Options

**xlns32_ideal**: Most accurate. Uses floating-point arithmetic for Gaussian Log.

**xlns32_alt**: Alternative addition algorithm with reduced branching.

### Recommended Configurations

**For development/testing:**
```bash
cmake --preset debug
```

**For production (speed priority):**
```bash
cmake --preset xlns16-all-opts
```

**For production (accuracy priority):**
```bash
cmake --preset all-ideal
```

## Troubleshooting

### "Command not found: cmake"
Install CMake from https://cmake.org/download/

### "C++ compiler not found"
Install a C++ compiler (see prerequisites above)

### Tests fail with "undefined reference to 'm'"
On some Linux systems, ensure math library is linked (already handled by CMakeLists.txt)

### Python tests skipped
Install Python 3.6+ if you want to run validation tests

### Build is slow
- Use `make -j$(nproc)` for parallel builds
- Consider using Ninja: `cmake -G Ninja ..`
- Use ccache: `cmake -DCMAKE_CXX_COMPILER_LAUNCHER=ccache ..`

## Build Artifacts

After building, you'll find:

build/
├── bin/               # Test executables
│   ├── xlns16test
│   ├── xlns32test
│   └── time16test
├── lib/               # Static libraries
│   ├── libxlns16.a
│   └── libxlns32.a
└── Testing/           # CTest results

## Integration into Your Project

### As a CMake subdirectory
```cmake
add_subdirectory(xlnscpp)
target_link_libraries(myproject PRIVATE xlns16 xlns32)
```

### Using installed files
```cmake
find_library(XLNS16_LIB xlns16 PATHS /usr/local/lib)
find_library(XLNS32_LIB xlns32 PATHS /usr/local/lib)
target_link_libraries(myproject PRIVATE ${XLNS16_LIB} ${XLNS32_LIB})
```