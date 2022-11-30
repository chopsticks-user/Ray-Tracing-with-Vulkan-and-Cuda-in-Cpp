#ifndef VKW_PIPELINE_GRAPHICS_HPP
#define VKW_PIPELINE_GRAPHICS_HPP

#include "config.hpp"

namespace vkw {

class GraphicsPipeline {
public:
  GraphicsPipeline() = default;
  GraphicsPipeline(VkDevice device,
                   const VkGraphicsPipelineCreateInfo *pCreateInfo,
                   VkPipelineCache pipelineCache = nullptr,
                   const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pipelineCache{pipelineCache},
        _pipelineLayout{pCreateInfo->layout},
        _renderPass{pCreateInfo->renderPass}, _pAllocator{pAllocator} {
    vkCreateGraphicsPipelines(device, pipelineCache, 1, pCreateInfo, pAllocator,
                              &_graphicsPipeline);
    _isOwner = true;
  }
  GraphicsPipeline(const GraphicsPipeline &) = delete;
  GraphicsPipeline(GraphicsPipeline &&rhs) { _moveDataFrom(std::move(rhs)); }
  GraphicsPipeline &operator=(const GraphicsPipeline &) = delete;
  GraphicsPipeline &operator=(GraphicsPipeline &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~GraphicsPipeline() { _destroyVkData(); }

  const VkPipeline &ref() const noexcept { return _graphicsPipeline; }

  const VkPipelineCache &cache() const noexcept { return _pipelineCache; }

  const VkPipelineLayout &layout() const noexcept { return _pipelineLayout; }

  const VkRenderPass &renderPass() const noexcept { return _renderPass; }

protected:
  VkPipeline _graphicsPipeline = VK_NULL_HANDLE;
  VkDevice _device = VK_NULL_HANDLE;
  VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
  VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
  VkRenderPass _renderPass = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
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
      vkDestroyPipeline(_device, _graphicsPipeline, _pAllocator);
      vkDestroyPipelineLayout(_device, _pipelineLayout, _pAllocator);
      vkDestroyRenderPass(_device, _renderPass, _pAllocator);
      _isOwner = false;
      if constexpr (enableValidationLayers) {
        std::cout << "GraphicsPipeline destructor" << '\n';
      }
    }
  }

  VkShaderModule
  _makeShaderModule(VkDevice device, const std::string &shaderPath,
                    const VkAllocationCallbacks *pAllocator = nullptr) {
    std::ifstream file(shaderPath, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open " + shaderPath);
    }
    size_t bufferSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(bufferSize);
    file.seekg(0);
    file.read(buffer.data(), bufferSize);
    file.close();

    VkShaderModuleCreateInfo shaderModuleInfo{};
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.codeSize = buffer.size();
    shaderModuleInfo.pCode = reinterpret_cast<const uint32_t *>(buffer.data());
    shaderModuleInfo.pNext = nullptr;

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &shaderModuleInfo, pAllocator,
                             &shaderModule) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create shader module.");
    }
    return shaderModule;
  }

private:
};

} /* namespace vkw */
#endif /* VKW_PIPELINE_GRAPHICS_HPP */