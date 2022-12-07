#include "logical.hpp"

#include "renderer/basic/instance/instance.hpp"
#include "renderer/devices/physical/physical.hpp"

#include <map>
#include <optional>

namespace neko {

LogicalDevice::LogicalDevice(const Instance &crInstance) {
  PhysicalDevices physicalDevices{crInstance};
}

} /* namespace neko */