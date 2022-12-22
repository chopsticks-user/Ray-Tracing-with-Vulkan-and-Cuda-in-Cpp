#include "commands.hpp"

#include "logical_device.hpp"

namespace neko {

CommandPool::CommandPool(const Device &crDevice,
                         VkCommandPoolCreateFlags flags) {
  flags |= VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = flags;
  poolInfo.queueFamilyIndex = crDevice.queue().mFamilyIndex;

  if (vkCreateCommandPool(*crDevice, &poolInfo, nullptr, &mCommandPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create command pool.");
  }
  mpcDevice = &crDevice;
}

CommandPool::CommandPool(CommandPool &&rhs) noexcept
    : mpcDevice{std::move(rhs.mpcDevice)},
      mCommandPool{std::exchange(rhs.mCommandPool, VK_NULL_HANDLE)} {}

CommandPool &CommandPool::operator=(CommandPool &&rhs) noexcept {
  release();
  mpcDevice = std::move(rhs.mpcDevice);
  mCommandPool = std::exchange(rhs.mCommandPool, VK_NULL_HANDLE);
  return *this;
}

void CommandPool::release() noexcept {
  if (mCommandPool != VK_NULL_HANDLE) {
    vkDestroyCommandPool(**mpcDevice, mCommandPool, nullptr);
    mCommandPool = VK_NULL_HANDLE;
  }
}

u32 CommandPool::queueFamilyIndex() const noexcept {
  return mpcDevice->queue().mFamilyIndex;
}

const VkQueue &CommandPool::queue() const noexcept {
  return mpcDevice->queue().mQueue;
}

VkCommandBuffer CommandPool::alloc(BufferLevel level) const {
  auto allocInfo = makeAllocInfo(1, level);
  VkCommandBuffer commandBuffer;
  if (vkAllocateCommandBuffers(**mpcDevice, &allocInfo, &commandBuffer) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate a command buffer.");
  }
  return commandBuffer;
}

std::vector<VkCommandBuffer> CommandPool::alloc(u32 count,
                                                BufferLevel level) const {
  auto allocInfo = makeAllocInfo(count, level);
  std::vector<VkCommandBuffer> commandBuffers{count};
  if (vkAllocateCommandBuffers(**mpcDevice, &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffers.");
  }
  return commandBuffers;
}

void CommandPool::beginBuffer(VkCommandBuffer commandBuffer,
                              BufferUsage usageFlags) const {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.pNext = nullptr;
  beginInfo.pInheritanceInfo = nullptr;
  beginInfo.flags = static_cast<VkCommandBufferUsageFlags>(usageFlags);

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin recording a command buffer.");
  }
}

void CommandPool::recordBuffer(std::function<void(VkCommandBuffer)> recordFunc,
                               BufferLevel level, VkFence fence) const {
  auto commandBuffer = alloc(level);
  beginBuffer(commandBuffer);
  recordFunc(commandBuffer);
  endBuffer(commandBuffer);
  submit(commandBuffer, fence);
  free(commandBuffer);
}

void CommandPool::recordBuffer(std::function<void(VkCommandBuffer)> recordFunc,
                               const VkSubmitInfo *pcSubmitInfo,
                               BufferLevel level, VkFence fence) const {
  auto commandBuffer = alloc(level);
  beginBuffer(commandBuffer);
  recordFunc(commandBuffer);
  endBuffer(commandBuffer);
  submit(pcSubmitInfo, fence);
  free(commandBuffer);
}

void CommandPool::endBuffer(VkCommandBuffer commandBuffer) const {
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("Failed to end recording a command buffer.");
  }
}

void CommandPool::submit(const VkSubmitInfo *pcSubmitInfo,
                         VkFence fence) const {
  auto queueHandle = queue();
  if (vkQueueSubmit(queueHandle, 1, pcSubmitInfo, fence) != VK_SUCCESS) {
    throw std::runtime_error("Failed to submit a command to queue.");
  }
  if (fence == VK_NULL_HANDLE) {
    vkQueueWaitIdle(queueHandle);
  }
}

void CommandPool::submit(VkCommandBuffer commandBuffer, VkFence fence) const {
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  auto queueHandle = queue();
  if (vkQueueSubmit(queueHandle, 1, &submitInfo, fence) != VK_SUCCESS) {
    throw std::runtime_error("Failed to submit a command to queue.");
  }
  if (fence == VK_NULL_HANDLE) {
    vkQueueWaitIdle(queueHandle);
  } else {
  }
}

void CommandPool::free(VkCommandBuffer commandBuffer) const noexcept {
  vkFreeCommandBuffers(**mpcDevice, mCommandPool, 1, &commandBuffer);
}

void CommandPool::free(
    std::vector<VkCommandBuffer> commandBuffers) const noexcept {
  vkFreeCommandBuffers(**mpcDevice, mCommandPool, vku32(commandBuffers.size()),
                       commandBuffers.data());
}

VkCommandBufferAllocateInfo
CommandPool::makeAllocInfo(u32 count, BufferLevel level) const {
  if (count == 0) {
    throw std::runtime_error("count == 0");
  }
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.pNext = nullptr;
  allocInfo.commandPool = mCommandPool;
  allocInfo.level = static_cast<VkCommandBufferLevel>(level);
  allocInfo.commandBufferCount = count;
  return allocInfo;
}

} /* namespace neko */