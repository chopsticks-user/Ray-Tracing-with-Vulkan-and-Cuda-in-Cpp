#ifndef VKH_HPP
#define VKH_HPP

#include "commands/commands.hpp"
#include "layers/validation_layers.hpp"
#include "lists/get_list.hpp"
#include "utils/utility.hpp"
#include "vk_objects/create_object.hpp"
#include "vk_objects/destroy_object.hpp"

#include <filesystem>

namespace vkh {

inline const std::string absoluteDirectory =
    std::filesystem::current_path().string();

} /* namespace vkh */

#endif /* VKH_HPP */
