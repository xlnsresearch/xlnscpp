# ── CLI tools for Gaussian Log cross-validation ─────────────────────────────

# Helper function
function(xlns_add_tool name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE xlns::xlns)
endfunction()

# Ideal sb/db computation (used by Python cross-validation scripts)
xlns_add_tool(sb16 sb16.cpp)
target_compile_definitions(sb16 PRIVATE xlns16_ideal)

xlns_add_tool(db16 db16.cpp)
target_compile_definitions(db16 PRIVATE xlns16_ideal)

# LPVIP (Mitchell) sb/db computation
xlns_add_tool(sbmit16 sbmit16.cpp)

xlns_add_tool(dbmit16 dbmit16.cpp)
