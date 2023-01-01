#ifndef HDG_INTERFACE_USERDEFINES_HPP
#define HDG_INTERFACE_USERDEFINES_HPP

#define NEKO_INITIALIZE_ENGINE_AT_ENTRY_POINT(entryPoint)               \
                                                                        \
    Neko::Internal::Engine *Neko::Internal::_nekoUserInitializeEngine() \
    {                                                                   \
        return new Neko::Internal::Engine{};                            \
    }

#define NEKO_SET_USER_PROJECT_ENTRY(entryPoint) \
    NEKO_INITIALIZE_ENGINE_AT_ENTRY_POINT(entryPoint)

#endif /* HDG_INTERFACE_USERDEFINES_HPP */
