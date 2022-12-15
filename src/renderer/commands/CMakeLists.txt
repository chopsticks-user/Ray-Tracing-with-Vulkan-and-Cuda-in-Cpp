
add_library(neko_renderer_commands
    ${CMAKE_CURRENT_SOURCE_DIR}/commands.cpp
)
target_link_libraries(neko_renderer_commands PRIVATE neko_utils)
