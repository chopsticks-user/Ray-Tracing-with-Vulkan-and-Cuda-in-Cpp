add_library(NekoCore
    ${CMAKE_CURRENT_SOURCE_DIR}/EngineConfigs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VKTemplates.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CoreImage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ThreadPool.cpp
)
target_include_directories(NekoCore
    PRIVATE ${PROJECT_SOURCE_DIR}/Modules/Json/single_include
    PRIVATE ${PROJECT_SOURCE_DIR}/Modules/Stb
    INTERFACE ${PROJECT_SOURCE_DIR}/Source/Core
)
target_link_libraries(NekoCore
    PUBLIC CompilerFlags
    PUBLIC glfw
    PUBLIC vulkan
)

# glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi