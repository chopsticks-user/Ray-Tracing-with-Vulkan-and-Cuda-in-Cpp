
add_library(neko_engine
    ${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp
)
target_include_directories(neko_engine PRIVATE
    ${PROJECT_SOURCE_DIR}/src/renderer
)
target_link_libraries(neko_engine PRIVATE
    neko_utils
    neko_threads
)