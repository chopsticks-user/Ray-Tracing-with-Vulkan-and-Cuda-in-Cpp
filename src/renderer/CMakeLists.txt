add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/basic)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/commands)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/devices)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pipelines)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_library(neko_renderer ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp)
target_link_libraries(neko_renderer
    PUBLIC compiler_flags
    PRIVATE neko_utils
    PRIVATE neko_threads
    PRIVATE neko_renderer_basic
    PRIVATE neko_renderer_devices
    PRIVATE neko_renderer_commands
    PRIVATE neko_renderer_pipelines
    PRIVATE neko_renderer_resources
)
