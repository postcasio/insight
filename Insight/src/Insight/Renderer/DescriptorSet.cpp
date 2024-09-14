#include "DescriptorSet.h"

#include "Context.h"


namespace Insight::Renderer
{
    DescriptorSet::DescriptorSet(const DescriptorSetInfo& info) : m_Name(info.Name)
    {
        vector<vk::DescriptorSetLayout> layouts = {info.Layout.GetVkDescriptorSetLayout()};

        const vk::DescriptorSetAllocateInfo allocateInfo = {
            .descriptorPool = Context::Instance().GetDescriptorPool().GetVkDescriptorPool(),
            .descriptorSetCount = static_cast<u32>(layouts.size()),
            .pSetLayouts = layouts.data()
        };

        m_DescriptorSet = Context::Instance().GetVkDevice().allocateDescriptorSets(allocateInfo)[0];
    }

    DescriptorSet::~DescriptorSet()
    {
        Context::Instance().GetVkDevice().freeDescriptorSets(
            Context::Instance().GetDescriptorPool().GetVkDescriptorPool(), m_DescriptorSet);
    }

    void DescriptorSet::UpdateDescriptorSet(const DescriptorSetUpdateInfo& info) const
    {
        vector<vk::WriteDescriptorSet> writes;
        vector<vector<vk::DescriptorImageInfo>*> imageInfos;
        vector<vector<vk::DescriptorBufferInfo>*> bufferInfos;

        imageInfos.reserve(info.Writes.size());
        bufferInfos.reserve(info.Writes.size());

        for (const auto& write : info.Writes)
        {

            auto writeImageInfos = new vector<vk::DescriptorImageInfo>();
            imageInfos.push_back(writeImageInfos);
            auto writeBufferInfos = new vector<vk::DescriptorBufferInfo>();
            bufferInfos.push_back(writeBufferInfos);

            switch (write.Type)
            {
            case vk::DescriptorType::eCombinedImageSampler:
                {
                    const auto& data = std::get<vector<DescriptorImageInfo>>(write.Data);
                    writeImageInfos->reserve(data.size());
                    for (const auto& image : data)
                    {
                        writeImageInfos->push_back({
                            .imageView = image.ImageView.GetVkImageView(),
                            .sampler = image.Sampler.GetVkSampler(),
                            .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal,
                        });
                    }

                    writes.push_back({
                        .dstSet = m_DescriptorSet,
                        .dstBinding = write.Binding,
                        .dstArrayElement = 0,
                        .descriptorType = write.Type,
                        .descriptorCount = static_cast<u32>(writeImageInfos->size()),
                        .pImageInfo = writeImageInfos->data()
                    });
                    break;
                }
            case vk::DescriptorType::eUniformBuffer:
                {
                    const auto& data = std::get<vector<DescriptorBufferInfo>>(write.Data);
                    writeBufferInfos->reserve(data.size());
                    for (const auto& buffer : data)
                    {
                        writeBufferInfos->push_back({
                            .buffer = buffer.Buffer.GetVkBuffer(),
                            .offset = buffer.Offset,
                            .range = buffer.Range
                        });
                    }

                    writes.push_back({
                        .dstSet = m_DescriptorSet,
                        .dstBinding = write.Binding,
                        .dstArrayElement = 0,
                        .descriptorType = write.Type,
                        .descriptorCount = static_cast<u32>(writeBufferInfos->size()),
                        .pBufferInfo = writeBufferInfos->data()
                    });
                    break;
                }
            default: break;
            }
        }

        Context::Instance().GetVkDevice().updateDescriptorSets(writes, {});

        for (auto imageInfo : imageInfos)
        {
            delete imageInfo;
        }

        for (auto bufferInfo : bufferInfos)
        {
            delete bufferInfo;
        }
    }
}
