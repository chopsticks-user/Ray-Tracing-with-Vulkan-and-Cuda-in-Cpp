#ifndef VKC_HPP
#define VKC_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <type_traits>

#ifndef add_template_overload
#define add_template_overload(return_type, args_type)                          \
  template <typename T, typename... args_type>                                 \
  std::enable_if_t<std::is_same_v<T, return_type>, T>
#endif /* add_template_overload */

namespace vkc {
namespace detail {

std::fstream createFileStream(std::string &&filePath) {
  std::fstream fs(filePath);
  if (!fs.is_open()) {
    throw std::runtime_error("Failed to open " + filePath);
  }
  return fs;
}

nlohmann::json getParsedJsonData(std::string &&filePath) {
  auto fs = createFileStream(std::move(filePath));
  return nlohmann::json::parse(fs);
}

} /* namespace detail */

add_template_overload(VkApplicationInfo, ArgTypes)
    parseJsonInfo(std::string filePath, ArgTypes... args) {
  auto data = detail::getParsedJsonData(std::move(filePath));
  return {};
}

add_template_overload(VkInstanceCreateInfo, ArgTypes)
    parseJsonInfo(std::string filePath, ArgTypes... args) {
  auto data = detail::getParsedJsonData(std::move(filePath));
  return {};
}

} /* namespace vkc */

#endif /* VKC_HPP */