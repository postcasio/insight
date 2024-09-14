#include "SceneRenderer.h"

#include "Command.h"
#include "Context.h"
#include "DefaultResources.h"
#include "Vertex3D.h"
#include "../Entity.h"
#include "../Time.h"
#include "../Components/CameraComponent.h"
#include "../Components/DirectionalLightComponent.h"
#include "../Components/EnvironmentComponent.h"
#include "../Components/PointLightComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/StaticMeshComponent.h"

namespace Insight::Renderer
{
    void SceneRenderer::UpdateSceneDataDescriptorSet()
    {
        m_SceneDataDescriptorSet = DescriptorSet::Create({
            .Name = "Scene Data",
            .Layout = Context::Instance().GetSceneDescriptorSetLayout()
        });

        m_SceneDataDescriptorSet->UpdateDescriptorSet({
            {
                {
                    .Type = vk::DescriptorType::eUniformBuffer,
                    .Binding = 0,
                    .Data = vector<DescriptorBufferInfo>{
                        {
                            .Buffer = *m_SceneBuffer,
                            .Offset = 0,
                            .Range = sizeof(SceneUniform)
                        }
                    }
                },
                {
                    .Type = vk::DescriptorType::eCombinedImageSampler,
                    .Binding = 1,
                    .Data = vector<DescriptorImageInfo>{
                        {
                            .ImageView = *m_IrradianceMap->GetCubemapImageView(),
                            .Sampler = *m_IrradianceMap->GetSampler()
                        }
                    }
                },
                {
                    .Type = vk::DescriptorType::eCombinedImageSampler,
                    .Binding = 2,
                    .Data = vector<DescriptorImageInfo>{
                        {
                            .ImageView = *m_PrefilteredEnvironmentMap->GetCubemapImageView(),
                            .Sampler = *m_PrefilteredEnvironmentMap->GetSampler(),
                        }
                    }
                },
                {
                    .Type = vk::DescriptorType::eCombinedImageSampler,
                    .Binding = 3,
                    .Data = vector<DescriptorImageInfo>{
                        {
                            .ImageView = *DefaultResources::GetBrdfLut()->GetImageView(),
                            .Sampler = *DefaultResources::GetBrdfLut()->GetSampler(),
                        }
                    }
                }
            }
        });
    }

    SceneRenderer::SceneRenderer(vk::Format outputFormat) : m_OutputFormat(outputFormat), m_OutputPass({
                                                                .Format = outputFormat,
                                                                .LightingSamplerDescriptorSetLayout = m_LightingPass.
                                                                GetSamplerDescriptorSetLayout(),
            .BloomSamplerDescriptorSetLayout = m_BloomPass.GetSamplerDescriptorSetLayout()
                                                            })
    {
        m_SceneBuffer = Buffer::Create({
            .Name = "Scene",
            .Size = sizeof(SceneUniform),
            .Usage = vk::BufferUsageFlagBits::eUniformBuffer,
        });

        m_IrradianceSampler = Sampler::Create({
            .Name = "Irradiance",
            .MagFilter = vk::Filter::eLinear,
            .MinFilter = vk::Filter::eLinear,
            .MipmapMode = vk::SamplerMipmapMode::eLinear,
            .AddressModeU = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeV = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeW = vk::SamplerAddressMode::eClampToEdge,
        });

        m_IrradianceMap = DefaultResources::GetBlackCubeTexture();
        m_PrefilteredEnvironmentMap = DefaultResources::GetBlackCubeTexture();

        UpdateSceneDataDescriptorSet();
    }

    void SceneRenderer::UploadSceneData(SceneUniform& sceneData) const
    {
        m_SceneBuffer->Upload(std::span(reinterpret_cast<u8*>(&sceneData), sizeof(SceneUniform)));
    }

    void SceneRenderer::CreateResources()
    {
        m_GBufferPass.CreateResources();
        m_LightingPass.CreateResources();
        m_BloomPass.CreateResources();
        m_OutputPass.CreateResources();

        m_ResourcesInitialized = true;
    }

