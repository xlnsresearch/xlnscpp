cmake_minimum_required(VERSION 3.15)
project(xlnscpp 
    VERSION 1.0.0
    DESCRIPTION "C++ package for Logarithmic Number System eXperimentation"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BUILD_TESTS "Build test executables" ON)
option(BUILD_BENCHMARKS "Build benchmark executables" ON)
option(ENABLE_WARNINGS "Enable compiler warnings" ON)

# Compile-time configuration options for XLNS16
option(XLNS16_IDEAL "Use ideal floating-point Gaussian Log for xlns16" OFF)
option(XLNS16_ALT "Use alternative addition algorithm for xlns16" OFF)
option(XLNS16_TABLE "Use table-based conversion for xlns16" OFF)
option(XLNS16_ALTOPT "Use less accurate LPVIP algorithm for xlns16" OFF)

# Compile-time configuration options for XLNS32
option(XLNS32_IDEAL "Use ideal floating-point Gaussian Log for xlns32" OFF)
option(XLNS32_ALT "Use alternative addition algorithm for xlns32" OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Subdirectories
add_subdirectory(src)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# Installation
install(FILES 
    xlns16.cpp
    xlns32.cpp
    xlns16cvtbl.h
    xlns16revcvtbl.h
    xlns16sbdbtbl.h
    xlns32tbl.h
    DESTINATION include/xlnscpp
)

# Print configuration summary
message(STATUS "")
message(STATUS "xlnscpp Configuration Summary:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Tests: ${BUILD_TESTS}")
message(STATUS "  Build Benchmarks: ${BUILD_BENCHMARKS}")
message(STATUS "")
message(STATUS "XLNS16 Options:")
message(STATUS "  XLNS16_IDEAL: ${XLNS16_IDEAL}")
message(STATUS "  XLNS16_ALT: ${XLNS16_ALT}")
message(STATUS "  XLNS16_TABLE: ${XLNS16_TABLE}")
message(STATUS "  XLNS16_ALTOPT: ${XLNS16_ALTOPT}")
message(STATUS "")
message(STATUS "XLNS32 Options:")
message(STATUS "  XLNS32_IDEAL: ${XLNS32_IDEAL}")
message(STATUS "  XLNS32_ALT: ${XLNS32_ALT}")
message(STATUS "")