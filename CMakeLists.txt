cmake_minimum_required(VERSION 3.10)

project(RTVC VERSION 1.0.0 LANGUAGES C CXX)

include_directories(include)

add_library(Renderer SHARED src/Renderer.cpp)
add_library(Vertex STATIC src/Vertex.cpp)
add_library(ProxyFuncs STATIC src/ProxyFuncs.cpp)
add_library(Utility STATIC src/Utility.cpp)

add_executable(Application Application.cpp)

include_directories(src)
include_directories(resources/shaders)

target_include_directories(Renderer PRIVATE resources/shaders)
target_include_directories(Renderer PRIVATE src)
target_include_directories(Renderer PUBLIC include)

# target_include_directories(Application PUBLIC include)

target_link_libraries(Renderer ProxyFuncs)
target_link_libraries(Renderer Vertex)
target_link_libraries(Renderer Utility)
target_link_libraries(Renderer glfw)
target_link_libraries(Renderer vulkan)
target_link_libraries(Renderer dl)
target_link_libraries(Renderer pthread)
target_link_libraries(Renderer X11)
target_link_libraries(Renderer Xxf86vm)
target_link_libraries(Renderer Xrandr)
target_link_libraries(Renderer Xi)

target_link_libraries(Application Renderer)

# target_include_directories(Application )