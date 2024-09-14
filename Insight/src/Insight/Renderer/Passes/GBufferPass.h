#pragma once

#include "../../Insight.h"
#include "../../Math.h"
#include "../CommandBuffer.h"

namespace Insight
{
    class Scene;
}

namespace Insight::Renderer
{
    class CommandBuffer;
    class DescriptorSet;
    class DescriptorSetLayout;
    class Sampler;
    class ImageView;
    class Image;

    struct GBufferPassExecuteInfo
    {
        CommandBuffer& CommandBuffer;
        Scene& Scene;
        DescriptorSet& SceneDataDescriptorSet;
    };

    class GBufferPass
    {
    public:
        GBufferPass();
        void CreateResources();

        void SetExtent(uvec2 extent)
        {
            m_Extent = extent;
        }

        void Execute(const GBufferPassExecuteInfo& info);
        [[nodiscard]] DescriptorSetLayout& GetSamplerDescriptorSetLayout() const { return *m_SamplerDescriptorSetLayout; }
        [[nodiscard]] Ref<DescriptorSet> GetSamplerDescriptorSet() { return m_SamplerDescriptorSet; }
        [[nodiscard]] Ref<Image> GetDepthImage() { return m_DepthImage; }
        [[nodiscard]] Ref<ImageView> GetDepthView() { return m_DepthImageView; }
        [[nodiscard]] Ref<Image> GetPositionImage() { return m_PositionImage; }
        [[nodiscard]] Ref<ImageView> GetPositionView() { return m_PositionView; }
        [[nodiscard]] Ref<Image> GetNormalImage() { return m_NormalImage; }
        [[nodiscard]] Ref<ImageView> GetNormalView() { return m_NormalView; }
        [[nodiscard]] Ref<Image> GetAlbedoImage() { return m_AlbedoImage; }
        [[nodiscard]] Ref<ImageView> GetAlbedoView() { return m_AlbedoView; }
        [[nodiscard]] Ref<Image> GetOrmImage() { return m_OrmImage; }
        [[nodiscard]] Ref<ImageView> GetOrmView() { return m_OrmView; }

    private:
        [[nodiscard]] vector<RenderingAttachmentInfo> GetAttachments() const;
        [[nodiscard]] RenderingAttachmentInfo GetDepthAttachment() const;


        uvec2 m_Extent{};

        Ref<Image> m_DepthImage;
        Ref<ImageView> m_DepthImageView;

        Ref<Image> m_PositionImage;
        Ref<ImageView> m_PositionView;

        Ref<Image> m_NormalImage;
        Ref<ImageView> m_NormalView;

        Ref<Image> m_AlbedoImage;
        Ref<ImageView> m_AlbedoView;

        Ref<Image> m_OrmImage;
        Ref<ImageView> m_OrmView;

        Ref<Sampler> m_Sampler;

        Unique<DescriptorSetLayout> m_SamplerDescriptorSetLayout;
        Ref<DescriptorSet> m_SamplerDescriptorSet;
    };
}
