
add_library(neko_renderer_pipelines
    ${CMAKE_CURRENT_SOURCE_DIR}/compute.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_objects.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_module.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/render_pass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_layout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/pipeline_cache.cpp
)
target_include_directories(neko_renderer_pipelines
    PRIVATE ${PROJECT_SOURCE_DIR}/src/renderer/devices
    PRIVATE ${PROJECT_SOURCE_DIR}/src/renderer/resources
)
target_link_libraries(neko_renderer_pipelines
    PUBLIC compiler_flags
    PRIVATE neko_core
)
