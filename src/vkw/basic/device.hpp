#ifndef VKW_DEVICE_HPP
#define VKW_DEVICE_HPP

#include "config.hpp"

#include <map>
#include <optional>
#include <vkh.hpp>

namespace vkw {

class Device {
public:
  Device() = default;
  Device(VkInstance instance, VkSurfaceKHR surface) {
    auto wrapper = _customInitialize(instance, surface);
    _device = wrapper.device;
    _physicalDevice = wrapper.physicalDevice;
    _queue = wrapper.queue;
    _isOwner = true;
  }
  Device(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo *pCreateInfo,
         const VkAllocationCallbacks *pAllocator = nullptr)
      : _physicalDevice{physicalDevice}, _pAllocator{pAllocator} {
    _device = vkh::createDevice(physicalDevice, pCreateInfo, pAllocator);
    if (pCreateInfo->queueCreateInfoCount != 1) {
      throw std::runtime_error("pCreateInfo->queueCreateInfoCount != 1");
    }
    _queue.familyIndex = pCreateInfo->pQueueCreateInfos->queueFamilyIndex;
    vkGetDeviceQueue(_device, _queue.familyIndex, 0, &_queue.ref);
    _isOwner = true;
  };
  Device(const Device &) = delete;
  Device(Device &&rhs) { _moveDataFrom(std::move(rhs)); }
  Device &operator=(const Device &) = delete;
  Device &operator=(Device &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~Device() { _destroyVkData(); }

  const VkDevice &ref() const noexcept { return _device; }

  const VkPhysicalDevice &physical() const noexcept { return _physicalDevice; }

  VkQueue &queue() noexcept { return _queue.ref; }

  template <typename ReturnType = uint32_t> auto familyIndex() const noexcept {
    return static_cast<ReturnType>(_queue.familyIndex);
  }

  void waitIdle() { vkDeviceWaitIdle(_device); }

private:
  VkDevice _device;
  VkPhysicalDevice _physicalDevice;
  MultiPurposeQueue _queue;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Device &&rhs) {
    _destroyVkData();
    _device = rhs._device;
    _physicalDevice = rhs._physicalDevice;
    _queue = std::move(rhs._queue);
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroyDevice(_device, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Device destructor" << '\n';
      }
    }
  }

private:
  static DeviceReturnWrapper _customInitialize(VkInstance instance,
                                               VkSurfaceKHR surface) {
    static const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME};

    static const std::vector<const char *> instanceLayers = {
        "VK_LAYER_KHRONOS_validation", "VK_LAYER_MANGOHUD_overlay"};

    /* Select a suitable physical device and one of its queue families */
    auto physicalDeviceList = vkh::getPhysicalDeviceList(instance);
    VkPhysicalDevice selectedPhysDev = VK_NULL_HANDLE;
    std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
        selectedQueueFamily;
    for (const auto &physDev : physicalDeviceList) {
      /* Only selected a discrete GPU that has a queue family supporting
      graphics, computing, and presentation commands */
      /* When the application creates a swapchain, the selected physical device
      must support VK_KHR_swapchain */
      /* Also, the selected physical device must be compatible with the
       swapchain that will be created. */
      if (checkDeviceProperties(physDev) &&
          checkDeviceExtensionSupport(physDev, deviceExtensions) &&
          checkDeviceSwapchainSupport(physDev) &&
          checkDeviceFeatureSupport(physDev)) {
        if (auto returnedQueueFamily = selectQueueFamily(surface, physDev);
            returnedQueueFamily.has_value()) {
          selectedQueueFamily = returnedQueueFamily;
          selectedPhysDev = physDev;
        }
      }
    }
    if (selectedPhysDev == VK_NULL_HANDLE) {
      throw std::runtime_error("Failed to select a physical device.");
    }

    /* Set up the selected queue family's creation info */
    uint32_t selectedIndex = selectedQueueFamily.value().first;
    const float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = nullptr;
    queueInfo.queueFamilyIndex = selectedIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &queuePriority;

    /* Create the logical device */
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueInfo;
    deviceCreateInfo.enabledExtensionCount =
        static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledLayerCount =
        static_cast<uint32_t>(instanceLayers.size());
    deviceCreateInfo.ppEnabledLayerNames = instanceLayers.data();

    auto device = vkh::createDevice(selectedPhysDev, &deviceCreateInfo);
    VkQueue queueHandle;
    vkGetDeviceQueue(device, selectedIndex, 0, &queueHandle);
    return {device, selectedPhysDev, {queueHandle, selectedIndex}};
  }

  static bool checkDeviceProperties(VkPhysicalDevice physDev) {
    auto physDevPropList = vkh::getPhysicalDevicePropertyList(physDev);
    return physDevPropList.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
  }

  static bool checkDeviceExtensionSupport(
      VkPhysicalDevice physDev,
      const std::vector<const char *> &deviceExtensions) {
    auto availableExtensionsList =
        vkh::getAvailableDeviceExtensionList(physDev);
    std::map<std::string, uint32_t> helper;
    for (const auto &availableExtension : availableExtensionsList) {
      helper[availableExtension.extensionName]++;
      //  std::cout << availableExtension.extensionName << '\n';
    }
    for (const auto &deviceExtension : deviceExtensions) {
      helper[deviceExtension]++;
      if (helper[deviceExtension] < 2) {
        return false;
      }
    }
    return true;
  }

  static bool
  checkDeviceSwapchainSupport([[maybe_unused]] VkPhysicalDevice physDev) {
    return true;
  }

  static bool checkDeviceFeatureSupport(VkPhysicalDevice physDev) {
    VkPhysicalDeviceFeatures supportedFeatures{};
    vkGetPhysicalDeviceFeatures(physDev, &supportedFeatures);
    return supportedFeatures.samplerAnisotropy;
  }

  static std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
  selectQueueFamily(VkSurfaceKHR surface, VkPhysicalDevice physDev) {
    auto graphicsFamilies = vkh::getGraphicsQueueFamilyPropertyList(physDev);
    auto computeFamilies = vkh::getComputeQueueFamilyPropertyList(physDev);
    auto presentFamilies =
        vkh::getPresentQueueFamilyPropertyList(physDev, surface);

    /* Find a queue that is capable of handling graphics, computing, and
    supporting presentation to the surface */
    /* any queue family with {VK_QUEUE_GRAPHICS_BIT} or {VK_QUEUE_COMPUTE_BIT}
    capabilities already implicitly support {VK_QUEUE_TRANSFER_BIT} operations
  */
    for (size_t i = 0; i < graphicsFamilies.size(); ++i) {
      if ((graphicsFamilies[i].has_value() && computeFamilies[i].has_value()) &&
          presentFamilies[i].has_value()) {
        return {{static_cast<uint32_t>(i), graphicsFamilies[i].value()}};
      }
    }
    return {};
  }
};

} /* namespace vkw */
#endif /* VKW_DEVICE_HPP */