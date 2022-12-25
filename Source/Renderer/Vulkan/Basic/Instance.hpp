#ifndef HDG_RENDERDER_BASIC_INSTANCE_HPP
#define HDG_RENDERDER_BASIC_INSTANCE_HPP

#include "CoreInternal.hpp"

#include "Context/Context.hpp"

namespace Neko::Internal
{
    class Instance
    {
    public:
        Instance() = default;
        Instance(const Instance &) = delete;
        Instance &operator=(const Instance &) = delete;

        explicit Instance(const EngineConfigs &settings);

        Instance(Instance &&rhs) noexcept;

        Instance &operator=(Instance &&rhs) noexcept;

        ~Instance() noexcept { release(); }

        const VkInstance &operator*() const noexcept { return mInstance; }

        void release() noexcept;

        std::vector<const char *> getRequiredExtensions();

    private:
        Context mContext = {};
        VkInstance mInstance = VK_NULL_HANDLE;
        VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessengerCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            [[maybe_unused]] void *pUserData);

        void populateDebugMessengerInfo(
            VkDebugUtilsMessengerCreateInfoEXT &debugMessengerInfo);
    };

} // namespace Neko::Internal

#endif /* HDG_RENDERDER_BASIC_INSTANCE_HPP */