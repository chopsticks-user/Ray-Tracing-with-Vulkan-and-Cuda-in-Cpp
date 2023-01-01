#include "Logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#define NEKO_LOG_INTERNAL(level, ...) spdlog::get("Neko")->level(__VA_ARGS__)
#define NEKO_LOG_USER(level, ...) spdlog::get("User")->level(__VA_ARGS__)