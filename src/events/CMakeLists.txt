
add_library(neko_events
    ${CMAKE_CURRENT_SOURCE_DIR}/events.cpp
)
target_link_libraries(neko_events
    PUBLIC compiler_flags
    PRIVATE neko_core
)