#ifndef VKH_COMMAND_BUFFERS_HPP
#define VKH_COMMAND_BUFFERS_HPP

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif /* GLFW_INCLUDE_VULKAN */

#include <stdexcept>
#include <vector>

namespace vkh {
/**
 * @brief
 *
 * @param device
 * @param pAllocateInfo
 * @return std::vector<VkCommandBuffer>
 */
std::vector<VkCommandBuffer>
allocateCommandBuffers(VkDevice device,
                       const VkCommandBufferAllocateInfo *pAllocateInfo);

} /* namespace vkh */

#endif /* VKH_COMMAND_BUFFERS_HPP */