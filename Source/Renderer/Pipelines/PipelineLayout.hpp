#ifndef NEKO_RENDERER_PIPELINES_LAYOUT_HPP
#define NEKO_RENDERER_PIPELINES_LAYOUT_HPP

#include "CoreNeko.hpp"

namespace neko {

class Device;
class DescriptorSetLayout;

class PipelineLayout {
public:
  PipelineLayout() = default;
  PipelineLayout(const PipelineLayout &) = delete;
  PipelineLayout &operator=(const PipelineLayout &) = delete;

  PipelineLayout(const Device &crDevice,
                 const std::vector<VkDescriptorSetLayout> &crDSLayout);

  PipelineLayout(PipelineLayout &&rhs) noexcept;

  PipelineLayout &operator=(PipelineLayout &&rhs) noexcept;

  ~PipelineLayout() noexcept { release(); }

  const VkPipelineLayout &operator*() const noexcept { return mPLayout; }

  void release() noexcept;

private:
  const Device *mpcDevice = nullptr;
  VkPipelineLayout mPLayout = VK_NULL_HANDLE;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_PIPELINES_LAYOUT_HPP */