#ifndef HDG_RENDERER_VULKAN_RESOURCES_BUFFER_HPP
#define HDG_RENDERER_VULKAN_RESOURCES_BUFFER_HPP

#include "CoreInternal.hpp"
#include "Basic/Context.hpp"

namespace Neko::Internal::VK
{

    class Device;
    class CommandPool;

    class Buffer
    {
    public:
        Buffer() = default;
        Buffer(const Buffer &) = delete;
        Buffer &operator=(const Buffer &) = delete;

    private:
    };

    class BufferView
    {
    public:
        BufferView() = default;
        BufferView(const BufferView &) = delete;
        BufferView &operator=(const BufferView &) = delete;

    private:
    };

    class BufferObject
    {
    public:
        BufferObject() = default;
        BufferObject(const BufferObject &) = delete;
        BufferObject &operator=(const BufferObject &) = delete;

        BufferObject(const Device &crDevice, const VkBufferCreateInfo *pcBufferInfo,
                     const VkBufferViewCreateInfo *pcBufferViewInfo = nullptr,
                     const VkMemoryAllocateInfo *pcAllocInfo = nullptr,
                     VkDeviceSize memoryOffset = 0);

        BufferObject(const Device &crDevice, VkDeviceSize size,
                     VkBufferUsageFlags usage, VkMemoryPropertyFlags propertyFlags,
                     VkDeviceSize memoryOffset = 0);

        BufferObject(BufferObject &&rhs) noexcept;

        BufferObject &operator=(BufferObject &&rhs) noexcept;

        const VkBuffer &operator*() const noexcept { return mBuffer; }

        virtual ~BufferObject() noexcept { release(); }

        const VkBufferView &view() const noexcept { return mBufferView; }

        void copy(const void *pHostData, VkDeviceSize size,
                  VkMemoryMapFlags flags = 0);

        void copy(const CommandPool &commandPool, const BufferObject &deviceData,
                  VkDeviceSize size);

        void release() noexcept;

    protected:
        const Device *mpcDevice = nullptr;
        VkBuffer mBuffer = VK_NULL_HANDLE;
        VkDeviceMemory mDeviceMemory = VK_NULL_HANDLE;
        VkDeviceSize mOffset = 0;
        VkBufferView mBufferView = VK_NULL_HANDLE;

        void createBuffer(const VkBufferCreateInfo *pcBufferInfo);

        void createBufferMemory(const VkMemoryAllocateInfo *pcAllocInfo,
                                VkDeviceSize memoryOffset = 0);

        void createBufferView(const VkBufferViewCreateInfo *pcBufferViewInfo);
    };

} // namespace Neko::Internal::VK

#endif /* HDG_RENDERER_VULKAN_RESOURCES_BUFFER_HPP */