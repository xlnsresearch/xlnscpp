# ── Test executables ──────────────────────────────────────────────────────────

# Helper function to add a test target with common settings
function(xlns_add_test name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE xlns::xlns)
    # Allow test-local includes (e.g., xlns16_testcase.h)
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()

# ── 16-bit tests ─────────────────────────────────────────────────────────────

# Default configuration (alt + altopt)
xlns_add_test(xlns16_test xlns16_test.cpp)
target_compile_definitions(xlns16_test PRIVATE xlns16_alt xlns16_altopt)

# Interactive function test (ideal mode)
xlns_add_test(xlns16_funtest xlns16_funtest.cpp)
target_compile_definitions(xlns16_funtest PRIVATE xlns16_ideal)

# ── 32-bit tests ─────────────────────────────────────────────────────────────

xlns_add_test(xlns32_test xlns32_test.cpp)

# Interactive function test
xlns_add_test(xlns32_funtest xlns32_funtest.cpp)

# ── Cross-library coexistence test ───────────────────────────────────────────

xlns_add_test(xlns_both_test xlns_both_test.cpp)
target_compile_definitions(xlns_both_test PRIVATE xlns32_ideal xlns16_ideal)

# ── Automated regression: build each xlns16 configuration ────────────────────

set(XLNS16_CASES 0 1 2 3 4 5)
foreach(case_id IN LISTS XLNS16_CASES)
    set(tgt xlns16_test_case${case_id})
    add_executable(${tgt} xlns16_test.cpp)
    target_link_libraries(${tgt} PRIVATE xlns::xlns)
    target_include_directories(${tgt} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(${tgt} PRIVATE xlns16case=${case_id})
endforeach()

# ── Benchmark ────────────────────────────────────────────────────────────────

if(XLNS_BUILD_BENCHMARKS)
    xlns_add_test(time16_benchmark time16_benchmark.cpp)
    target_compile_definitions(time16_benchmark PRIVATE xlns16_alt xlns16_table)
endif()
