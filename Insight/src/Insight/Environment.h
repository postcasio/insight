#pragma once
#include "Assets/StaticMesh.h"
#include "Assets/Texture.h"

namespace Insight {

    struct RenderEnvironmentInfo
    {
        Renderer::CommandBuffer& CommandBuffer;
        Renderer::Image& Target;
        Renderer::ImageView& TargetView;
        Renderer::Image& DepthTarget;
        Renderer::ImageView& DepthTargetView;
        Renderer::DescriptorSet& SceneDataDescriptorSet;
    };

    struct EnvironmentUniform
    {
        alignas(4) int HasEnvironmentMap = 0;
    };

class Environment {
public:
    Environment();
    ~Environment() = default;

    [[nodiscard]] Ref<Texture> GetEnvironmentMap() { return m_EnvironmentMap; }
    [[nodiscard]] Ref<Texture> GetEnvironmentCubemap() { return m_EnvironmentCubemap; }
    [[nodiscard]] Ref<Texture> GetPrefilteredEnvironmentCubemap() { return m_PrefilteredEnvironmentCubemap; }
    [[nodiscard]] Ref<Texture> GetEnvironmentIrradianceMap() { return m_EnvironmentIrradianceMap; }

    void UpdateEnvironmentCubemap();
    void SetEnvironmentMap(const Ref<Texture>& environmentMap);
    void UpdateEnvironmentIrradianceMap();
    void SetEnvironmentCubemap(const Ref<Texture>& environmentCubemap);
    [[nodiscard]] bool HasEnvironmentMap() const { return m_EnvironmentMap != nullptr; }
    void UpdateDescriptorSet();

    void Render(const RenderEnvironmentInfo& info) const;
    [[nodiscard]] EnvironmentUniform& GetEnvironmentUniform() { return m_EnvironmentUniform; }

private:
    Ref<Texture> m_EnvironmentMap;
    Ref<Texture> m_EnvironmentCubemap;
    Ref<Texture> m_PrefilteredEnvironmentCubemap;

    Ref<Renderer::Sampler> m_Sampler;
    Ref<Renderer::DescriptorSet> m_DescriptorSet;
    Ref<StaticMesh> m_SkyboxMesh;
    Unique<Renderer::PipelineLayout> m_PipelineLayout;
    Unique<Renderer::DynamicGraphicsPipeline> m_Pipeline;

    Ref<Renderer::Buffer> m_EnvironmentBuffer;
    EnvironmentUniform m_EnvironmentUniform;
    Ref<Texture> m_EmptyEnvironmentMap;
    Ref<Texture> m_EnvironmentIrradianceMap;
};

} // Insight
