
add_library(neko_renderer_commands
    ${CMAKE_CURRENT_SOURCE_DIR}/commands.cpp
)
target_include_directories(neko_renderer_commands PRIVATE
    ${PROJECT_SOURCE_DIR}/src/renderer/devices
)
target_link_libraries(neko_renderer_commands
    PUBLIC compiler_flags
    PRIVATE neko_utils
)
