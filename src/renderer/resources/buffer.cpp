#include "buffer.hpp"

#include "commands.hpp"
#include "logical_device.hpp"

namespace neko {

BufferObject::BufferObject(const Device &crDevice,
                           const VkBufferCreateInfo *pcBufferInfo,
                           const VkBufferViewCreateInfo *pcBufferViewInfo,
                           const VkMemoryAllocateInfo *pcAllocInfo,
                           VkDeviceSize memoryOffset) {
  mpcDevice = &crDevice;
  createBuffer(pcBufferInfo);
  createBufferMemory(pcAllocInfo, memoryOffset);
  createBufferView(pcBufferViewInfo);
  mOffset = memoryOffset;
}

BufferObject::BufferObject(BufferObject &&rhs) noexcept
    : mpcDevice{std::move(rhs.mpcDevice)}, mBuffer{std::exchange(
                                               rhs.mBuffer, VK_NULL_HANDLE)},
      mDeviceMemory{std::exchange(rhs.mDeviceMemory, VK_NULL_HANDLE)},
      mOffset{std::exchange(rhs.mOffset, false)},
      mBufferView{std::exchange(rhs.mBufferView, VK_NULL_HANDLE)} {}

BufferObject &BufferObject::operator=(BufferObject &&rhs) noexcept {
  release();
  mpcDevice = std::move(rhs.mpcDevice);
  mBuffer = std::exchange(rhs.mBuffer, VK_NULL_HANDLE);
  mDeviceMemory = std::exchange(rhs.mDeviceMemory, VK_NULL_HANDLE);
  mOffset = std::exchange(rhs.mOffset, 0);
  mBufferView = std::exchange(rhs.mBufferView, VK_NULL_HANDLE);
  return *this;
}

void BufferObject::release() noexcept {
  if (mBuffer != VK_NULL_HANDLE) {
    if (mBufferView != VK_NULL_HANDLE) {
      vkDestroyBufferView(**mpcDevice, mBufferView, nullptr);
      mBufferView = VK_NULL_HANDLE;
    }
    if (mDeviceMemory != VK_NULL_HANDLE) {
      vkFreeMemory(**mpcDevice, mDeviceMemory, nullptr);
      mDeviceMemory = VK_NULL_HANDLE;
    }
    vkDestroyBuffer(**mpcDevice, mBuffer, nullptr);
    mBuffer = VK_NULL_HANDLE;
  }
}

void BufferObject::copy(void *pHostData, VkDeviceSize size,
                        VkMemoryMapFlags flags) {
  mpcDevice->copyFromHost(pHostData, mDeviceMemory, size, mOffset, flags);
}

void BufferObject::copy(const CommandPool &commandPool,
                        const BufferObject &deviceData, VkDeviceSize size) {
  commandPool.recordBuffer([&](VkCommandBuffer commandBuffer) {
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = deviceData.mOffset;
    copyRegion.dstOffset = mOffset;
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, deviceData.buffer(), mBuffer, 1,
                    &copyRegion);
  });
}

void BufferObject::createBuffer(const VkBufferCreateInfo *pcBufferInfo) {
  if (vkCreateBuffer(**mpcDevice, pcBufferInfo, nullptr, &mBuffer) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create Buffer");
  }
}

void BufferObject::createBufferMemory(const VkMemoryAllocateInfo *pcAllocInfo,
                                      VkDeviceSize memoryOffset) {
  if (pcAllocInfo != nullptr) {
    if (vkAllocateMemory(**mpcDevice, pcAllocInfo, nullptr, &mDeviceMemory) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate Buffer memory");
    }
    if (vkBindBufferMemory(**mpcDevice, mBuffer, mDeviceMemory, memoryOffset) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to bind Buffer to memory");
    }
  }
}

void BufferObject::createBufferView(
    const VkBufferViewCreateInfo *pcBufferViewInfo) {
  if (pcBufferViewInfo != nullptr &&
      vkCreateBufferView(**mpcDevice, pcBufferViewInfo, nullptr,
                         &mBufferView) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create Buffer view");
  }
}

} /* namespace neko */