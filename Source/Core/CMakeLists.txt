
add_library(NekoCore
    ${CMAKE_CURRENT_SOURCE_DIR}/Configs/EngineConfigs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils/ImageLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Threading/ThreadPool.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger/Logger.cpp
)
target_include_directories(NekoCore
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Core
)
target_link_libraries(NekoCore
    PRIVATE CompilerFlags
    PRIVATE NlohmannJson
    PRIVATE StbImage
    PRIVATE SpdLog
    PRIVATE TinyObjLoader
)

# glfw vulkan dl pthread X11 Xxf86vm Xrandr Xi