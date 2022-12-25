#ifndef NEKO_RENDERER_HPP
#define NEKO_RENDERER_HPP

#include "CoreNeko.hpp"

#include "RendererModules.hpp"

namespace Neko
{
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

        std::vector<ShaderObject::Vertex> vertices = {
            {{-0.25f, -0.25f, -0.2f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.25f, -0.25f, -0.2f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.25f, 0.25f, -0.2f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.25f, 0.25f, -0.2f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

            {{-0.25f, -0.25f, 0.3f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.25f, -0.25f, 0.3f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.25f, 0.25f, 0.3f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.25f, 0.25f, 0.3f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

        };
        std::vector<ShaderObject::Index> indices{0, 1, 2, 2, 3, 0,
                                                 4, 5, 6, 6, 7, 4,
                                                 2, 6, 7, 7, 3, 2,
                                                 1, 5, 4, 4, 0, 1,
                                                 1, 5, 6, 6, 2, 1,
                                                 0, 4, 7, 7, 3, 0};

        void updateDesciptorSets();

        ShaderObject::Uniform getUniformBuffer(float elapsedTime);

        void updateFrame(u32 currentFrame);

        void recordCommandBuffer(VkCommandBuffer commandBuffer, u32 imageIndex);

        void renderProcess();

        static void resizeCallback(GLFWwindow *window, i32 width, i32 height);

        void recreateSwapchain();

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