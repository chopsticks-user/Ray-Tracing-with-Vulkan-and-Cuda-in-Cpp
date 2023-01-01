
add_library(NekoEvents
    ${CMAKE_CURRENT_SOURCE_DIR}/Events.cpp
)
target_link_libraries(NekoEvents
    PUBLIC CompilerFlags
    PRIVATE NekoCore
)