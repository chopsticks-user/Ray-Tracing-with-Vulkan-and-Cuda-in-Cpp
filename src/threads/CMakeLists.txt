
add_library(neko_threads ${CMAKE_CURRENT_SOURCE_DIR}/threads.cpp)
target_include_directories(neko_threads INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(neko_threads PRIVATE neko_utils)