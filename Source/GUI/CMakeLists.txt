
add_library(NekoGUI ${CMAKE_CURRENT_SOURCE_DIR}/GUI.cpp)
target_include_directories(NekoGUI
    INTERFACE ${PROJECT_SOURCE_DIR}/Source/GUI
)
target_link_libraries(NekoGUI
    PRIVATE CompilerFlags
    PRIVATE NekoCore
    PRIVATE ImGUI
)