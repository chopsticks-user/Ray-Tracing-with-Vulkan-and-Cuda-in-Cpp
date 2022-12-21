#ifndef NEKO_RENDERER_PIPELINES_GRAPHICS_HPP
#define NEKO_RENDERER_PIPELINES_GRAPHICS_HPP

#include "core.hpp"

#include "pipeline_layout.hpp"

namespace neko {

class Device;
class Swapchain;
class DescriptorSetLayout;
class PipelineCache;
class RenderPass;
enum class ShaderStage;

class GraphicsPipeline {
public:
  GraphicsPipeline() = default;
  GraphicsPipeline(const GraphicsPipeline &) = delete;
  GraphicsPipeline &operator=(const GraphicsPipeline &) = delete;

  GraphicsPipeline(
      const Configs &crConfigs, const Device &crDevice,
      const Swapchain &crSwapchain, const RenderPass &crRenderPass,
      const DescriptorSetLayout &crDSLayout,
      const std::vector<std::pair<ShaderStage, std::string>> &pairShaderPaths);

  GraphicsPipeline(GraphicsPipeline &&rhs) noexcept;

  GraphicsPipeline &operator=(GraphicsPipeline &&rhs) noexcept;

  ~GraphicsPipeline() noexcept { release(); }

  const VkPipeline &operator*() const noexcept { return mGPipeline; }

  void release() noexcept;

private:
  const Device *mpcDevice = nullptr;
  const RenderPass *mpcRenderPass = nullptr;
  PipelineLayout mLayout = {};
  VkPipeline mGPipeline = VK_NULL_HANDLE;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_PIPELINES_GRAPHICS_HPP */