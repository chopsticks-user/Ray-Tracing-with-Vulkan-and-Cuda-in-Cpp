#ifndef VKW_CONFIG_HPP
#define VKW_CONFIG_HPP

#ifndef CUSTOM
#define CUSTOM
#endif /* CUSTOM */

namespace vkw {

#ifdef NDEBUG
inline constexpr bool enableValidationLayers = false;
#else
inline constexpr bool enableValidationLayers = true;
#endif /* NDEBUG */

} /* namespace vkw */
#endif /* VKW_CONFIG_HPP */