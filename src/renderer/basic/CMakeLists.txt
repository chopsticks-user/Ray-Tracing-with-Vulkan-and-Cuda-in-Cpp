
add_library(neko_renderer_basic
    ${CMAKE_CURRENT_SOURCE_DIR}/context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp
)
target_link_libraries(neko_renderer_basic PRIVATE neko_utils)
