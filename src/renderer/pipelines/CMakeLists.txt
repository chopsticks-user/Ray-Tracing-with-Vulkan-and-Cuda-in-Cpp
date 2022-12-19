
add_library(neko_renderer_pipelines
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics.cpp
)
target_link_libraries(neko_renderer_pipelines
    PUBLIC compiler_flags
    PRIVATE neko_core
)
