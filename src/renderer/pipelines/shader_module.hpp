#ifndef NEKO_RENDERER_PIPELINES_SHADER_MODULE_HPP
#define NEKO_RENDERER_PIPELINES_SHADER_MODULE_HPP

#include "core.hpp"

namespace neko {

class Device;

class ShaderModule {
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

} /* namespace neko */

#endif /* NEKO_RENDERER_PIPELINES_SHADER_MODULE_HPP */