
add_library(neko_renderer_resources
    ${CMAKE_CURRENT_SOURCE_DIR}/images.cpp
)
target_link_libraries(neko_renderer_resources
    PUBLIC compiler_flags
    PRIVATE neko_utils
)
