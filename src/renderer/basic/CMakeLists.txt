
add_library(neko_renderer_basic
    ${CMAKE_CURRENT_SOURCE_DIR}/context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/swapchain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/framebuffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/extensions.cpp
)
target_include_directories(neko_renderer_basic
    PRIVATE ${PROJECT_SOURCE_DIR}/src/renderer/devices
)
target_link_libraries(neko_renderer_basic
    PUBLIC compiler_flags
    PRIVATE neko_core
)
