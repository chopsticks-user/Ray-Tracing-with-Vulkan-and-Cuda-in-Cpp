# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/basic)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/commands)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/devices)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pipelines)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_library(neko_renderer STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/basic/context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/basic/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/basic/instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/basic/surface.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/devices/logical_device.cpp
)
target_include_directories(neko_renderer PUBLIC
    ${PROJECT_SOURCE_DIR}/src/utils
    ${PROJECT_SOURCE_DIR}/src/threads
)

# target_link_libraries(neko_renderer PRIVATE
# neko_utils
# neko_threads
# neko_renderer_basic
# neko_renderer_devices
# )
