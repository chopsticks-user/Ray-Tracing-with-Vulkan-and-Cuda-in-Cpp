cmake_minimum_required(VERSION 3.15)

project(NekoEngine VERSION 1.0.0 LANGUAGES CXX)

include(Tools/CMake/CompilerFlags.cmake)

configure_file(${PROJECT_SOURCE_DIR}/Tools/CMake/NekoEngine.h.in
    ${PROJECT_SOURCE_DIR}/Source/Core/CMakeConfigs.hpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/Source)

add_executable(Application ${PROJECT_SOURCE_DIR}/Application.cpp)
target_link_libraries(Application
    PUBLIC CompilerFlags
    PRIVATE Neko
)

# include(${PROJECT_SOURCE_DIR}/tools/cmake/Install.cmake)
