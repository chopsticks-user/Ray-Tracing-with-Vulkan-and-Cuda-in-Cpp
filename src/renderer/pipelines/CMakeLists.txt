
add_library(neko_renderer_pipelines
    ${CMAKE_CURRENT_SOURCE_DIR}/compute.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_objects.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_module.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_pass.cpp
)
target_link_libraries(neko_renderer_pipelines
    PUBLIC compiler_flags
    PRIVATE neko_core
)
