cmake_minimum_required(VERSION 3.14)
project(xlnscpp
    VERSION 1.0.0
    DESCRIPTION "XLNSCPP: A C++ library for Logarithmic Number System eXperimentation"
    HOMEPAGE_URL "https://github.com/xlnsresearch/xlnscpp"
    LANGUAGES CXX
)

# ── Standard CMake settings ──────────────────────────────────────────────────

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Options ──────────────────────────────────────────────────────────────────

option(XLNS_BUILD_TESTS   "Build test programs"          ON)
option(XLNS_BUILD_TOOLS   "Build CLI tools (sb16, db16)" ON)
option(XLNS_BUILD_BENCHMARKS "Build benchmarks"          ON)

# ── Header-only library target ───────────────────────────────────────────────

add_library(xlns INTERFACE)
target_include_directories(xlns INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(xlns INTERFACE m)  # libm for math.h

# Alias for use via add_subdirectory / FetchContent
add_library(xlns::xlns ALIAS xlns)

# ── Sub-directories ──────────────────────────────────────────────────────────

if(XLNS_BUILD_TESTS OR XLNS_BUILD_BENCHMARKS)
    add_subdirectory(test)
endif()

if(XLNS_BUILD_TOOLS)
    add_subdirectory(tools)
endif()

# ── Install rules ────────────────────────────────────────────────────────────

include(GNUInstallDirs)

install(TARGETS xlns
    EXPORT xlnsTargets
)

install(DIRECTORY include/xlns
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT xlnsTargets
    FILE xlnsTargets.cmake
    NAMESPACE xlns::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xlns
)

# ── Package config ───────────────────────────────────────────────────────────

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/xlnsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/xlnsConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xlns
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/xlnsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/xlnsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/xlnsConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xlns
)
