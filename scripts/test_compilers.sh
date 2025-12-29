#!/bin/bash
# Test build with different compilers

COMPILERS=("g++" "clang++")

for compiler in "${COMPILERS[@]}"; do
    if command -v $compiler &> /dev/null; then
        echo "Testing with $compiler..."
        rm -rf build
        cmake -B build -DCMAKE_CXX_COMPILER=$compiler
        cmake --build build
        
        if [ $? -eq 0 ]; then
            echo "✓ $compiler: SUCCESS"
        else
            echo "✗ $compiler: FAILED"
            exit 1
        fi
    else
        echo "⚠ $compiler not found, skipping"
    fi
done

echo "All compilers tested successfully!"