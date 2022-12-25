#ifndef NEKO_RENDERER_RESOURCES_DESCRIPTOR_HPP
#define NEKO_RENDERER_RESOURCES_DESCRIPTOR_HPP

#include "CoreNeko.hpp"

namespace Neko
{

    class Device;

    typedef std::vector<VkDescriptorSet> DescriptorSets;

    class DescriptorSetLayout
    {
    public:
        DescriptorSetLayout() = default;
        DescriptorSetLayout(const DescriptorSetLayout &) = delete;
        DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

        DescriptorSetLayout(const EngineConfigs &crConfig, const Device &crDevice);

        DescriptorSetLayout(DescriptorSetLayout &&rhs) noexcept;

        DescriptorSetLayout &operator=(DescriptorSetLayout &&rhs) noexcept;

        ~DescriptorSetLayout() noexcept { release(); }

        const VkDescriptorSetLayout &operator*() const noexcept { return mDSLayout; }

        void release() noexcept;

    private:
        const Device *mpcDevice = nullptr;
        VkDescriptorSetLayout mDSLayout = VK_NULL_HANDLE;
    };

    class DescriptorPool
    {
    public:
        DescriptorPool() = default;
        DescriptorPool(const DescriptorPool &) = delete;
        DescriptorPool &operator=(const DescriptorPool &) = delete;

        DescriptorPool(const EngineConfigs &crConfig, const Device &crDevice,
                       u32 descriptorSetCount);

        DescriptorPool(DescriptorPool &&rhs) noexcept;

        DescriptorPool &operator=(DescriptorPool &&rhs) noexcept;

        ~DescriptorPool() noexcept { release(); }

        const VkDescriptorPool &operator*() const noexcept { return mDPool; }

        DescriptorSets
        alloc(const std::vector<VkDescriptorSetLayout> &dsLayouts) const;

        void update(u32 dWriteCount, const VkWriteDescriptorSet *pDWrites,
                    u32 dCopyCount, const VkCopyDescriptorSet *pDCopies) const;

        void update(const std::vector<VkWriteDescriptorSet> &dWrites,
                    const std::vector<VkCopyDescriptorSet> &dCopies) const;

        void reset(VkDescriptorPoolResetFlags resetFlags) const;

        void free(const DescriptorSets &dSets) const;

        void free(u32 dSetCount, const VkDescriptorSet *pDSets) const;

        void release() noexcept;

    private:
        const Device *mpcDevice = nullptr;
        VkDescriptorPool mDPool = VK_NULL_HANDLE;
    };

    // class DescriptorSets {
    // public:
    //   DescriptorSets() = default;
    //   DescriptorSets(const DescriptorSets &) = delete;
    //   DescriptorSets &operator=(const DescriptorSets &) = delete;

    //   DescriptorSets(const Configs &crConfig, const Device &crDevice);

    //   DescriptorSets(DescriptorSets &&rhs) noexcept;

    //   DescriptorSets &operator=(DescriptorSets &&rhs) noexcept;

    //   ~DescriptorSets() noexcept { release(); }

    //   const std::vector<VkDescriptorSet> &operator*() const noexcept {
    //     return mDSets;
    //   }

    //   void release() noexcept;

    // private:
    //   std::vector<VkDescriptorSet> mDSets;
    // };

} // namespace Neko

#endif /* NEKO_RENDERER_RESOURCES_DESCRIPTOR_HPP */