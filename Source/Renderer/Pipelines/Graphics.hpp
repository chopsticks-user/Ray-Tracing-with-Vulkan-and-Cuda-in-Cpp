#ifndef NEKO_RENDERER_PIPELINES_GRAPHICS_HPP
#define NEKO_RENDERER_PIPELINES_GRAPHICS_HPP

#include "CoreNeko.hpp"

namespace neko {

class Device;
class Swapchain;
class PipelineLayout;
class PipelineCache;
class RenderPass;
enum class ShaderStage;

class GraphicsPipeline {
public:
  GraphicsPipeline() = default;
  GraphicsPipeline(const GraphicsPipeline &) = delete;
  GraphicsPipeline &operator=(const GraphicsPipeline &) = delete;

  GraphicsPipeline(
      const EngineConfigs &crConfigs, const Device &crDevice,
      const Swapchain &crSwapchain, const RenderPass &crRenderPass,
      const PipelineLayout &crPipelineLayout,
      const std::vector<std::pair<ShaderStage, std::string>> &pairShaderPaths);

  GraphicsPipeline(GraphicsPipeline &&rhs) noexcept;

  GraphicsPipeline &operator=(GraphicsPipeline &&rhs) noexcept;

  ~GraphicsPipeline() noexcept { release(); }

  const VkPipeline &operator*() const noexcept { return mGPipeline; }

  void release() noexcept;

private:
  const Device *mpcDevice = nullptr;
  const RenderPass *mpcRenderPass = nullptr;
  const PipelineLayout *mpcPipelineLayout = nullptr;
  VkPipeline mGPipeline = VK_NULL_HANDLE;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_PIPELINES_GRAPHICS_HPP */