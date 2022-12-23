#ifndef NEKO_INTERFACE_USER_DEFINES_HPP
#define NEKO_INTERFACE_USER_DEFINES_HPP

#include "CoreNeko.hpp"

#include "Engine/Engine.hpp"

namespace neko {

Engine *initializeEngine(const std::string &configFilePath);

} // namespace neko

#endif /* NEKO_INTERFACE_USER_DEFINES_HPP */