#include "commands.hpp"

namespace vkh {

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

} /* namespace vkh */