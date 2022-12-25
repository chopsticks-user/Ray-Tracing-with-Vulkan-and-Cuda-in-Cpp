
add_library(neko_renderer_basic TARGET_OBJECTS
    ${CMAKE_CURRENT_SOURCE_DIR}/Context/context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Instance/instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Surface/surface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Swapchain/swapchain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Framebuffers/framebuffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Extensions/extensions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Synchronization/Synchoronization.cpp
)
