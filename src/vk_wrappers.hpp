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
         bool resizable = true, GLFWmonitor *monitor = nullptr,
         GLFWwindow *share = nullptr)
      : _window{vkh::createWindow(width, height, title, resizable, monitor,
                                  share)} {
    _isOwner = true;
  }
  Window(const Window &) = delete;
  Window(Window &&rhs) { _moveDataFrom(std::move(rhs)); }
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Window() { _destroyVkData(); }
  const pGLFWwindow &ref() const noexcept { return _window; }

private:
  pGLFWwindow _window;
  bool _isOwner = false;

  void _moveDataFrom(Window &&rhs) {
    _window = rhs._window;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroyWindow(_window);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Window destructor" << '\n';
      }
    }
  }
};

class Instance {
public:
  Instance() {
    _instance = _customInitialize(_debugInfo);
    _isOwner = true;
  }
  Instance(VkInstanceCreateInfo *pCreateInfo,
           const VkAllocationCallbacks *pAllocator = nullptr)
      : _pAllocator{pAllocator} {
    _instance = vkh::createInstance(pCreateInfo, pAllocator);
    if constexpr (enableValidationLayers) {
      _debugInfo = *(VkDebugUtilsMessengerCreateInfoEXT *)(pCreateInfo->pNext);
    }
    _isOwner = true;
  }
  Instance(const Instance &) = delete;
  Instance(Instance &&rhs) { _moveDataFrom(std::move(rhs)); }
  Instance &operator=(const Instance &) = delete;
  Instance &operator=(Instance &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Instance() { _destroyVkData(); }

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
    _debugInfo = std::move(rhs._debugInfo);
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroyInstance(_instance, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Instance destructor" << '\n';
      }
    }
  }

  CUSTOM static VkInstance
  _customInitialize(VkDebugUtilsMessengerCreateInfoEXT &debInfo) {
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
      debInfo = debugInfo;
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
                 const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                 const VkAllocationCallbacks *pAllocator = nullptr)
      : _instance{instance}, _pAllocator{pAllocator} {
    if constexpr (vkw::enableValidationLayers) {
      if (vkh::checkValidationLayerSupport() == false) {
        throw std::runtime_error("Validation layers are not supported.");
      }
      _debugMessenger =
          vkh::createDebugMessenger(_instance, pCreateInfo, pAllocator);
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
  ~DebugMessenger() { _destroyVkData(); }

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

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroyDebugMessenger(_instance, _debugMessenger, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "DebugMessenger destructor" << '\n';
      }
    }
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
  ~Surface() { _destroyVkData(); }
  const VkSurfaceKHR &ref() const noexcept { return _surface; }

private:
  VkSurfaceKHR _surface;
  VkInstance _instance;
  GLFWwindow *_window;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(Surface &&rhs) {
    _destroyVkData();
    _surface = rhs._surface;
    _instance = rhs._instance;
    _window = rhs._window;
    _pAllocator = rhs._pAllocator;
    _isOwner = true;
    rhs._isOwner = false;
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroySurface(_instance, _surface, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Surface destructor" << '\n';
      }
    }
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
  ~Device() { _destroyVkData(); }

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

class GraphicsPipeline {
public:
  GraphicsPipeline() = default;
  GraphicsPipeline(VkDevice device, VkExtent2D extent, VkFormat format,
                   const VkDescriptorSetLayout *desSetLayout,
                   const std::string vertexFilePath,
                   const std::string fragFilePath)
      : _device{device}, _pipelineCache{nullptr}, _pAllocator{nullptr} {
    _customInitialize(device, extent, format, desSetLayout, vertexFilePath,
                      fragFilePath);
  }
  GraphicsPipeline(VkDevice device,
                   const VkGraphicsPipelineCreateInfo *pCreateInfo,
                   VkPipelineCache pipelineCache = nullptr,
                   const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pipelineCache{pipelineCache},
        _pipelineLayout{pCreateInfo->layout},
        _renderPass{pCreateInfo->renderPass}, _pAllocator{pAllocator} {
    _graphicsPipeline =
        vkh::createPipeline(device, pipelineCache, pCreateInfo, pAllocator);
    _isOwner = true;
  }
  GraphicsPipeline(const GraphicsPipeline &) = delete;
  GraphicsPipeline(GraphicsPipeline &&rhs) { _moveDataFrom(std::move(rhs)); }
  GraphicsPipeline &operator=(const GraphicsPipeline &) = delete;
  GraphicsPipeline &operator=(GraphicsPipeline &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~GraphicsPipeline() { _destroyVkData(); }

  const VkPipeline &ref() const noexcept { return _graphicsPipeline; }

  const VkPipelineCache &cache() const noexcept { return _pipelineCache; }

  const VkPipelineLayout &layout() const noexcept { return _pipelineLayout; }

  const VkRenderPass &renderPass() const noexcept { return _renderPass; }

private:
  VkPipeline _graphicsPipeline;
  VkDevice _device;
  VkPipelineCache _pipelineCache;
  VkPipelineLayout _pipelineLayout;
  VkRenderPass _renderPass;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(GraphicsPipeline &&rhs) {
    _destroyVkData();
    _graphicsPipeline = rhs._graphicsPipeline;
    _device = rhs._device;
    _pipelineCache = rhs._pipelineCache;
    _pipelineLayout = rhs._pipelineLayout;
    _renderPass = rhs._renderPass;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroyPipeline(_device, _graphicsPipeline, _pAllocator);
      vkh::destroyPipelineLayout(_device, _pipelineLayout, _pAllocator);
      vkh::destroyRenderPass(_device, _renderPass, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "GraphicsPipeline destructor" << '\n';
      }
    }
  }

  CUSTOM void _customInitialize(VkDevice device, const VkExtent2D &extent,
                                const VkFormat &format,
                                const VkDescriptorSetLayout *desSetLayout,
                                const std::string &vertexFilePath,
                                const std::string &fragFilePath) {
    vkh::ShaderModuleWrapper shaderModule{};
    shaderModule.vertex = vkh::createShaderModule(
        device, vkh::absoluteDirectory + vertexFilePath);
    shaderModule.fragment =
        vkh::createShaderModule(device, vkh::absoluteDirectory + fragFilePath);

    /*  */
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = shaderModule.vertex;
    vertShaderStageInfo.pName = "main";
    vertShaderStageInfo.pSpecializationInfo = nullptr;

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = shaderModule.fragment;
    fragShaderStageInfo.pName = "main";
    fragShaderStageInfo.pSpecializationInfo = nullptr;

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                      fragShaderStageInfo};

    /*  */
    auto bindingDescription = vkh::Vertex2D_RGB::getBindingDescription();
    auto attributeDescriptions = vkh::Vertex2D_RGB::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertInputInfo{};
    vertInputInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertInputInfo.vertexBindingDescriptionCount = 1;
    vertInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertInputInfo.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(attributeDescriptions.size());
    vertInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    /*  */
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    inputAssemblyInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    /*  */
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;

    VkPipelineViewportStateCreateInfo viewportStateInfo{};
    viewportStateInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.viewportCount = 1;
    viewportStateInfo.pViewports = &viewport;
    viewportStateInfo.scissorCount = 1;
    viewportStateInfo.pScissors = &scissor;

    /*  */
    VkPipelineRasterizationStateCreateInfo rastStateInfo{};
    rastStateInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rastStateInfo.pNext = nullptr;
    rastStateInfo.depthClampEnable = VK_FALSE;
    rastStateInfo.rasterizerDiscardEnable = VK_FALSE;
    rastStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rastStateInfo.lineWidth = 1.0f;
    rastStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rastStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rastStateInfo.depthBiasEnable = VK_FALSE;
    rastStateInfo.depthBiasConstantFactor = 0.0f;
    rastStateInfo.depthBiasClamp = 0.0f;
    rastStateInfo.depthBiasSlopeFactor = 0.0f;

    /*  */
    VkPipelineMultisampleStateCreateInfo multisampleStateInfo{};
    multisampleStateInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleStateInfo.sampleShadingEnable = VK_FALSE;
    multisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleStateInfo.minSampleShading = 1.0f;
    multisampleStateInfo.pSampleMask = nullptr;
    multisampleStateInfo.alphaToCoverageEnable = VK_FALSE;
    multisampleStateInfo.alphaToOneEnable = VK_FALSE;

    /* Depth and stencil testing */

    /* Color blending */
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor =
        VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlendState{};
    colorBlendState.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &colorBlendAttachment;
    colorBlendState.blendConstants[0] = 0.0f;
    colorBlendState.blendConstants[1] = 0.0f;
    colorBlendState.blendConstants[2] = 0.0f;
    colorBlendState.blendConstants[3] = 0.0f;

    /* Dynamic state */
    // std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
    //                                              VK_DYNAMIC_STATE_LINE_WIDTH};

    // VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
    // dynamicStateInfo.sType =
    //     VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicStateInfo.dynamicStateCount =
    //     static_cast<uint32_t>(dynamicStates.size());
    // dynamicStateInfo.pDynamicStates = dynamicStates.data();

    /* Pipeline layout */
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = desSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    _pipelineLayout = vkh::createPipelineLayout(device, &pipelineLayoutInfo);

    /* Render pass */
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency subPassDep{}; /* Needed when rendering */
    subPassDep.srcSubpass = VK_SUBPASS_EXTERNAL;
    subPassDep.dstSubpass = 0;
    subPassDep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subPassDep.srcAccessMask = 0;
    subPassDep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subPassDep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.pNext = nullptr;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &subPassDep;
    _renderPass = vkh::createRenderPass(device, &renderPassInfo);

    /* Create a graphics pipeline */
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.stageCount = vkh::ShaderModuleWrapper::stageCount;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportStateInfo;
    pipelineInfo.pRasterizationState = &rastStateInfo;
    pipelineInfo.pMultisampleState = &multisampleStateInfo;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlendState;
    pipelineInfo.pDynamicState = nullptr;
    pipelineInfo.layout = _pipelineLayout;
    pipelineInfo.renderPass = _renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    _graphicsPipeline = vkh::createPipeline<vkh::Graphics>(
        device, _pipelineCache, &pipelineInfo);
    vkh::destroyShaderModule(device, shaderModule.vertex);
    vkh::destroyShaderModule(device, shaderModule.fragment);
    _isOwner = true;
  }
};

class Framebuffers {
public:
  Framebuffers() = default;
  Framebuffers(VkDevice device, const std::vector<VkImageView> &imageViews,
               VkRenderPass renderPass, VkExtent2D extent)
      : _device{device}, _pAllocator{nullptr} {
    _customInitialize(device, imageViews, renderPass, extent);
  }
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
  ~Framebuffers() { _destroyVkData(); }

  const std::vector<VkFramebuffer> &ref() const noexcept {
    return _framebuffers;
  }

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
    _destroyVkData();
    _framebuffers = std::move(rhs._framebuffers);
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      for (auto &framebuffer : _framebuffers) {
        vkh::destroyFramebuffer(_device, framebuffer, _pAllocator);
      }
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Framebuffers destructor" << '\n';
      }
    }
  }

  CUSTOM void _customInitialize(VkDevice device,
                                const std::vector<VkImageView> &imageViews,
                                VkRenderPass renderPass, VkExtent2D extent) {
    size_t imageCount = imageViews.size();
    _framebuffers.resize(imageCount);
    std::vector<VkImageView> attachments{imageViews};
    for (size_t i = 0; i < imageViews.size(); ++i) {
      VkFramebufferCreateInfo framebufferInfo{};
      framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      framebufferInfo.renderPass = renderPass;
      framebufferInfo.attachmentCount = 1;
      framebufferInfo.pAttachments = &attachments[i];
      framebufferInfo.width = extent.width;
      framebufferInfo.height = extent.height;
      framebufferInfo.layers = 1;
      _framebuffers[i] = vkh::createFramebuffer(device, &framebufferInfo);
    }
    _isOwner = true;
  }
};

