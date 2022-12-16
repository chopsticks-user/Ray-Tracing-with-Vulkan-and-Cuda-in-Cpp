
add_library(neko_utils
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings.cpp
)
target_include_directories(neko_utils
    PRIVATE ${PROJECT_SOURCE_DIR}/modules/json/single_include
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(neko_utils
    PUBLIC compiler_flags
    PUBLIC glfw3
    PUBLIC vulkan
)

# glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi