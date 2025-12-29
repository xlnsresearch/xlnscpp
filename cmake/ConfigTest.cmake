# Test script for validating different configuration combinations
# This helps ensure all compile-time options work correctly

set(CONFIGURATIONS
    "default"
    "xlns16-ideal"
    "xlns16-table"
    "xlns16-alt"
    "xlns16-all-opts"
    "xlns32-ideal"
    "xlns32-alt"
    "all-ideal"
)

message(STATUS "Testing ${CMAKE_CURRENT_LIST_LENGTH} configurations...")

foreach(config ${CONFIGURATIONS})
    message(STATUS "Testing configuration: ${config}")
    execute_process(
        COMMAND ${CMAKE_COMMAND} --preset ${config}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE result
    )
    
    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Configuration ${config} failed!")
    endif()
endforeach()