class Swapchain {
public:
  Swapchain() = default;
  Swapchain(VkSurfaceKHR surface, VkDevice device,
            VkPhysicalDevice physicalDevice,
            VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_FIFO_KHR)
      : _device{device}, _pAllocator{nullptr} {
    _customInitialize(surface, device, physicalDevice, preferredPresentMode);
  }
  Swapchain(VkDevice device, const VkSwapchainCreateInfoKHR *pCreateInfo,
            const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pAllocator{pAllocator} {
    _swapchain = vkh::createSwapchain(device, pCreateInfo, pAllocator);
    _format = pCreateInfo->imageFormat;
    _extent = pCreateInfo->imageExtent;
    _isOwner = true;
  }
  Swapchain(const Swapchain &) = delete;
  Swapchain(Swapchain &&rhs) { _moveDataFrom(std::move(rhs)); }
  Swapchain &operator=(const Swapchain &) = delete;
  Swapchain &operator=(Swapchain &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  ~Swapchain() { _destroyVkData(); }

  const VkSwapchainKHR &ref() const noexcept { return _swapchain; }

  const VkFormat &format() const noexcept { return _format; }

  const VkExtent2D &extent() const noexcept { return _extent; }

private:
  VkSwapchainKHR _swapchain;
  VkFormat _format;
  VkExtent2D _extent;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(Swapchain &&rhs) {
    _destroyVkData();
    _swapchain = rhs._swapchain;
    _format = std::move(rhs._format);
    _extent = std::move(rhs._extent);
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkh::destroySwapchain(_device, _swapchain, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Swapchain destructor" << '\n';
      }
    }
  }

  CUSTOM void _customInitialize(
      VkSurfaceKHR surface, VkDevice device, VkPhysicalDevice physicalDevice,
      VkPresentModeKHR preferredPresentMode = VK_PRESENT_MODE_FIFO_KHR) {
    /* Vulkan 1.3.231 - A Specification, pg 2235 */

    auto surfaceCapabilities =
        vkh::getPhysicalDeviceSurfaceCapabilities(physicalDevice, surface);
    auto surfaceFormats =
        vkh::getPhysicalDeviceSurfaceFormatList(physicalDevice, surface);
    auto surfacePresentModes =
        vkh::getPhysicalDeviceSurfacePresentModeList(physicalDevice, surface);

    VkSwapchainCreateInfoKHR swapchainInfo{};
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.pNext = nullptr;

    /* Images of the swapchain can be used to create a VkImageView with a
    different format than what the swapchain was create with. */
    /* If flags contains VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR then the
    pNext chain must include a VkImageFormatListCreateInfo structure with a
    viewFormatCount greater than zero and pViewFormats must have an element
    equal to imageFormat */
    // swapchainInfo.flags = VK_SWAPCHAIN_CREATE_MUTABLE_FORMAT_BIT_KHR;

    swapchainInfo.surface = surface;

    /* plus 1 to prevent waiting on the driver to complete internal operations
    before another image is accquired */
    uint32_t minImageCount = surfaceCapabilities.minImageCount + 1;

    /* minImageCount <= surfaceCapabilities.maxImageCount, and
    surfaceCapabilities.maxImageCount might be less than
    surfaceCapabilities.minImageCount */
    if (surfaceCapabilities.maxImageCount > 0 &&
        minImageCount > surfaceCapabilities.maxImageCount) {
      minImageCount = surfaceCapabilities.maxImageCount;
    }
    swapchainInfo.minImageCount = minImageCount;

    /* imageFormat and imageColorSpace must match the surfaceFormats.format and
    surfaceFormats.colorSpace members, respectively */
    if (surfaceFormats.empty()) {
      throw std::runtime_error("No surface format available.");
    }
    swapchainInfo.imageFormat = surfaceFormats[0].format;
    swapchainInfo.imageColorSpace = surfaceFormats[0].colorSpace;
    for (const auto &format : surfaceFormats) {
      if (format.format == VK_FORMAT_R8G8B8A8_SRGB &&
          format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        swapchainInfo.imageFormat = format.format;
        swapchainInfo.imageColorSpace = format.colorSpace;
        break;
      }
    }

    /* imageExtent is the size (in pixels) of the swapchain
    image(s). The behavior is platform-dependent if the image
    extent does not match the surface’s currentExtent as returned
    by vkGetPhysicalDeviceSurfaceCapabilitiesKHR. */
    uint32_t imageExtentWidth = surfaceCapabilities.currentExtent.width;
    uint32_t imageExtentHeight = surfaceCapabilities.currentExtent.height;

    /* {0, 0} <= minImageExtent <= imageExtent <= maxImageExtent */
    if (imageExtentWidth > surfaceCapabilities.maxImageExtent.width) {
      imageExtentWidth = surfaceCapabilities.maxImageExtent.width;
    }
    if (imageExtentHeight > surfaceCapabilities.maxImageExtent.height) {
      imageExtentHeight = surfaceCapabilities.maxImageExtent.height;
    }
    swapchainInfo.imageExtent = {imageExtentWidth, imageExtentHeight};

    /* imageArrayLayers is the number of views in a multiview/stereo surface.
    For non-stereoscopic-3D applications, this value is 1. */
    /* 0 < imageArrayLayers <= maxImageArrayLayers */
    swapchainInfo.imageArrayLayers = 1;

    /* {presentMode} determines how incoming present requests will be processed
    and queued internally. */
    /* enum {VkPresentModeKHR}, pg 2219 */
    /* {VK_PRESENT_MODE_FIFO_KHR} is guaranteed to be available */
    swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    // auto  = VK_PRESENT_MODE_IMMEDIATE_KHR;
    for (const auto &presentMode : surfacePresentModes) {
      if (presentMode == preferredPresentMode) {
        swapchainInfo.presentMode = preferredPresentMode;
        break;
      }
    }

    /* {VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT} specifies that the image can be
    used to create a {VkImageView} suitable for use as a color or resolve
    attachment in a {VkFramebuffer}, pg 898 */
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    /* {VK_SHARING_MODE_EXCLUSIVE} requires some works to be done with ownership
     */
    swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchainInfo.queueFamilyIndexCount = 0;
    swapchainInfo.pQueueFamilyIndices = nullptr;

    /* image content is presented without being transformed */
    if (surfaceCapabilities.supportedTransforms &
        VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
      swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
      throw std::runtime_error(
          "VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR not supported.");
    }

    if (surfaceCapabilities.supportedCompositeAlpha &
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR) {
      swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    } else {
      throw std::runtime_error(
          "VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR not supported.");
    }

    swapchainInfo.clipped = VK_TRUE;
    swapchainInfo.pNext = nullptr;
    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

    _format = swapchainInfo.imageFormat;
    _extent = swapchainInfo.imageExtent;
    _swapchain = vkh::createSwapchain(device, &swapchainInfo);
    _isOwner = true;
  }
};

class ImageViews {
public:
  ImageViews() = default;
  ImageViews(VkDevice device, VkSwapchainKHR swapchain, VkFormat format)
      : _device{device}, _pAllocator{nullptr} {
    _customInitialize(device, swapchain, format);
  }
  ImageViews(VkDevice device,
             const std::vector<VkImageViewCreateInfo> &createInfos,
             const VkAllocationCallbacks *pAllocator = nullptr)
      : _imageViews{createInfos.size()}, _device{device}, _pAllocator{
                                                              pAllocator} {
    size_t imageCount = createInfos.size();
    for (size_t i = 0; i < imageCount; ++i) {
      _imageViews[i] = vkh::createImageView(device, &createInfos[i]);
    }
    _isOwner = true;
  }
  ImageViews(const ImageViews &) = delete;
  ImageViews(ImageViews &&rhs) { _moveDataFrom(std::move(rhs)); }
  ImageViews &operator=(const ImageViews &) = delete;
  ImageViews &operator=(ImageViews &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }

