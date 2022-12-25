#ifndef HDG_RENDERER_DEVICES_LOGICAL_HPP
#define HDG_RENDERER_DEVICES_LOGICAL_HPP

#include "CoreInternal.hpp"

namespace Neko::Internal
{

    class Instance;
    class Surface;

    /**
     * @brief
     *
     * TODO: Temporary implementation, the members should not be modified
     *
     */
    struct UniversalQueue
    {
        uint32_t mFamilyIndex;
        uint32_t mIndex;
        VkQueue mQueue;
    };

    /**
     * @brief
     *
     * !Use only one universal queue
     */
    class Device
    {
    public:
        Device() = default;
        Device(const Device &) = delete;
        Device &operator=(const Device &) = delete;

        Device(const Instance &crInstance, const Surface &crSurface);

        Device(Device &&) noexcept;

        Device &operator=(Device &&) noexcept;

        ~Device() noexcept { release(); }

        VkDevice operator*() const noexcept { return mLogicalDevice; }

        operator VkDevice() const noexcept { return mLogicalDevice; }

        const VkPhysicalDevice &physical() const noexcept { return mPhysicalDevice; }

        const UniversalQueue &queue() const noexcept { return mQueue; }

        void copyFromHost(const void *pHostData, VkDeviceMemory memory,
                          VkDeviceSize size, VkDeviceSize offset = 0,
                          VkMemoryMapFlags flags = 0) const;

        void waitIdle() const noexcept { vkDeviceWaitIdle(mLogicalDevice); }

        void release() noexcept;

    private:
        VkDevice mLogicalDevice = nullptr;
        VkPhysicalDevice mPhysicalDevice = nullptr;
        UniversalQueue mQueue = {};

        uint32_t selectUniversalQueueFamily(VkPhysicalDevice physicalDevice,
                                            VkSurfaceKHR surface);

        [[nodiscard]] VkPhysicalDevice
        selectPhysicalDevice(std::vector<VkPhysicalDevice> &&rrPhysicalDevices);

        bool checkRequirements(VkPhysicalDevice physicalDevice);

        bool checkProperties(VkPhysicalDevice physicalDevice);

        bool checkExtensions(VkPhysicalDevice physicalDevice);

        bool checkFeatures(VkPhysicalDevice physicalDevice);

        std::vector<VkPhysicalDevice> getPhysicalDevices(VkInstance instance);
    };

} // namespace Neko::Internal

#endif /* HDG_RENDERER_DEVICES_LOGICAL_HPP */