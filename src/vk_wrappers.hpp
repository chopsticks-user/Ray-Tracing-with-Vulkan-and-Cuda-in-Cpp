#ifndef VK_WRAPPERS_HPP
#define VK_WRAPPERS_HPP

#include <vkh.hpp>

#include <map>
#include <optional>

#ifndef CUSTOM
#define CUSTOM
#endif /* CUSTOM */

namespace vkw {

#ifdef NDEBUG
inline constexpr bool enableValidationLayers = false;
#else
inline constexpr bool enableValidationLayers = true;
#endif /* NDEBUG */

class GLFW {
public:
  GLFW() { glfwInit(); }
  ~GLFW() { glfwTerminate(); }
  GLFW(const GLFW &) = delete;
  GLFW(GLFW &&) = delete;
  GLFW &operator=(const GLFW &) = delete;
  GLFW &operator=(GLFW &&) = delete;
};

class Window {
  typedef GLFWwindow *pGLFWwindow;

public:
  Window() : _window{vkh::createWindow(800, 600)} { _isOwner = true; };
  Window(int width, int height, const char *title = "Vulkan Application",
         GLFWmonitor *monitor = nullptr, GLFWwindow *share = nullptr)
      : _window{vkh::createWindow(width, height, title, monitor, share)} {
    _isOwner = true;
  }
  Window(const Window &) = delete;
  Window(Window &&rhs) { _moveDataFrom(std::move(rhs)); }
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Window() {
    if (_isOwner) {
      vkh::destroyWindow(_window);
      std::cout << "Window destructor" << '\n';
    }
  }
  const pGLFWwindow &ref() const noexcept { return _window; }

private:
  pGLFWwindow _window;
  bool _isOwner = false;

  void _moveDataFrom(Window &&rhs) {
    _window = rhs._window;
    _isOwner = true;
    rhs._isOwner = false;
  }
};

class Instance {
public:
  Instance() {
    _instance = _customInitialize();
    _isOwner = true;
  }
  Instance(VkInstanceCreateInfo *pCreateInfo,
           const VkAllocationCallbacks *pAllocator = nullptr)
      : _pAllocator{pAllocator} {
    _instance = vkh::createInstance(pCreateInfo, pAllocator);
    _isOwner = true;
  }
  Instance(const Instance &) = delete;
  Instance(Instance &&rhs) { _moveDataFrom(std::move(rhs)); }
  Instance &operator=(const Instance &) = delete;
  Instance &operator=(Instance &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Instance() {
    if (_isOwner) {
      vkh::destroyInstance(_instance, _pAllocator);
      std::cout << "Instance destructor" << '\n';
    }
  }

  const VkInstance &ref() const noexcept { return _instance; }

  const VkDebugUtilsMessengerCreateInfoEXT &debugInfo() const noexcept {
    return _debugInfo;
  }

private:
  VkInstance _instance;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;
  VkDebugUtilsMessengerCreateInfoEXT _debugInfo;

  void _moveDataFrom(Instance &&rhs) {
    _instance = rhs._instance;
    _pAllocator = rhs._pAllocator;
    _debugInfo = rhs._debugInfo;
    _isOwner = true;
    rhs._isOwner = false;
  }

  CUSTOM static VkInstance _customInitialize() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Application";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    std::vector<const char *> requiredExtensions{
        vkh::getRequiredInstanceExtensionNameList()};
    std::vector<const char *> layers = {"VK_LAYER_MANGOHUD_overlay"};
    ;
    VkInstanceCreateInfo instanceInfo{};

    if constexpr (vkw::enableValidationLayers) {
      requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      layers.push_back("VK_LAYER_KHRONOS_validation");
      VkDebugUtilsMessengerCreateInfoEXT debugInfo;
      populateDebugMessengerCreateInfo(debugInfo);
      instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugInfo;
    }

    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    instanceInfo.ppEnabledLayerNames = layers.data();
    instanceInfo.enabledExtensionCount =
        static_cast<uint32_t>(requiredExtensions.size());
    instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();
    return vkh::createInstance(&instanceInfo);
  }

  CUSTOM static void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = vkh::debugCallback;
    createInfo.pUserData = nullptr;
  }
};

