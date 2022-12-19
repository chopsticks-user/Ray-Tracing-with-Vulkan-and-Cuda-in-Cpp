#ifndef NEKO_RENDERER_COMMANDS_HPP
#define NEKO_RENDERER_COMMANDS_HPP

#include "core.hpp"

namespace neko {

class Device;

class CommandPool {
public:
  enum BufferLevel {
    primary = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    secondary = VK_COMMAND_BUFFER_LEVEL_SECONDARY,
  };

  enum BufferUsage {
    oneTimeSubmit = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    renderPassCountinue = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT,
    simultaneousUse = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
  };

  CommandPool() = default;
  CommandPool(const CommandPool &) = delete;
  CommandPool &operator=(const CommandPool &) = delete;

  CommandPool(const Device &crDevice,
              VkCommandPoolCreateFlags flags =
                  VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

  CommandPool(CommandPool &&rhs) noexcept;

  CommandPool &operator=(CommandPool &&rhs) noexcept;

  ~CommandPool() noexcept { release(); }

  const VkCommandPool &operator*() const noexcept { return mCommandPool; }

  void release() noexcept;

  u32 queueFamilyIndex() const noexcept;

  const VkQueue &queue() const noexcept;

  VkCommandBuffer alloc(BufferLevel level = primary) const;

  std::vector<VkCommandBuffer> alloc(u32 count,
                                     BufferLevel level = primary) const;

  void beginBuffer(VkCommandBuffer commandBuffer,
                   BufferUsage usageFlags = oneTimeSubmit) const;

  void recordBuffer(void (*recordFunc)(VkCommandBuffer),
                    BufferLevel level = primary,
                    VkFence fence = VK_NULL_HANDLE) const;

  void recordBuffer(void (*recordFunc)(VkCommandBuffer),
                    const VkSubmitInfo *pcSubmitInfo,
                    BufferLevel level = primary,
                    VkFence fence = VK_NULL_HANDLE) const;

  void endBuffer(VkCommandBuffer commandBuffer) const;

  void submit(const VkSubmitInfo *pcSubmitInfo,
              VkFence fence = VK_NULL_HANDLE) const;

  void submit(VkCommandBuffer commandBuffer,
              VkFence fence = VK_NULL_HANDLE) const;

  void free(VkCommandBuffer commandBuffer) const noexcept;

  void free(std::vector<VkCommandBuffer> commandBuffers) const noexcept;

private:
  const Device *mpcDevice = nullptr;
  VkCommandPool mCommandPool = VK_NULL_HANDLE;

  VkCommandBufferAllocateInfo makeAllocInfo(u32 count = 1,
                                            BufferLevel level = primary) const;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_COMMANDS_HPP */