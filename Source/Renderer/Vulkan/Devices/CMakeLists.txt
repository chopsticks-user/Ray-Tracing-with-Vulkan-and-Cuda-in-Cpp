
add_library(NekoRendererVulkanDevices OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/Logical.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Physical.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Queue.cpp
)
target_include_directories(NekoRendererVulkanDevices
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Renderer/Vulkan
)
target_link_libraries(NekoRendererVulkanDevices
    PRIVATE CompilerFlags
    PRIVATE NekoCore
)