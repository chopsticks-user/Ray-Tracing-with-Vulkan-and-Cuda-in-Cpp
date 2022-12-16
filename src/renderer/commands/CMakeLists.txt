
add_library(neko_renderer_commands
    ${CMAKE_CURRENT_SOURCE_DIR}/commands.cpp
)
target_link_libraries(neko_renderer_commands
    PUBLIC compiler_flags
    PRIVATE neko_utils
)
