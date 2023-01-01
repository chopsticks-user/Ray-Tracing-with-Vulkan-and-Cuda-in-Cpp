
add_library(NekoRendererVulkanPipelines OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/Compute.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Graphics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ShaderObjects.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ShaderModules.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderPass.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PipelineLayout.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PipelineCache.cpp
)
target_include_directories(NekoRendererVulkanPipelines
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Renderer/Vulkan
)
target_link_libraries(NekoRendererVulkanPipelines
    PRIVATE CompilerFlags
    PRIVATE NekoCore
)
