cmake_minimum_required(VERSION 3.15)

project(NekoEngine VERSION 1.0.0 LANGUAGES CXX)

# configure_file(config.h.in, config.h)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(BUILD_SHARED_LIBS ON)

add_subdirectory(src)

add_executable(Application
    ${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp
)
target_include_directories(Application PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(Application PRIVATE neko)
