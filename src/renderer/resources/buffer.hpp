#ifndef NEKO_RENDERER_RESOURCES_BUFFER_HPP
#define NEKO_RENDERER_RESOURCES_BUFFER_HPP

#include "core.hpp"

namespace neko {

class Device;
class CommandPool;

class Buffer {
public:
  Buffer() = default;
  Buffer(const Buffer &) = delete;
  Buffer &operator=(const Buffer &) = delete;

private:
};

class BufferView {
public:
  BufferView() = default;
  BufferView(const BufferView &) = delete;
  BufferView &operator=(const BufferView &) = delete;

private:
};

class BufferObject {
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

  virtual ~BufferObject() noexcept { release(); }

  const VkBuffer &buffer() const noexcept { return mBuffer; }

  const VkBufferView &view() const noexcept { return mBufferView; }

  void copy(void *pHostData, VkDeviceSize size, VkMemoryMapFlags flags = 0);

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

} /* namespace neko */

#endif /* NEKO_RENDERER_RESOURCES_BUFFER_HPP */