    void SceneRenderer::RenderScene(const RenderSceneInfo& info)
    {
        // -----------------------------------------
        // INITIALIZATION
        // -----------------------------------------

        // Submit resources
        Command::SubmitResource(m_SceneDataDescriptorSet);

        // -----------------------------------------
        // BUILD SCENE
        // -----------------------------------------

        const auto camera = info.Camera.GetComponent<CameraComponent>();
        const auto cameraTransform = info.Camera.GetComponent<TransformComponent>();

        const auto view = glm::inverse(cameraTransform.GetTransform());

        m_SceneData.Projection = camera.Projection;
        m_SceneData.CameraPosition = cameraTransform.Position;
        m_SceneData.NearPlane = camera.NearPlane;
        m_SceneData.FarPlane = camera.FarPlane;
        m_SceneData.View = view;
        m_SceneData.Time = Time::GetFrameTime();

        i32 pointLightIndex = 0;

        info.Scene.GetRegistry().view<PointLightComponent, TransformComponent>().each(
            [this, info, &pointLightIndex](auto entity, PointLightComponent& pointLight,
                                           TransformComponent& transform) mutable
            {
                m_SceneData.PointLights[pointLightIndex].ColorIntensity = {pointLight.Color, pointLight.Intensity};
                m_SceneData.PointLights[pointLightIndex].PositionRange = {transform.Position, pointLight.Range};
                m_SceneData.PointLights[pointLightIndex].CastsShadows = pointLight.CastsShadows ? 1 : 0;

                pointLightIndex++;
            }
        );

        m_SceneData.PointLightCount = pointLightIndex;

        i32 directionalLightIndex = 0;

        info.Scene.GetRegistry().view<DirectionalLightComponent, TransformComponent>().each(
            [this, info, &directionalLightIndex](auto entity, DirectionalLightComponent& directionalLight,
                                                 TransformComponent& transform) mutable
            {
                m_SceneData.DirectionalLights[directionalLightIndex].ColorIntensity = {
                    directionalLight.Color, directionalLight.Intensity
                };
                m_SceneData.DirectionalLights[directionalLightIndex].Direction = {
                    glm::normalize(-transform.Position), 0.0f
                };
                m_SceneData.DirectionalLights[directionalLightIndex].CastsShadows = directionalLight.CastsShadows
                        ? 1
                        : 0;

                directionalLightIndex++;
            }
        );

        m_SceneData.DirectionalLightCount = directionalLightIndex;

        const Entity environmentEntity = {info.Scene.GetRegistry().view<EnvironmentComponent>().front(), &info.Scene};

        if (environmentEntity.IsValid())
        {
            m_SceneData.HasEnvironment = true;

            m_SceneData.PostProcess.Bloom = 1.0f;
            m_SceneData.PostProcess.Exposure = 1.0f;

            UploadSceneData(m_SceneData);

            const auto& environment = environmentEntity.GetComponent<EnvironmentComponent>().Environment;

            auto irradianceMap = environment->GetEnvironmentIrradianceMap();
            auto prefilteredEnvironmentMap = environment->GetPrefilteredEnvironmentCubemap();

            if (irradianceMap != m_IrradianceMap || prefilteredEnvironmentMap != m_PrefilteredEnvironmentMap)
            {
                m_IrradianceMap = irradianceMap;
                m_PrefilteredEnvironmentMap = prefilteredEnvironmentMap;

                UpdateSceneDataDescriptorSet();
            }
        }
        else
        {
            UploadSceneData(m_SceneData);
        }

        Command::SubmitResource(m_IrradianceMap);
        Command::SubmitResource(m_IrradianceSampler);

        // -----------------------------------------
        // G-BUFFER PASS
        // -----------------------------------------

        m_GBufferPass.Execute({
            .CommandBuffer = info.CommandBuffer,
            .Scene = info.Scene,
            .SceneDataDescriptorSet = *m_SceneDataDescriptorSet
        });

        // -----------------------------------------
        // LIGHTING PASS
        // -----------------------------------------

        m_LightingPass.Execute({
            .CommandBuffer = info.CommandBuffer,
            .DepthTarget = *m_GBufferPass.GetDepthImage(),
            .DepthTargetView = *m_GBufferPass.GetDepthView(),
            .Scene = info.Scene,
            .SceneDataDescriptorSet = *m_SceneDataDescriptorSet,
            .GBufferSamplerDescriptorSet = *m_GBufferPass.GetSamplerDescriptorSet(),
        });

        // -----------------------------------------
        // BLOOM PASS
        // -----------------------------------------

        m_BloomPass.Execute({
            .CommandBuffer = info.CommandBuffer,
            .InputSamplerDescriptorSet = *m_LightingPass.GetSamplerDescriptorSet()
        });

        // -----------------------------------------
        // OUTPUT PASS
        // -----------------------------------------

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = info.Target,
            .NewLayout = vk::ImageLayout::eColorAttachmentOptimal
        });

        m_OutputPass.Execute({
            .CommandBuffer = info.CommandBuffer,
            .Target = info.Target,
            .TargetView = info.TargetView,
            .LightingSamplerDescriptorSet = *m_LightingPass.GetSamplerDescriptorSet(),
            .BloomSamplerDescriptorSet = *m_BloomPass.GetSamplerDescriptorSet()
        });

        info.CommandBuffer.PipelineBarrier(ImageBarrier{
            .Image = info.Target,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });
    }

    void SceneRenderer::SetExtent(const uvec2& extent)
    {
        m_Extent = extent;

        m_GBufferPass.SetExtent(extent);
        m_LightingPass.SetExtent(extent);
        m_BloomPass.SetExtent(extent);
        m_OutputPass.SetExtent(extent);
    }
}
