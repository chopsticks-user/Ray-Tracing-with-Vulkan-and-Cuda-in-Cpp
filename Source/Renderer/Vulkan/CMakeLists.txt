
set(RendererVulkanSourceFiles )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Basic)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Commands)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/devices)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pipelines)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_library(NekoRenderer
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Extensions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Surface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Swapchain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Synchronization.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Basic/Framebuffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Devices/LogicalDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Devices/PhysicalDevice.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Devices/Queues.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp
)
target_include_directories(NekoRenderer
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${PROJECT_SOURCE_DIR}/Modules/TinyObjLoader
)
target_link_libraries(NekoRenderer
    PUBLIC CompilerFlags
    PRIVATE NekoCore
)