  ~ImageViews() { _destroyVkData(); }

  const std::vector<VkImageView> &ref() { return _imageViews; }

  template <typename SizeType = size_t> auto size() const noexcept {
    return static_cast<SizeType>(_imageViews.size());
  }

  const VkImageView &operator[](size_t index) const noexcept {
    return _imageViews[index];
  }

private:
  std::vector<VkImageView> _imageViews;
  VkDevice _device;
  const VkAllocationCallbacks *_pAllocator;
  bool _isOwner = false;

  void _moveDataFrom(ImageViews &&rhs) {
    _destroyVkData();
    _imageViews = std::move(rhs._imageViews);
    _device = rhs._device;
    _pAllocator = rhs._pAllocator;
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      for (auto &imageView : _imageViews) {
        vkh::destroyImageView(_device, imageView, _pAllocator);
      }
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "Framebuffers destructor" << '\n';
      }
    }
  }

  CUSTOM void _customInitialize(VkDevice device, VkSwapchainKHR swapchain,
                                VkFormat format) {
    auto images = vkh::getSwapchainImages(device, swapchain);
    size_t imageCount = images.size();
    _imageViews.resize(imageCount);
    for (size_t i = 0; i < imageCount; ++i) {
      VkImageViewCreateInfo imageViewInfo{};
      imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      imageViewInfo.pNext = nullptr;
      // imageViewInfo.flags =
      imageViewInfo.image = images[i];

      /* treat images as 2D textures */
      imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

      imageViewInfo.format = format;

      /* default mapping */
      imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
      imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

      /* color aspect */
      imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

      /* In stereographic 3D applications, create a swapchain with multiple
      layers before creating multiple image views for each images representing
      the views for the left and right eyes by accessing different layers */
      imageViewInfo.subresourceRange.baseMipLevel = 0;
      imageViewInfo.subresourceRange.levelCount = 1;
      imageViewInfo.subresourceRange.baseArrayLayer = 0;
      imageViewInfo.subresourceRange.layerCount = 1;

      _imageViews[i] = vkh::createImageView(device, &imageViewInfo);
    }
    _isOwner = true;
  }
};

} /* namespace vkw */

#endif /* VK_WRAPPERS_HPP */