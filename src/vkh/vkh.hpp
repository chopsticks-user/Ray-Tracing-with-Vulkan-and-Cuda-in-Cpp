#ifndef VKH_HPP
#define VKH_HPP

#include "commands.hpp"
#include "create_object.hpp"
#include "destroy_object.hpp"
#include "get_list.hpp"
#include "structures.hpp"
#include "utility.hpp"
#include "validation_layers.hpp"
#include "wrapper.hpp"

#include <filesystem>

namespace vkh {

inline const std::string absoluteDirectory =
    std::filesystem::current_path().string();

} /* namespace vkh */

#endif /* VKH_HPP */
