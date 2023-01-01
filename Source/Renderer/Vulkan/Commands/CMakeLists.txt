
add_library(NekoRendererVulkanCommands OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/Commands.cpp
)
target_include_directories(NekoRendererVulkanCommands
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Renderer/Vulkan
)
target_link_libraries(NekoRendererVulkanCommands
    PRIVATE CompilerFlags
    PRIVATE NekoCore
)
