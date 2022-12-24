# ------------------------ GLFW ---------------------------------------#
#
add_library(GLFW INTERFACE)
target_include_directories(GLFW INTERFACE)
target_link_libraries(GLFW INTERFACE glfw)

# ------------------------ Vulkan SDK ---------------------------------#
#
add_library(Vulkan INTERFACE)
target_include_directories(Vulkan INTERFACE)
target_link_libraries(Vulkan INTERFACE vulkan)

# ------------------------ Nlohmann Json ------------------------------#
#
add_library(NlohmannJson INTERFACE)
target_include_directories(NlohmannJson INTERFACE
    ${PROJECT_SOURCE_DIR}/Modules/Json/single_include
)
target_link_libraries(NlohmannJson INTERFACE)

# ------------------------ Stb Image ----------------------------------#
#
add_library(StbImage INTERFACE)
target_include_directories(StbImage INTERFACE
    ${PROJECT_SOURCE_DIR}/Modules/Stb
)
target_link_libraries(StbImage INTERFACE)

# ---------------------------------------------------------------------#
