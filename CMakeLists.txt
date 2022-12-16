cmake_minimum_required(VERSION 3.15)

project(NekoEngine VERSION 1.0.0 LANGUAGES CXX)

include(tools/cmake/CompilerFlags.cmake)

configure_file(${PROJECT_SOURCE_DIR}/tools/cmake/NekoEngine.h.in
    ${PROJECT_SOURCE_DIR}/src/utils/configs.hpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_executable(Application ${PROJECT_SOURCE_DIR}/Application.cpp)
target_link_libraries(Application
    PUBLIC compiler_flags
    PRIVATE neko
)
message(${CMAKE_BUILD_TYPE})

# include(${PROJECT_SOURCE_DIR}/tools/cmake/Install.cmake)
