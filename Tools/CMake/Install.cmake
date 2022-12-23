
set(InstallableLibs "Neko;CompilerFlags")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Libraries)

option(CMAKE_SKIP_RPATH ON)

install(
    FILES
    ${NekoEngineDirectory}/Build/Source/Core/libNekoCore.so
    ${NekoEngineDirectory}/Build/Source/Engine/libNekoEngine.so
    ${NekoEngineDirectory}/Build/Source/Events/libNekoEvents.so
    ${NekoEngineDirectory}/Build/Source/Renderer/libNekoRenderer.so

    DESTINATION ${PROJECT_SOURCE_DIR}/Libraries
)
