
add_library(neko_renderer_devices
    ${CMAKE_CURRENT_SOURCE_DIR}/logical_device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/physical_device.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/queues.cpp
)
target_include_directories(neko_renderer_devices PRIVATE
    ${PROJECT_SOURCE_DIR}/src/renderer/basic
)
target_link_libraries(neko_renderer_devices
    PUBLIC compiler_flags
    PRIVATE neko_core
)