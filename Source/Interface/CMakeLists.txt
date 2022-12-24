
add_library(NekoInterface INTERFACE)
target_include_directories(NekoInterface INTERFACE
    ${PROJECT_SOURCE_DIR}/Source
)
target_link_libraries(NekoInterface
    INTERFACE CompilerFlags
)