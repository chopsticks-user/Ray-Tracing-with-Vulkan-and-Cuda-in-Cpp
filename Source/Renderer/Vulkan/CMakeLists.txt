
add_library(NekoRendererVulkan
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Extensions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Surface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Swapchain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Synchronization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Framebuffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Devices/Logical.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Devices/Physical.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Devices/Queue.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Commands/Commands.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Pipelines/ShaderModules.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Pipelines/ShaderObjects.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Pipelines/RenderPass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Pipelines/PipelineLayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Pipelines/PipelineCache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Pipelines/Graphics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Pipelines/Compute.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/ResourceUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Buffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Images.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/UsageSpecific.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Descriptors.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Sampler.cpp
)
target_include_directories(NekoRendererVulkan
    PUBLIC ${PROJECT_SOURCE_DIR}/Source/Renderer/Vulkan
)
target_link_libraries(NekoRendererVulkan
    PRIVATE CompilerFlags
    PRIVATE NekoCore
    PRIVATE GLFW
    PRIVATE Vulkan
)
