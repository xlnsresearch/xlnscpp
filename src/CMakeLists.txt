# XLNS16 Library
add_library(xlns16 STATIC
    ${CMAKE_SOURCE_DIR}/xlns16.cpp
)

target_include_directories(xlns16 PUBLIC
    ${CMAKE_SOURCE_DIR}
)

# Apply compile definitions based on options
if(XLNS16_IDEAL)
    target_compile_definitions(xlns16 PUBLIC xlns16_ideal)
endif()

if(XLNS16_ALT)
    target_compile_definitions(xlns16 PUBLIC xlns16_alt)
endif()

if(XLNS16_TABLE)
    target_compile_definitions(xlns16 PUBLIC xlns16_table)
endif()

if(XLNS16_ALTOPT)
    target_compile_definitions(xlns16 PUBLIC xlns16_altopt)
endif()

# XLNS32 Library
add_library(xlns32 STATIC
    ${CMAKE_SOURCE_DIR}/xlns32.cpp
)

target_include_directories(xlns32 PUBLIC
    ${CMAKE_SOURCE_DIR}
)

# Apply compile definitions based on options
if(XLNS32_IDEAL)
    target_compile_definitions(xlns32 PUBLIC xlns32_ideal)
endif()

if(XLNS32_ALT)
    target_compile_definitions(xlns32 PUBLIC xlns32_alt)
endif()

# Compiler warnings
if(ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(xlns16 PRIVATE /W4)
        target_compile_options(xlns32 PRIVATE /W4)
    else()
        target_compile_options(xlns16 PRIVATE -Wall -Wextra -Wpedantic)
        target_compile_options(xlns32 PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

# Link math library on Unix
if(UNIX AND NOT APPLE)
    target_link_libraries(xlns16 PUBLIC m)
    target_link_libraries(xlns32 PUBLIC m)
endif()