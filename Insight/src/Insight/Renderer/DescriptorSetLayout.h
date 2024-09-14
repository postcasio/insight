#pragma once

#include "Vulkan.h"

namespace Insight::Renderer
{
    struct DescriptorSetLayoutInfo
    {
        string Name;
        vector<vk::DescriptorSetLayoutBinding> Bindings;
    };

    class DescriptorSetLayout
    {
    public:
        static Unique<DescriptorSetLayout> Create(const DescriptorSetLayoutInfo& info)
        {
            return CreateUnique<DescriptorSetLayout>(info);
        }

        explicit DescriptorSetLayout(const DescriptorSetLayoutInfo& info);
        ~DescriptorSetLayout();

        [[nodiscard]] const vk::DescriptorSetLayout& GetVkDescriptorSetLayout() const
        {
            return m_VkDescriptorSetLayout;
        }

        [[nodiscard]] string GetName() const { return m_Name; }

    private:
        string m_Name;
        vector<vk::DescriptorSetLayoutBinding> m_Bindings;
        vk::DescriptorSetLayout m_VkDescriptorSetLayout;
    };
}
