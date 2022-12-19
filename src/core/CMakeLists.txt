
add_library(neko_core
    ${CMAKE_CURRENT_SOURCE_DIR}/core.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/settings.cpp
)
target_include_directories(neko_core
    PRIVATE ${PROJECT_SOURCE_DIR}/modules/json/single_include
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(neko_core
    PUBLIC compiler_flags
    PUBLIC glfw
    PUBLIC vulkan
)

# glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi