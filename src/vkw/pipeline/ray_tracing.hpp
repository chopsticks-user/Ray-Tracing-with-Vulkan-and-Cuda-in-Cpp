#ifndef VKW_PIPELINE_RAY_TRACING_HPP
#define VKW_PIPELINE_RAY_TRACING_HPP

#include "config.hpp"

namespace vkw {

class RayTracingPipeline {
public:
  RayTracingPipeline() = default;
  RayTracingPipeline(VkDevice device,
                     std::vector<VkRayTracingPipelineCreateInfoKHR> createInfos,
                     VkDeferredOperationKHR deferredOperation = VK_NULL_HANDLE,
                     VkPipelineCache pipelineCache = VK_NULL_HANDLE,
                     const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _deferredOperation{deferredOperation},
        _pipelineCache{pipelineCache}, _pAllocator{pAllocator} {
    vkCreateRayTracingPipelinesKHR(device, deferredOperation, pipelineCache,
                                   static_cast<uint32_t>(createInfos.size()),
                                   createInfos.data(), pAllocator,
                                   &_rtPipeline);
    _isOwner = true;
  }
  RayTracingPipeline(const RayTracingPipeline &) = delete;
  RayTracingPipeline(RayTracingPipeline &&rhs) {
    _moveDataFrom(std::move(rhs));
  }
  RayTracingPipeline &operator=(const RayTracingPipeline &) = delete;
  RayTracingPipeline &operator=(RayTracingPipeline &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~RayTracingPipeline() { _destroyVkData(); }

  const VkPipeline &ref() const noexcept { return _rtPipeline; }

protected:
  VkPipeline _rtPipeline = VK_NULL_HANDLE;
  VkDevice _device = VK_NULL_HANDLE;
  VkDeferredOperationKHR _deferredOperation = VK_NULL_HANDLE;
  VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(RayTracingPipeline &&rhs) {
    _destroyVkData();
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyPipeline(_device, _rtPipeline, _pAllocator);
      _isOwner = false;
    }
  }

private:
};

} /* namespace vkw */
#endif /* VKW_PIPELINE_RAY_TRACING_HPP */