#include "Instance.hpp"

#include <iostream>

namespace Neko::Internal::VK
{

    VKAPI_ATTR VkBool32 VKAPI_CALL Instance::debugMessengerCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        [[maybe_unused]] void *pUserData)
    {
        if (messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        {
            std::cerr << '\n'
                      << pCallbackData->pMessage << std::endl;
        }
        return VK_FALSE;
    }

    void Instance::populateDebugMessengerInfo(
        VkDebugUtilsMessengerCreateInfoEXT &debugMessengerInfo)
    {
        debugMessengerInfo.sType =
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugMessengerInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugMessengerInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugMessengerInfo.pfnUserCallback = debugMessengerCallback;
        debugMessengerInfo.pUserData = nullptr;
    }

    std::vector<const char *> Instance::getRequiredExtensions()
    {
        uint32_t requiredExtensionCount;
        const char **pRequiredExtensions =
            glfwGetRequiredInstanceExtensions(&requiredExtensionCount);

        std::vector<const char *> requiredExtensions{requiredExtensionCount};
        size_t iExtension = 0;
        for (auto &extension : requiredExtensions)
        {
            extension = pRequiredExtensions[iExtension];
            ++iExtension;
        }
        return requiredExtensions;
    }

    Instance::Instance(const Core::EngineConfigs &configs)
    {
        uint32_t apiVersion;
        vkEnumerateInstanceVersion(&apiVersion);

        VkApplicationInfo applicationInfo{};
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pApplicationName = configs.general.appName.c_str();
        applicationInfo.applicationVersion = VK_MAKE_VERSION(configs.general.appVersion.major,
                                                             configs.general.appVersion.minor,
                                                             configs.general.appVersion.patch);
        applicationInfo.pEngineName = configs.general.engineName.c_str();
        applicationInfo.engineVersion = VK_MAKE_VERSION(configs.general.engineVersion.major,
                                                        configs.general.engineVersion.minor,
                                                        configs.general.engineVersion.patch);
        applicationInfo.apiVersion = apiVersion;

        std::vector<const char *> layers{};

        // ! MangoHud on
        if constexpr (Core::platform == Core::Enum::Platform::linuxk)
        {
            layers.emplace_back("VK_LAYER_MANGOHUD_overlay");
        }

        auto extensions = getRequiredExtensions();

        VkDebugUtilsMessengerCreateInfoEXT *pDebugMessengerInfo = nullptr;
        VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo{};

        if constexpr (Core::System::debugMode)
        {
            layers.emplace_back("VK_LAYER_KHRONOS_validation");
            extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

            populateDebugMessengerInfo(debugMessengerInfo);
            pDebugMessengerInfo = &debugMessengerInfo;
        }

        VkInstanceCreateInfo instanceInfo{};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pNext = pDebugMessengerInfo;
        instanceInfo.pApplicationInfo = &applicationInfo;
        instanceInfo.enabledLayerCount = vku32(layers.size());
        instanceInfo.ppEnabledLayerNames = layers.data();
        instanceInfo.enabledExtensionCount = vku32(extensions.size());
        instanceInfo.ppEnabledExtensionNames = extensions.data();

        if (vkCreateInstance(&instanceInfo, nullptr, &mInstance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create instance.");
        }

        if constexpr (Core::System::debugMode)
        {
            if (mContext.createDebugMessenger(mInstance, pDebugMessengerInfo, nullptr,
                                              &mDebugMessenger) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create debug messenger");
            }
        }
    }

    Instance::Instance(Instance &&rhs) noexcept
        : mContext{std::move(rhs.mContext)}, mInstance{std::move(rhs.mInstance)},
          mDebugMessenger{std::move(rhs.mDebugMessenger)} {}

    Instance &Instance::operator=(Instance &&rhs) noexcept
    {
        release();
        mContext = std::move(rhs.mContext);
        mInstance = std::exchange(rhs.mInstance, VK_NULL_HANDLE);
        mDebugMessenger = std::exchange(rhs.mDebugMessenger, VK_NULL_HANDLE);
        return *this;
    }

    void Instance::release() noexcept
    {
        if (mInstance != VK_NULL_HANDLE)
        {
            if constexpr (Core::System::debugMode)
            {
                mContext.destroyDebugMessenger(mInstance, mDebugMessenger, nullptr);
            }
            vkDestroyInstance(mInstance, nullptr);
            mInstance = VK_NULL_HANDLE;
        }
    }

} // namespace Neko::Internal::VK