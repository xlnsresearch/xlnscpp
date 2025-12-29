# Helper function to create test executables
function(add_xlns_test test_name source_file)
    add_executable(${test_name} ${CMAKE_SOURCE_DIR}/${source_file})
    
    # Link appropriate library based on test name
    if(${test_name} MATCHES ".*16.*")
        target_link_libraries(${test_name} PRIVATE xlns16)
    elseif(${test_name} MATCHES ".*32.*")
        target_link_libraries(${test_name} PRIVATE xlns32)
    elseif(${test_name} MATCHES ".*both.*")
        target_link_libraries(${test_name} PRIVATE xlns16 xlns32)
    endif()
    
    # Add as CTest test
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

# Add all test executables
add_xlns_test(xlns16test xlns16test.cpp)
add_xlns_test(xlns16funtest xlns16funtest.cpp)
add_xlns_test(xlns32test xlns32test.cpp)
add_xlns_test(xlns32funtest xlns32funtest.cpp)
add_xlns_test(xlnsbothtest xlnsbothtest.cpp)

# Special handling for tests with specific dependencies
add_executable(db16test ${CMAKE_SOURCE_DIR}/db16.cpp)
target_link_libraries(db16test PRIVATE xlns16)
add_test(NAME db16test COMMAND db16test)

add_executable(dbmit16test ${CMAKE_SOURCE_DIR}/dbmit16.cpp)
target_link_libraries(dbmit16test PRIVATE xlns16)
add_test(NAME dbmit16test COMMAND dbmit16test)

add_executable(sb16test ${CMAKE_SOURCE_DIR}/sb16.cpp)
target_link_libraries(sb16test PRIVATE xlns16)
add_test(NAME sb16test COMMAND sb16test)

add_executable(sbmit16test ${CMAKE_SOURCE_DIR}/sbmit16.cpp)
target_link_libraries(sbmit16test PRIVATE xlns16)
add_test(NAME sbmit16test COMMAND sbmit16test)

# Python validation tests (if Python is available)
find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
    add_test(NAME dbtest_py 
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/dbtest.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    
    add_test(NAME dblptest_py 
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/dblptest.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    
    add_test(NAME sbtest_py 
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/sbtest.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    
    add_test(NAME sblptest_py 
        COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/sblptest.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()