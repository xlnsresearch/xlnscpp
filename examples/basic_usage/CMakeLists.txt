cmake_minimum_required(VERSION 3.15)
project(xlnscpp_example)

# Add xlnscpp as subdirectory
add_subdirectory(../../ xlnscpp_build)

# Create example executable
add_executable(basic_example main.cpp)
target_link_libraries(basic_example PRIVATE xlns16 xlns32)