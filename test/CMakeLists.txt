# ── Test executables ──────────────────────────────────────────────────────────

# Helper function to add a test target with common settings
function(xlns_add_test name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE xlns::xlns)
    # Allow test-local includes (e.g., xlns16_testcase.h)
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/unit)
endfunction()

# ── 16-bit unit tests ────────────────────────────────────────────────────────

# Default configuration (alt + altopt)
xlns_add_test(xlns16_test unit/xlns16_test.cpp)
target_compile_definitions(xlns16_test PRIVATE xlns16_alt xlns16_altopt)

# ── 32-bit unit tests ────────────────────────────────────────────────────────

xlns_add_test(xlns32_test unit/xlns32_test.cpp)

# ── Cross-library coexistence test ───────────────────────────────────────────

xlns_add_test(xlns_both_test unit/xlns_both_test.cpp)
target_compile_definitions(xlns_both_test PRIVATE xlns32_ideal xlns16_ideal)

# ── Automated regression: build each xlns16 configuration ────────────────────

set(XLNS16_CASES 0 1 2 3 4 5)
foreach(case_id IN LISTS XLNS16_CASES)
    set(tgt xlns16_test_case${case_id})
    add_executable(${tgt} unit/xlns16_test.cpp)
    target_link_libraries(${tgt} PRIVATE xlns::xlns)
    target_include_directories(${tgt} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/unit)
    target_compile_definitions(${tgt} PRIVATE xlns16case=${case_id})
endforeach()

# ── Functional tests ─────────────────────────────────────────────────────────

# Interactive function test (ideal mode)
xlns_add_test(xlns16_funtest functional/xlns16_funtest.cpp)
target_compile_definitions(xlns16_funtest PRIVATE xlns16_ideal)

# Interactive function test
xlns_add_test(xlns32_funtest functional/xlns32_funtest.cpp)

# ── Benchmark ────────────────────────────────────────────────────────────────

if(XLNS_BUILD_BENCHMARKS)
    xlns_add_test(time16_benchmark benchmark/time16_benchmark.cpp)
    target_compile_definitions(time16_benchmark PRIVATE xlns16_alt xlns16_table)
endif()
