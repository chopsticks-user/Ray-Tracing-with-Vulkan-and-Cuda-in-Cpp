#include "ShaderModules.hpp"

#include "Devices/LogicalDevice.hpp"

#include <fstream>

namespace Neko
{

    ShaderModule::ShaderModule(const Device &crDevice,
                               const std::string &shaderFilePath)
        : mpcDevice{&crDevice}
    {
        std::ifstream file(shaderFilePath, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open " + shaderFilePath);
        }
        size_t bufferSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(bufferSize);
        file.seekg(0);
        file.read(buffer.data(), bufferSize);
        file.close();

        VkShaderModuleCreateInfo shaderModuleInfo{};
        shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleInfo.codeSize = buffer.size();
        shaderModuleInfo.pCode = reinterpret_cast<const uint32_t *>(buffer.data());
        shaderModuleInfo.pNext = nullptr;
        if (vkCreateShaderModule(**mpcDevice, &shaderModuleInfo, nullptr,
                                 &mShaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create shader module.");
        }
    }

    ShaderModule::ShaderModule(ShaderModule &&rhs) noexcept
        : mpcDevice{std::move(rhs.mpcDevice)},
          mShaderModule{std::exchange(rhs.mShaderModule, VK_NULL_HANDLE)} {}

    ShaderModule &ShaderModule::operator=(ShaderModule &&rhs) noexcept
    {
        release();
        mpcDevice = std::move(rhs.mpcDevice);
        mShaderModule = std::exchange(rhs.mShaderModule, VK_NULL_HANDLE);
        return *this;
    }

    void ShaderModule::release() noexcept
    {
        if (mShaderModule != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(**mpcDevice, mShaderModule, nullptr);
            mShaderModule = VK_NULL_HANDLE;
        }
    }

} // namespace Neko