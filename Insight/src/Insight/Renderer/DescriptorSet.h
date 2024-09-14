#pragma once
#include "Buffer.h"
#include "DescriptorSetLayout.h"
#include "ImageView.h"
#include "Sampler.h"

namespace Insight::Renderer
{
    struct DescriptorSetInfo
    {
        string Name;
        DescriptorSetLayout& Layout;
    };

    struct DescriptorImageInfo
    {
        ImageView &ImageView;
        Sampler &Sampler;
        vk::ImageLayout ImageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    };

    struct DescriptorBufferInfo
    {
        Buffer &Buffer;
        vk::DeviceSize Offset = 0;
        vk::DeviceSize Range = VK_WHOLE_SIZE;
    };

    struct DescriptorSetWriteInfo
    {
        vk::DescriptorType Type;
        u32 Binding = 0;
        std::variant<vector<DescriptorImageInfo>, vector<DescriptorBufferInfo>> Data;
    };

    struct DescriptorSetUpdateInfo
    {
        vector<DescriptorSetWriteInfo> Writes;
    };

    class DescriptorSet
    {
    public:
        static Ref<DescriptorSet> Create(const DescriptorSetInfo& info)
        {
            return CreateRef<DescriptorSet>(info);
        }

        explicit DescriptorSet(const DescriptorSetInfo& info);
        ~DescriptorSet();

        void UpdateDescriptorSet(const DescriptorSetUpdateInfo& info) const;

        [[nodiscard]] string GetName() const { return m_Name; }
        [[nodiscard]] vk::DescriptorSet GetVkDescriptorSet() const { return m_DescriptorSet; }

    private:
        string m_Name;
        vk::DescriptorSet m_DescriptorSet;
    };
}
