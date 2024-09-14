#pragma once

#include "CommandBuffer.h"
#include "../Insight.h"
#include "../Scene.h"
#include "../Entity.h"
#include "Passes/BloomPass.h"
#include "Passes/GBufferPass.h"
#include "Passes/LightingPass.h"
#include "Passes/OutputPass.h"

namespace Insight
{
    class Texture;
}

namespace Insight::Renderer
{
    struct SceneRendererInfo
    {
        uvec2 Extent;
        vk::Format Format;
    };

    struct RenderSceneInfo
    {
        CommandBuffer& CommandBuffer;
        Image& Target;
        ImageView& TargetView;
        Scene& Scene;
        Entity Camera;
    };

    struct PointLightUniform
    {
        alignas(16) vec4 PositionRange;
        alignas(16) vec4 ColorIntensity;
        alignas(4) i32 CastsShadows;
    };

    struct DirectionalLightUniform
    {
        alignas(16) vec4 Direction;
        alignas(16) vec4 ColorIntensity;
        alignas(4) i32 CastsShadows;
    };

    struct PostProcessUniform
    {
        float Exposure = 1.0f;
        float Bloom = 1.0f;
    };

    struct SceneUniform
    {
        alignas(16) mat4 Projection;
        alignas(16) mat4 View;
        alignas(16) vec3 CameraPosition;
        alignas(4) f32 NearPlane;
        alignas(4) f32 FarPlane;
        alignas(4) f32 Time;
        alignas(4) u32 HasEnvironment = false;
        PointLightUniform PointLights[16];
        alignas(4) i32 PointLightCount;
        DirectionalLightUniform DirectionalLights[4];
        alignas(4) i32 DirectionalLightCount;
        PostProcessUniform PostProcess;
    };

    class SceneRenderer
    {
    public:
        void UpdateSceneDataDescriptorSet();
        explicit SceneRenderer(vk::Format outputFormat = vk::Format::eR8G8B8A8Unorm);
        void UploadSceneData(SceneUniform& sceneData) const;

        void CreateResources();
        void DisposeResources();


        void RenderScene(const RenderSceneInfo& info);
        [[nodiscard]] DescriptorSet& GetSceneDataDescriptorSet() const { return *m_SceneDataDescriptorSet; }
        void SetExtent(const uvec2& extent);
        SceneUniform& GetSceneData() { return m_SceneData; }

    private:
        bool m_ResourcesInitialized = false;
        uvec2 m_Extent = {0, 0};

        vk::Format m_OutputFormat;

        // SCENE

        Ref<DescriptorSet> m_SceneDataDescriptorSet;
        Ref<Buffer> m_SceneBuffer;
        SceneUniform m_SceneData = {};

        Ref<Texture> m_IrradianceMap;
        Ref<Texture> m_PrefilteredEnvironmentMap;
        Ref<Sampler> m_IrradianceSampler;

        // GBUFFER

        GBufferPass m_GBufferPass{};

        // LIGHTING

        LightingPass m_LightingPass{
            {
                .GBufferSamplerDescriptorSetLayout = m_GBufferPass.GetSamplerDescriptorSetLayout()
            }};

        // BLOOM

        BloomPass m_BloomPass{
            {
                .InputSamplerDescriptorSetLayout = m_LightingPass.GetSamplerDescriptorSetLayout()
            }
        };

        // OUTPUT

        OutputPass m_OutputPass;

        Unique<PipelineLayout> m_OutputPipelineLayout;
        Unique<DynamicGraphicsPipeline> m_OutputPipeline;
        Unique<DescriptorSetLayout> m_OutputDescriptorSetLayout;
        Ref<DescriptorSet> m_OutputDescriptorSet;

    };
}
