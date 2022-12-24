
add_library(NekoCore
    ${CMAKE_CURRENT_SOURCE_DIR}/EngineConfigs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CoreImage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ThreadPool.cpp
)
target_include_directories(NekoCore
    INTERFACE ${PROJECT_SOURCE_DIR}/Source/Core
)
target_link_libraries(NekoCore
    PUBLIC CompilerFlags
    PUBLIC GLFW
    PUBLIC Vulkan
    PRIVATE NlohmannJson
    PRIVATE StbImage
    PRIVATE SpdLog
    PRIVATE TinyObjLoader
)

# glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi