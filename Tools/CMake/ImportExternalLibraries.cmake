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

# ------------------------ Spd Log ------------------------------------#
#
add_library(SpdLog INTERFACE)
target_include_directories(SpdLog INTERFACE
    ${PROJECT_SOURCE_DIR}/Modules/Spdlog/include
)
target_link_libraries(SpdLog INTERFACE)

# ------------------------ Tiny Object Image --------------------------#
#
add_library(TinyObjLoader INTERFACE)
target_include_directories(TinyObjLoader INTERFACE
    ${PROJECT_SOURCE_DIR}/Modules/TinyObjLoader
)
target_link_libraries(TinyObjLoader INTERFACE)

# ------------------------ ImGUI --------------------------------------#
#
add_library(ImGUI
    ${PROJECT_SOURCE_DIR}/Modules/ImGUI/imgui.cpp
    ${PROJECT_SOURCE_DIR}/Modules/ImGUI/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/Modules/ImGUI/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/Modules/ImGUI/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/Modules/ImGUI/imgui_widgets.cpp
)
target_include_directories(ImGUI INTERFACE
    ${PROJECT_SOURCE_DIR}/Modules/ImGUI
)
target_link_libraries(ImGUI INTERFACE)

# ---------------------------------------------------------------------#
