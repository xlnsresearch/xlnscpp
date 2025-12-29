#!/bin/bash
# Comprehensive build validation script

set -e  # Exit on error

echo "===== xlnscpp Build Validation ====="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print status
print_status() {
    if [ $1 -eq 0 ]; then
        echo -e "${GREEN}✓${NC} $2"
    else
        echo -e "${RED}✗${NC} $2"
        exit 1
    fi
}

# Clean previous builds
echo "Cleaning previous builds..."
rm -rf build
print_status $? "Clean completed"

# Test 1: Default configuration
echo ""
echo "Test 1: Default Configuration"
cmake -B build -S .
print_status $? "CMake configuration"

cmake --build build
print_status $? "Build"

cd build && ctest --output-on-failure
print_status $? "Tests"
cd ..

# Test 2: Debug build
echo ""
echo "Test 2: Debug Build"
rm -rf build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
print_status $? "Debug configuration"

cmake --build build
print_status $? "Debug build"

# Test 3: XLNS16 ideal mode
echo ""
echo "Test 3: XLNS16 Ideal Mode"
rm -rf build
cmake -B build -S . -DXLNS16_IDEAL=ON
print_status $? "XLNS16 ideal configuration"

cmake --build build
print_status $? "XLNS16 ideal build"

cd build && ctest --output-on-failure
print_status $? "XLNS16 ideal tests"
cd ..

# Test 4: XLNS16 table mode
echo ""
echo "Test 4: XLNS16 Table Mode"
rm -rf build
cmake -B build -S . -DXLNS16_TABLE=ON
print_status $? "XLNS16 table configuration"

cmake --build build
print_status $? "XLNS16 table build"

# Test 5: All optimizations
echo ""
echo "Test 5: All Optimizations"
rm -rf build
cmake -B build -S . \
    -DXLNS16_ALT=ON \
    -DXLNS16_TABLE=ON \
    -DXLNS32_ALT=ON
print_status $? "All optimizations configuration"

cmake --build build
print_status $? "All optimizations build"

cd build && ctest --output-on-failure
print_status $? "All optimizations tests"
cd ..

# Test 6: Minimal build (no tests)
echo ""
echo "Test 6: Minimal Build"
rm -rf build
cmake -B build -S . \
    -DBUILD_TESTS=OFF \
    -DBUILD_BENCHMARKS=OFF
print_status $? "Minimal configuration"

cmake --build build
print_status $? "Minimal build"

# Summary
echo ""
echo -e "${GREEN}===== All Tests Passed! =====${NC}"
echo ""
echo "Build system is working correctly."