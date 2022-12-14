
add_library(neko_threads STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/threads.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/threads.hpp
)
target_include_directories(neko_threads PUBLIC
    ${PROJECT_SOURCE_DIR}/src/utils
)
target_link_libraries(neko_threads PRIVATE
    neko_utils
)