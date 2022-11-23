#include "commands.hpp"

namespace vkh {

VkCommandBuffer
allocateCommandBuffer(VkDevice device,
                      const VkCommandBufferAllocateInfo *pAllocateInfo) {
  if (pAllocateInfo->commandBufferCount != 1) {
    throw std::runtime_error(
        "VkCommandBufferAllocateInfo::commandBufferCount != 1, use "
        "{allocateCommandBuffers} instead.");
  }
  VkCommandBuffer commandBuffer;
  if (vkAllocateCommandBuffers(device, pAllocateInfo, &commandBuffer) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffers.");
  }
  return commandBuffer;
}

std::vector<VkCommandBuffer>
allocateCommandBuffers(VkDevice device,
                       const VkCommandBufferAllocateInfo *pAllocateInfo) {
  std::vector<VkCommandBuffer> commandBuffers{
      pAllocateInfo->commandBufferCount};
  /* If the allocation of any of those command buffers fails, the implementation
  must free all successfully allocated command buffer objects from this
  command, set all entries of the pCommandBuffers array to NULL and return
  the error. */
  if (vkAllocateCommandBuffers(device, pAllocateInfo, commandBuffers.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffers.");
  }
  return commandBuffers;
}

void beginCommandBuffer(VkCommandBuffer commandBuffer,
                        const VkCommandBufferBeginInfo *pBeginInfo) {
  if (vkBeginCommandBuffer(commandBuffer, pBeginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin command buffer.");
  }
}

void endCommandBuffer(VkCommandBuffer commandBuffer) {
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("Failed to end command buffer.");
  }
}

} /* namespace vkh */