
set(libSourcePaths
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/events/events.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/threads/threads.cpp
    ${PROJECT_SOURCE_DIR}/include/neko.hpp

    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/basic/context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/basic/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/basic/instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/basic/surface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer/devices/logical_device.cpp
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/utils)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/events)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/renderer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/threads)

add_library(neko ${libSourcePaths})

# if(BUILD_SHARED_LIBS)
# add_library(neko SHARED ${libSourcePaths})
# else()
# add_library(neko STATIC ${libSourcePaths})
# endif()
target_include_directories(neko PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${PROJECT_SOURCE_DIR}/src
)
target_link_libraries(neko PRIVATE
    neko_utils
    neko_engine
    neko_events
    neko_renderer
    neko_threads
)
