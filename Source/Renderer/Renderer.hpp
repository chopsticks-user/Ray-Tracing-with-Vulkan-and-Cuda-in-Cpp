#ifndef NEKO_RENDERER_HPP
#define NEKO_RENDERER_HPP

#include "CoreNeko.hpp"

#include "RendererModules.hpp"

namespace Neko
{
    struct ModelVertexData
    {
        std::vector<ShaderObject::Vertex> vertices;
        std::vector<ShaderObject::Index> indices;
    };

    class Renderer
    {
    public:
        Renderer() = default;
        Renderer(const Renderer &) = delete;
        Renderer(Renderer &&) = default;
        Renderer &operator=(const Renderer &) = delete;
        Renderer &operator=(Renderer &&) = default;

        Renderer(const EngineConfigs &settings, ThreadPool &threadPool);

        ~Renderer();

        void start();

    private:
        const u64 maxFramesInFlight = 2;
        bool resized = false;

        // std::vector<ShaderObject::Vertex> vertices;
        // std::vector<ShaderObject::Index> indices;

        ModelVertexData modelData;

        void
        updateDesciptorSets();

        ShaderObject::Uniform getUniformBuffer(float elapsedTime);

        void updateFrame(u32 currentFrame);

        void recordCommandBuffer(VkCommandBuffer commandBuffer, u32 imageIndex);

        void renderProcess();

        static void resizeCallback(GLFWwindow *window, i32 width, i32 height);

        void recreateSwapchain();

        ModelVertexData loadModel(const std::string &modelPath);

    private:
        const EngineConfigs *mpConfigs;
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
        VertexBuffer mVertexBuffer;
        IndexBuffer mIndexBuffer;
        TextureImage mTextureImage;
        Sampler mSampler;

        DescriptorSetLayout mDescriptorSetLayout;
        DescriptorPool mDescriptorPool;
        DescriptorSets mDescriptorSets;

        RenderPass mRenderPass;
        Framebuffers mFramebuffers;
        PipelineLayout mPipelineLayout;
        GraphicsPipeline mGraphicsPipeline;
    };

} // namespace Neko

#endif /* NEKO_RENDERER_HPP */