cmake_minimum_required(VERSION 3.15)

project(NekoEngine VERSION 1.0.0 LANGUAGES CXX)

add_definitions(-g -Wall -Wextra -pedantic -Wextra -Wshadow -Wconversion)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# configure_file(config.h.in, config.h)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_executable(Application ${PROJECT_SOURCE_DIR}/Application.cpp)
target_link_libraries(Application PRIVATE neko)
