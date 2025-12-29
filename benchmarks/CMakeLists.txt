# Benchmark executable
add_executable(time16test ${CMAKE_SOURCE_DIR}/time16test.cpp)
target_link_libraries(time16test PRIVATE xlns16)

# Add benchmark as a test (but mark it as benchmark)
add_test(NAME time16test COMMAND time16test)
set_tests_properties(time16test PROPERTIES LABELS "benchmark")