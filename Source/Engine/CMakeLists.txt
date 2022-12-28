
add_library(NekoEngine
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp
)
target_include_directories(NekoEngine PRIVATE
    ${PROJECT_SOURCE_DIR}/Source/Renderer/Generic
    ${PROJECT_SOURCE_DIR}/Source/Renderer
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Renderer/Vulkan
)
target_link_libraries(NekoEngine
    PRIVATE CompilerFlags
    PRIVATE NekoCore
    PRIVATE NekoRenderer
)