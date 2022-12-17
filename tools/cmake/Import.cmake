
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/src/platforms/windows/libs/GLFW/glfw.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/src/platforms/indep/include"
)

add_library(vulkan STATIC IMPORTED)
set_target_properties(vulkan PROPERTIES
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/src/platforms/windows/libs/vulkan/vulkan.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/src/platforms/indep/include"
)