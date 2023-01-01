
add_library(NekoRenderer
    GenericRenderer.cpp
)
target_include_directories(NekoRenderer
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Renderer
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Renderer/Vulkan
)
target_link_libraries(NekoRenderer
    PRIVATE CompilerFlags
    PRIVATE NekoCore
    PUBLIC NekoRendererVulkan
    PRIVATE TinyObjLoader
)
