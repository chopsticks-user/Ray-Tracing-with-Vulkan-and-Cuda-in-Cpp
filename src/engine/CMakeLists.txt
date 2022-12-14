
add_library(neko_engine STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.hpp
)
target_include_directories(neko_engine PUBLIC
    ${PROJECT_SOURCE_DIR}/src/utils
    ${PROJECT_SOURCE_DIR}/src/renderer
    ${PROJECT_SOURCE_DIR}/src/threads
)

# target_link_libraries(neko_engine PRIVATE
# neko_utils
# neko_renderer
# neko_threads
# )
