#ifndef RTVC_COMMAND_POOL_HPP
#define RTVC_COMMAND_POOL_HPP

#include "device.hpp"

namespace rtvc {

class CommandPool : public vkw::CommandPool {
public:
  using vkw::CommandPool::CommandPool;

  CommandPool(const Device &device,
              VkCommandPoolCreateFlags flags =
                  VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT)
      : vkw::CommandPool{device.ref(), device.familyIndex(), flags} {}

private:
};

} /* namespace rtvc */
#endif /* RTVC_COMMAND_POOL_HPP */