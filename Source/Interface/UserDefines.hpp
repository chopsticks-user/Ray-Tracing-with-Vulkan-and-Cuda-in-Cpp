#ifndef NEKO_INTERFACE_USER_DEFINES_HPP
#define NEKO_INTERFACE_USER_DEFINES_HPP

#define NEKO_INITIALIZE_ENGINE_AT_ENTRY_POINT(entryPoint) \
                                                          \
    neko::Engine *neko::_nekoUserInitializeEngine()       \
    {                                                     \
        return new neko::Engine{};                        \
    }

#define NEKO_SET_USER_PROJECT_ENTRY(entryPoint) \
    NEKO_INITIALIZE_ENGINE_AT_ENTRY_POINT(entryPoint)

#endif /* NEKO_INTERFACE_USER_DEFINES_HPP */