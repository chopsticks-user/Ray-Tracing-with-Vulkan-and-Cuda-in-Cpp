
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/basic)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/commands)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/devices)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pipelines)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_library(neko_renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.hpp
)
target_include_directories(neko_renderer PUBLIC
    ${PROJECT_SOURCE_DIR}/src/utils
    ${PROJECT_SOURCE_DIR}/src/threads
)
target_link_libraries(neko_renderer PRIVATE
    neko_utils
    neko_threads
    neko_renderer_basic
    neko_renderer_devices
)
