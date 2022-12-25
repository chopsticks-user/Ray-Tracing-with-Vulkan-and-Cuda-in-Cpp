
add_library(NekoRendererGeneric OBJECTS GenericRenderer.cpp)
target_include_directories(NekoRendererGeneric
    PRIVATE ${PROJECT_SOURCE_DIR}/Renderer
)
