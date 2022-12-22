
add_library(NekoEngine
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp
)
target_include_directories(NekoEngine PRIVATE
    ${PROJECT_SOURCE_DIR}/Source/Renderer
)
target_link_libraries(NekoEngine
    PUBLIC CompilerFlags
    PRIVATE NekoCore
)