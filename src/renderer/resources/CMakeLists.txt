
add_library(neko_renderer_resources
    ${CMAKE_CURRENT_SOURCE_DIR}/image.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/usage_specific.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
)
target_include_directories(neko_renderer_resources
    PRIVATE ${PROJECT_SOURCE_DIR}/src/renderer/commands
    PRIVATE ${PROJECT_SOURCE_DIR}/src/renderer/devices
    PRIVATE ${PROJECT_SOURCE_DIR}/src/renderer/basic
)
target_link_libraries(neko_renderer_resources
    PUBLIC compiler_flags
    PRIVATE neko_core
)
