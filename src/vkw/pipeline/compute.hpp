#ifndef VKW_PIPELINE_COMPUTE_HPP
#define VKW_PIPELINE_COMPUTE_HPP

#include "config.hpp"

namespace vkw {

class ComputePipeline {
public:
  ComputePipeline() = default;
  ComputePipeline(VkDevice device,
                  std::vector<VkComputePipelineCreateInfo> createInfos,
                  VkPipelineCache pipelineCache = VK_NULL_HANDLE,
                  const VkAllocationCallbacks *pAllocator = nullptr)
      : _device{device}, _pipelineCache{pipelineCache}, _pAllocator{
                                                            pAllocator} {
    vkCreateComputePipelines(device, pipelineCache,
                             static_cast<uint32_t>(createInfos.size()),
                             createInfos.data(), pAllocator, &_computePipeline);
    _isOwner = true;
  }
  ComputePipeline(const ComputePipeline &) = delete;
  ComputePipeline(ComputePipeline &&rhs) { _moveDataFrom(std::move(rhs)); }
  ComputePipeline &operator=(const ComputePipeline &) = delete;
  ComputePipeline &operator=(ComputePipeline &&rhs) {
    _moveDataFrom(std::move(rhs));
    return *this;
  }
  virtual ~ComputePipeline() { _destroyVkData(); }

  const VkPipeline &ref() const noexcept { return _computePipeline; }

protected:
  VkPipeline _computePipeline = VK_NULL_HANDLE;
  VkDevice _device = VK_NULL_HANDLE;
  VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
  const VkAllocationCallbacks *_pAllocator = nullptr;
  bool _isOwner = false;

  void _moveDataFrom(ComputePipeline &&rhs) {
    _destroyVkData();
    if (rhs._isOwner) {
      _isOwner = true;
      rhs._isOwner = false;
    }
  }

  void _destroyVkData() {
    if (_isOwner) {
      vkDestroyPipeline(_device, _computePipeline, _pAllocator);
      _isOwner = false;
    }
  }

private:
};

} /* namespace vkw */
#endif /* VKW_PIPELINE_COMPUTE_HPP */