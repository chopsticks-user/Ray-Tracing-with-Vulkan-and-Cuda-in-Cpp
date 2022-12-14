
add_library(neko_renderer_basic OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/instance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/surface.cpp
)
target_include_directories(neko_renderer_basic PUBLIC
    ${PROJECT_SOURCE_DIR}/src/utils
)

# target_link_libraries(neko_engine PRIVATE
# neko_utils
# )