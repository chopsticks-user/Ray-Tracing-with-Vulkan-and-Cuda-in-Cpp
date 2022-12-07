#include "instance.hpp"

#include "renderer/basic/context/context.hpp"

#include <iostream>

namespace neko {

VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debugMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    [[maybe_unused]] void *pUserData) {
  if (messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
    std::cerr << '\n' << pCallbackData->pMessage << std::endl;
  }
  return VK_FALSE;
}

void Instance::populateDebugMessengerInfo(
    VkDebugUtilsMessengerCreateInfoEXT &debugMessengerInfo) {
  debugMessengerInfo.sType =
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  debugMessengerInfo.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      //   VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  debugMessengerInfo.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  debugMessengerInfo.pfnUserCallback = debugMessengerCallback;
  debugMessengerInfo.pUserData = nullptr;
}

std::vector<const char *> Instance::getRequiredExtensions() {
  uint32_t requiredExtensionCount;
  const char **pRequiredExtensions =
      glfwGetRequiredInstanceExtensions(&requiredExtensionCount);

  std::vector<const char *> requiredExtensions{requiredExtensionCount};
  size_t iExtension = 0;
  for (auto &extension : requiredExtensions) {
    extension = pRequiredExtensions[iExtension];
    ++iExtension;
  }
  return requiredExtensions;
}

Instance::Instance(const Settings &settings, const Context &context)
    : mcrContext{context} {
  uint32_t apiVersion;
  vkEnumerateInstanceVersion(&apiVersion);

  VkApplicationInfo applicationInfo{};
  applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  applicationInfo.pApplicationName = settings.general.appName.c_str();
  applicationInfo.applicationVersion = settings.general.appVersion;
  applicationInfo.pEngineName = settings.general.engineName.c_str();
  applicationInfo.engineVersion = settings.general.engineVersion;
  applicationInfo.apiVersion = apiVersion;

  std::vector<const char *> layers = {"VK_LAYER_MANGOHUD_overlay"};
  auto extensions = this->getRequiredExtensions();

  VkDebugUtilsMessengerCreateInfoEXT *pDebugMessengerInfo = nullptr;
  VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo{};

  if constexpr (neko::debugMode) {
    layers.emplace_back("VK_LAYER_KHRONOS_validation");
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    this->populateDebugMessengerInfo(debugMessengerInfo);
    pDebugMessengerInfo = &debugMessengerInfo;
  }

  VkInstanceCreateInfo instanceInfo{};
  instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceInfo.pNext = pDebugMessengerInfo;
  instanceInfo.pApplicationInfo = &applicationInfo;
  instanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
  instanceInfo.ppEnabledLayerNames = layers.data();
  instanceInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  instanceInfo.ppEnabledExtensionNames = extensions.data();

  if (vkCreateInstance(&instanceInfo, nullptr, &mInstance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create instance.");
  }

  if constexpr (neko::debugMode) {
    if (mcrContext.createDebugMessenger(mInstance, pDebugMessengerInfo, nullptr,
                                        &mDebugMessenger) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create debug messenger");
    }
  }
}

Instance::~Instance() {
  mcrContext.destroyDebugMessenger(mInstance, mDebugMessenger, nullptr);
  vkDestroyInstance(mInstance, nullptr);
}

} /* namespace neko */