class DebugMessenger {
public:
  DebugMessenger() = default;
  DebugMessenger(VkInstance instance,
                 const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo = {},
                 const VkAllocationCallbacks *pAllocator = nullptr)
      : _instance{instance}, _pAllocator{pAllocator} {
    if constexpr (vkw::enableValidationLayers) {
      if (vkh::checkValidationLayerSupport() == false) {
        throw std::runtime_error("Validation layers are not supported.");
        _debugMessenger =
            vkh::createDebugMessenger(_instance, pCreateInfo, pAllocator);
      }
      _isOwner = true;
    }
  }
  DebugMessenger(const DebugMessenger &) = delete;
  DebugMessenger(DebugMessenger &&rhs) { _moveDataFrom(std::move(rhs)); }
  DebugMessenger &operator=(const DebugMessenger &) = delete;
  DebugMessenger &operator=(DebugMessenger &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~DebugMessenger() {
    if (_isOwner) {
      vkh::destroyDebugMessenger(_instance, _debugMessenger, _pAllocator);
      std::cout << "DebugMessenger destructor" << '\n';
    }
  }

  const VkDebugUtilsMessengerEXT &ref() const noexcept {
    return _debugMessenger;
  }

private:
  VkDebugUtilsMessengerEXT _debugMessenger = VK_NULL_HANDLE;
  VkInstance _instance = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(DebugMessenger &&rhs) {
    if constexpr (vkw::enableValidationLayers) {
      _debugMessenger = rhs._debugMessenger;
      _instance = rhs._instance;
      _pAllocator = rhs._pAllocator;
      _isOwner = true;
    }
    rhs._isOwner = false;
  }
};

class Surface {
public:
  Surface() = default;
  Surface(VkInstance instance, GLFWwindow *window,
          const VkAllocationCallbacks *pAllocator = nullptr)
      : _instance{instance}, _window{window}, _pAllocator{pAllocator} {
    _surface = vkh::createSurface(instance, window, pAllocator);
    _isOwner = true;
  }
  Surface(const Surface &) = delete;
  Surface(Surface &&rhs) { _moveDataFrom(std::move(rhs)); }
  Surface &operator=(const Surface &) = delete;
  Surface &operator=(Surface &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Surface() {
    if (_isOwner) {
      vkh::destroySurface(_instance, _surface, _pAllocator);
      std::cout << "Surface destructor" << '\n';
    }
  }
  const VkSurfaceKHR &ref() const noexcept { return _surface; }

private:
  VkSurfaceKHR _surface;
  VkInstance _instance;
  GLFWwindow *_window;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(Surface &&rhs) {
    _surface = rhs._surface;
    _instance = rhs._instance;
    _window = rhs._window;
    _pAllocator = rhs._pAllocator;
    _isOwner = true;
    rhs._isOwner = false;
  }
};

struct MultiPurposeQueue {
  VkQueue ref;
  uint32_t familyIndex;
};

struct DeviceReturnWrapper {
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  MultiPurposeQueue queue;
};

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
  ~Device() {
    if (_isOwner) {
      vkh::destroyDevice(_device, _pAllocator);
      std::cout << "Device destructor" << '\n';
    }
  }

  const VkDevice &ref() const noexcept { return _device; }

  const VkPhysicalDevice &physical() const noexcept { return _physicalDevice; }

  VkQueue &queue() noexcept { return _queue.ref; }

  template <typename ReturnType = uint32_t> auto familyIndex() const noexcept {
    return static_cast<ReturnType>(_queue.familyIndex);
  }

private:
  VkDevice _device;
  VkPhysicalDevice _physicalDevice;
  MultiPurposeQueue _queue;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(Device &&rhs) {
    _device = rhs._device;
    _physicalDevice = rhs._physicalDevice;
    _queue = rhs._queue;
    _pAllocator = rhs._pAllocator;
    _isOwner = true;
    rhs._isOwner = false;
  }

