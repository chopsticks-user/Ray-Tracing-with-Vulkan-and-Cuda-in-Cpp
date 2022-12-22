
set(installable_libs neko compiler_flags)

install(TARGETS
    neko_engine
    neko_events
    neko_renderer
    neko_threads
    neko_utils
    DESTINATION ${PROJECT_SOURCE_DIR}/install/lib
)

install(FILES
    ${PROJECT_SOURCE_DIR}/src/neko.hpp
    DESTINATION ${PROJECT_SOURCE_DIR}/install/include
)

install(FILES
    ${PROJECT_SOURCE_DIR}/src/engine/engine.hpp
    ${PROJECT_SOURCE_DIR}/src/events/events.hpp
    ${PROJECT_SOURCE_DIR}/src/renderer/renderer.hpp
    ${PROJECT_SOURCE_DIR}/src/threads/threads.hpp
    ${PROJECT_SOURCE_DIR}/src/utils/utils.hpp
    DESTINATION ${PROJECT_SOURCE_DIR}/install/include/neko
)

install(TARGETS
    Application
    DESTINATION ${PROJECT_SOURCE_DIR}/install/bin
)

install(FILES
    ${PROJECT_SOURCE_DIR}/data/configs/MangoHud.conf
    DESTINATION ${PROJECT_SOURCE_DIR}/install/bin
)

install(FILES
    ${PROJECT_SOURCE_DIR}/data/configs/settings.json
    DESTINATION ${PROJECT_SOURCE_DIR}/install/bin/data/configs
)
