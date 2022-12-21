#ifndef NEKO_RENDERER_HPP
#define NEKO_RENDERER_HPP

#include "core.hpp"

#include "basic/context.hpp"
#include "basic/extensions.hpp"
#include "basic/framebuffers.hpp"
#include "basic/instance.hpp"
#include "basic/surface.hpp"
#include "basic/swapchain.hpp"
#include "basic/sync.hpp"
#include "basic/window.hpp"
#include "commands/commands.hpp"
#include "devices/logical_device.hpp"
#include "devices/physical_device.hpp"
#include "devices/queues.hpp"
#include "pipelines/compute.hpp"
#include "pipelines/graphics.hpp"
#include "pipelines/pipeline_cache.hpp"
#include "pipelines/pipeline_layout.hpp"
#include "pipelines/render_pass.hpp"
#include "pipelines/shader_module.hpp"
#include "pipelines/shader_objects.hpp"
#include "resources/buffer.hpp"
#include "resources/descriptor.hpp"
#include "resources/image.hpp"
#include "resources/sampler.hpp"
#include "resources/usage_specific.hpp"
#include "resources/utils.hpp"

namespace neko {

class ThreadPool;

class Renderer {
public:
  Renderer() = delete;
  Renderer(const Renderer &) = delete;
  Renderer(Renderer &&) = default;
  Renderer &operator=(const Renderer &) = delete;
  Renderer &operator=(Renderer &&) = default;

  Renderer(const Configs &settings, ThreadPool &threadPool);

  ~Renderer();

  void start();

private:
  const u64 maxFrameInFlights = 2;

  void recordCommandBuffer(VkCommandBuffer commandBuffer, u32 imageIndex);

  void renderProcess();

private:
  const Configs *mpConfigs;
  ThreadPool *mpThreadPool;

  Instance mInstance;
  Window mWindow;
  Surface mSurface;
  Device mDevice;
  CommandPool mCommandPool;
  std::vector<VkCommandBuffer> mCommandBuffers;
  Swapchain mSwapchain;
  RenderSync mRenderSync;

  DepthBuffer mDepthBuffer;
  std::vector<UniformBuffer> mUniformBuffers;
  Sampler mSampler;

  DescriptorSetLayout mDescriptorSetLayout;
  DescriptorPool mDescriptorPool;
  DescriptorSets mDescriptorSets;

  RenderPass mRenderPass;
  Framebuffers mFramebuffers;
  GraphicsPipeline mGraphicsPipeline;
};

} /* namespace neko */

#endif /* NEKO_RENDERER_HPP */