  static CUSTOM DeviceReturnWrapper _customInitialize(VkInstance instance,
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
          checkDeviceSwapchainSupport(physDev)) {
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

  CUSTOM static bool checkDeviceProperties(VkPhysicalDevice physDev) {
    auto physDevPropList = vkh::getPhysicalDevicePropertyList(physDev);
    return physDevPropList.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
  }

  CUSTOM static bool checkDeviceExtensionSupport(
      VkPhysicalDevice physDev,
      const std::vector<const char *> &deviceExtensions) {
    auto availableExtensionsList =
        vkh::getAvailableDeviceExtensionList(physDev);
    std::map<std::string, uint32_t> helper;
    for (const auto &availableExtension : availableExtensionsList) {
      helper[availableExtension.extensionName]++;
      // std::cout << availableExtension.extensionName << '\n';
    }
    for (const auto &deviceExtension : deviceExtensions) {
      helper[deviceExtension]++;
      if (helper[deviceExtension] < 2) {
        return false;
      }
    }
    return true;
  }

  CUSTOM static bool
  checkDeviceSwapchainSupport([[maybe_unused]] VkPhysicalDevice physDev) {
    return true;
  }

  CUSTOM static std::optional<std::pair<uint32_t, VkQueueFamilyProperties>>
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

template <vkh::PipelineType pipelineType> class Pipeline {
public:
  Pipeline() = default;
  Pipeline(VkDevice device, VkPipelineCache pipelineCache,
           const vkh::pipelineInfoType<pipelineType> *pCreateInfo,
           const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pipelineCache{pipelineCache}, _pAllocator{
                                                            pAllocator} {
    _pipeline =
        vkh::createPipeline(device, pipelineCache, pCreateInfo, pAllocator);
    _isOwner = true;
  }
  Pipeline(const Pipeline &) = delete;
  Pipeline(Pipeline &&rhs) { _moveDataFrom(std::move(rhs)); }
  Pipeline &operator=(const Pipeline &) = delete;
  Pipeline &operator=(Pipeline &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Pipeline() {
    if (_isOwner) {
      vkh::destroyPipeline(_device, _pipeline, _pAllocator);
      std::cout << "Pipeline destructor" << '\n';
    }
  }

  const VkPipeline &ref() const noexcept { return _pipeline; }

private:
  VkPipeline _pipeline;
  VkDevice _device;
  VkPipelineCache _pipelineCache;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(Pipeline &&rhs) {
    _pipeline = rhs._pipeline;
    _device = rhs._device;
    _pipelineCache = rhs._pipelineCache;
    _pAllocator = rhs._pAllocator;
    _isOwner = true;
    rhs._isOwner = false;
  }
};

class Framebuffers {
public:
  Framebuffers() = default;
  Framebuffers(VkDevice device,
               const std::vector<VkFramebufferCreateInfo> &createInfos,
               const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    size_t framebufferCount = createInfos.size();
    _framebuffers.resize(createInfos.size());
    for (size_t i = 0; i < framebufferCount; ++i) {
      _framebuffers[i] =
          vkh::createFramebuffer(device, &createInfos[i], pAllocator);
    }
    _isOwner = true;
  }
  Framebuffers(const Framebuffers &) = delete;
  Framebuffers(Framebuffers &&rhs) { _moveDataFrom(std::move(rhs)); }
  Framebuffers &operator=(const Framebuffers &) = delete;
  Framebuffers &operator=(Framebuffers &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Framebuffers() {
    if (_isOwner) {
      for (auto &framebuffer : _framebuffers) {
        vkh::destroyFramebuffer(_device, framebuffer, _pAllocator);
      }
      std::cout << "Framebuffers destructor" << '\n';
    }
  }

  const VkFramebuffer *ref() const noexcept { return _framebuffers.data(); }

  template <typename SizeType = size_t> auto size() const noexcept {
    return static_cast<SizeType>(_framebuffers.size());
  }

  const VkFramebuffer &operator[](size_t index) const noexcept {
    return _framebuffers[index];
  }

private:
  std::vector<VkFramebuffer> _framebuffers;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(Framebuffers &&rhs) {
    _framebuffers = rhs._framebuffers;
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    _isOwner = true;
    rhs._isOwner = false;
  }
};

} /* namespace vkw */

#endif /* VK_WRAPPERS_HPP */