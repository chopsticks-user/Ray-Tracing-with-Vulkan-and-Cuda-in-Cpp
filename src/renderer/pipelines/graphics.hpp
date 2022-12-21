#ifndef NEKO_RENDERER_PIPELINES_GRAPHICS_HPP
#define NEKO_RENDERER_PIPELINES_GRAPHICS_HPP

#include "core.hpp"

#include "pipeline_cache.hpp"
#include "pipeline_layout.hpp"
#include "render_pass.hpp"
#include "shader_module.hpp"

namespace neko {

class Device;
class Swapchain;
class DescriptorSetLayout;
class DepthBuffer;

class GraphicsPipeline {
public:
  GraphicsPipeline() = default;
  GraphicsPipeline(const GraphicsPipeline &) = delete;
  GraphicsPipeline &operator=(const GraphicsPipeline &) = delete;

  GraphicsPipeline(
      const Configs &crConfigs, const Device &crDevice,
      const Swapchain &crSwapchain, const DescriptorSetLayout &crDSLayout,
      const DepthBuffer &depthBuffer,
      const std::vector<std::pair<ShaderStage, std::string>> &pairShaderPaths,
      const PipelineCache &crCache = {});

  GraphicsPipeline(GraphicsPipeline &&rhs) noexcept;

  GraphicsPipeline &operator=(GraphicsPipeline &&rhs) noexcept;

  ~GraphicsPipeline() noexcept { release(); }

  const VkPipeline &operator*() const noexcept { return mGPipeline; }

  void release() noexcept;

private:
  const Device *mpcDevice = nullptr;
  const PipelineCache *mpcCache = nullptr;
  VkPipeline mGPipeline = VK_NULL_HANDLE;
  PipelineLayout mLayout = {};
  RenderPass mRenderPass = {};
};

} /* namespace neko */

#endif /* NEKO_RENDERER_PIPELINES_GRAPHICS_HPP */