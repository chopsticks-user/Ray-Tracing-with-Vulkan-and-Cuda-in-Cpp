
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/utils)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/events)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/renderer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/threads)

add_library(neko INTERFACE)
target_include_directories(neko INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(neko INTERFACE
    compiler_flags
    neko_utils
    neko_engine
    neko_events
    neko_renderer
    neko_threads
)

# add_library(neko neko.cpp)
# target_include_directories(neko INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(neko INTERFACE
# compiler_flags
# neko_utils
# neko_engine
# neko_events
# neko_renderer
# neko_threads
# )
