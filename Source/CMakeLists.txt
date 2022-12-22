
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Engine)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Events)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Renderer)

add_library(Neko INTERFACE)
target_include_directories(Neko INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Neko INTERFACE
    CompilerFlags
    NekoCore
    NekoEngine
    NekoEvents
    NekoRenderer
)
