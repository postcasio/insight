#include "DescriptorSetLayout.h"

#include "Context.h"
#include "DebugMarkers.h"

namespace Insight::Renderer
{
    DescriptorSetLayout::DescriptorSetLayout(const DescriptorSetLayoutInfo& info) : m_Name(info.Name),
        m_Bindings(info.Bindings)
    {
        const vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{
            .bindingCount = static_cast<u32>(m_Bindings.size()),
            .pBindings = m_Bindings.data(),
        };

        m_VkDescriptorSetLayout = Context::Instance().GetVkDevice().createDescriptorSetLayout(
            descriptorSetLayoutCreateInfo);

        DebugMarkers::MarkDescriptorSetLayout(m_VkDescriptorSetLayout, m_Name);
    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {
        Context::Instance().GetVkDevice().destroyDescriptorSetLayout(m_VkDescriptorSetLayout);
    }
}
