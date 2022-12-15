add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/basic)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/commands)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/devices)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pipelines)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_library(neko_renderer ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp)
target_link_libraries(neko_renderer PRIVATE
    neko_utils
    neko_threads
    neko_renderer_basic
    neko_renderer_devices
    neko_renderer_commands
    neko_renderer_pipelines
    neko_renderer_resources
)
