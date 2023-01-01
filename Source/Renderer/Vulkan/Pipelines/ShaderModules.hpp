#ifndef HDG_RENDERER_VULKAN_PIPELINES_SHADER_MODULE_HPP
#define HDG_RENDERER_VULKAN_PIPELINES_SHADER_MODULE_HPP

#include "CoreInternal.hpp"
#include "Basic/Context.hpp"

namespace Neko::Internal::VK
{
    class Device;

    enum class ShaderStage
    {
        vertex = VK_SHADER_STAGE_VERTEX_BIT,
        fragment = VK_SHADER_STAGE_FRAGMENT_BIT,
        compute = VK_SHADER_STAGE_COMPUTE_BIT,
    };

    class ShaderModule
    {
    public:
        ShaderModule() = default;
        ShaderModule(const ShaderModule &) = delete;
        ShaderModule &operator=(const ShaderModule &) = delete;

        ShaderModule(const Device &crDevice, const std::string &shaderFilePath);

        ShaderModule(ShaderModule &&rhs) noexcept;

        ShaderModule &operator=(ShaderModule &&rhs) noexcept;

        ~ShaderModule() noexcept { release(); }

        const VkShaderModule &operator*() const noexcept { return mShaderModule; }

        void release() noexcept;

    private:
        const Device *mpcDevice = nullptr;
        VkShaderModule mShaderModule = VK_NULL_HANDLE;
    };

} // namespace Neko::Internal::VK

#endif /* HDG_RENDERER_VULKAN_PIPELINES_SHADER_MODULE_HPP */