#ifndef HDG_RENDERER_GENERIC_HPP
#define HDG_RENDERER_GENERIC_HPP

#include "CoreInternal.hpp"

#include "VulkanModules.hpp"

namespace Neko::Internal
{
    struct ModelVertexData
    {
        std::vector<VK::ShaderObject::Vertex> vertices;
        std::vector<VK::ShaderObject::Index> indices;
    };

    class Renderer
    {
    public:
        Renderer() = default;
        Renderer(const Renderer &) = delete;
        Renderer(Renderer &&) = default;
        Renderer &operator=(const Renderer &) = delete;
        Renderer &operator=(Renderer &&) = default;

        Renderer(const Core::EngineConfigs &settings, Core::ThreadPool &threadPool);

        ~Renderer();

        void start();

    private:
        const u64 maxFramesInFlight = 2;
        bool resized = false;

        ModelVertexData modelData;

        void
        updateDesciptorSets();

        VK::ShaderObject::Uniform getUniformBuffer(float elapsedTime);

        void updateFrame(u32 currentFrame);

        void recordCommandBuffer(VkCommandBuffer commandBuffer, u32 imageIndex);

        void renderProcess();

        static void resizeCallback(GLFWwindow *window, i32 width, i32 height);

        void recreateSwapchain();

        ModelVertexData loadModel(const std::string &modelPath);

    private:
        const Core::EngineConfigs *mpConfigs;
        Core::ThreadPool *mpThreadPool;

        VK::Instance mInstance;
        VK::Window mWindow;
        VK::Surface mSurface;
        VK::Device mDevice;
        VK::CommandPool mCommandPool;
        std::vector<VkCommandBuffer> mCommandBuffers;
        VK::Swapchain mSwapchain;
        VK::RenderSync mRenderSync;

        VK::DepthBuffer mDepthBuffer;
        std::vector<VK::UniformBuffer> mUniformBuffers;
        VK::VertexBuffer mVertexBuffer;
        VK::IndexBuffer mIndexBuffer;
        VK::TextureImage mTextureImage;
        VK::Sampler mSampler;

        VK::DescriptorSetLayout mDescriptorSetLayout;
        VK::DescriptorPool mDescriptorPool;
        VK::DescriptorSets mDescriptorSets;

        VK::RenderPass mRenderPass;
        VK::Framebuffers mFramebuffers;
        VK::PipelineLayout mPipelineLayout;
        VK::GraphicsPipeline mGraphicsPipeline;
    };

} // namespace Neko::Internal

#endif /* HDG_RENDERER_GENERIC_HPP */