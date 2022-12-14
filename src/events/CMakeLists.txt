
add_library(neko_events STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/events.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/events.hpp
)

# target_include_directories(neko_events
# PRIVATE ${PROJECT_SOURCE_DIR}/src/utils
# PRIVATE ${PROJECT_SOURCE_DIR}/src/renderer
# PRIVATE ${PROJECT_SOURCE_DIR}/src/threads
# )

# target_link_libraries(neko_engine PRIVATE
# )