
add_library(neko_threads threads.cpp)
target_include_directories(neko_threads PUBLIC
    ${PROJECT_SOURCE_DIR}/src/utils
)
target_link_libraries(neko_threads PRIVATE
    neko_utils
)