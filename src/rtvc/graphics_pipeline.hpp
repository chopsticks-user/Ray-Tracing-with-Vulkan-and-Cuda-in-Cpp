#ifndef RTVC_GRAPHICS_PIPELINE_HPP
#define RTVC_GRAPHICS_PIPELINE_HPP

#include "descriptor.hpp"
#include "device.hpp"
#include "swapchain.hpp"

namespace rtvc {

class GraphicsPipeline : public vkw::GraphicsPipeline {
public:
  struct CustomArgs {
    const Device &device;
    const Swapchain &swapchain;
    VkFormat depthFormat;
    VkSampleCountFlagBits nSamples;
    const DescriptorSetLayout &desSetLayout;
    const std::string &vertPath;
    const std::string &fragPath;
  };

  using vkw::GraphicsPipeline::GraphicsPipeline;

  GraphicsPipeline(const Device &device, const Swapchain &swapchain,
                   VkFormat depthFormat, VkSampleCountFlagBits nSamples,
                   const DescriptorSetLayout &desSetLayout,
                   const std::string &vertPath, const std::string &fragPath) {
    _initialize({
        device,
        swapchain,
        depthFormat,
        nSamples,
        desSetLayout,
        vertPath,
        fragPath,
    });
  }

  GraphicsPipeline(const CustomArgs &args) { _initialize(args); }

private:
  void _initialize(const CustomArgs &args);
};

} /* namespace rtvc */
#endif /* RTVC_GRAPHICS_PIPELINE_HPP */