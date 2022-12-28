
add_library(NekoRendererVulkanResources OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/Images.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Buffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UsageSpecific.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ResourceUtils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Sampler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Descriptors.cpp
)
target_include_directories(NekoRendererVulkanResources
    PRIVATE ${PROJECT_SOURCE_DIR}/Source/Renderer/Vulkan
)
target_link_libraries(NekoRendererVulkanResources
    PRIVATE CompilerFlags
    PRIVATE NekoCore
)
