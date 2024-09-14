#include "Sampler.h"

#include "Context.h"
#include "DebugMarkers.h"

namespace Insight::Renderer
{
    Sampler::Sampler(const SamplerInfo& info) : m_Name(info.Name)
    {
        const vk::SamplerCreateInfo samplerCreateInfo{
            .flags = info.Flags,
            .magFilter = info.MagFilter,
            .minFilter = info.MinFilter,
            .mipmapMode = info.MipmapMode,
            .addressModeU = info.AddressModeU,
            .addressModeV = info.AddressModeV,
            .addressModeW = info.AddressModeW,
            .mipLodBias = info.MipLodBias,
            .anisotropyEnable = info.AnisotropyEnabled,
            .maxAnisotropy = info.MaxAnisotropy,
            .compareEnable = info.CompareEnable,
            .compareOp = info.CompareOp,
            .minLod = info.MinLod,
            .maxLod = info.MaxLod,
            .borderColor = info.BorderColor,
            .unnormalizedCoordinates = info.UnnormalizedCoordinates,
        };

        m_VkSampler = Context::Instance().GetVkDevice().createSampler(samplerCreateInfo);

        DebugMarkers::MarkSampler(m_VkSampler, m_Name);
    }

    Sampler::~Sampler()
    {
        Context::Instance().GetVkDevice().destroySampler(m_VkSampler);
    }
} // Renderer
// Insight
