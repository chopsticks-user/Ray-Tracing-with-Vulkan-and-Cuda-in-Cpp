
add_library(NekoRendererVulkanBasic OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Surface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Swapchain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framebuffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Extensions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Synchronization.cpp
)
target_include_directories(NekoRendererVulkanBasic
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Renderer/Vulkan
)
target_link_libraries(NekoRendererVulkanBasic
    PRIVATE CompilerFlags
    PRIVATE NekoCore
)
