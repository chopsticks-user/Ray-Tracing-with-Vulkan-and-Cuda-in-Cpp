
add_library(neko_renderer_resources
    ${CMAKE_CURRENT_SOURCE_DIR}/images.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/depth_buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uniform_buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
)
target_include_directories(neko_renderer_resources
    PRIVATE ${PROJECT_SOURCE_DIR}/src/renderer/commands
)
target_link_libraries(neko_renderer_resources
    PUBLIC compiler_flags
    PRIVATE neko_core
)
