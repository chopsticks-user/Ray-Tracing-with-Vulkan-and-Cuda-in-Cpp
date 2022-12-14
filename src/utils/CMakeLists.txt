
add_library(neko_utils STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/json_utils.cpp
)
target_link_libraries(neko_utils PUBLIC
    glfw vulkan

    # glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi
)