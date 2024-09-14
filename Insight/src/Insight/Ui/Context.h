#pragma once

#include "../Insight.h"
#include "../Renderer/CommandBuffer.h"
#include "../Renderer/DescriptorPool.h"
#include "../Renderer/DescriptorSet.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/Image.h"
#include "../Renderer/Framebuffer.h"
#include "../Renderer/ImageView.h"
#include "../Renderer/Sampler.h"


namespace Insight::Renderer
{
    class RenderPass;
}

namespace Insight::Ui
{
    class Context
    {
    public:
        Context() = default;

        static void SetFont();
        void Initialize();
        void Update();
        void Render(const Renderer::CommandBuffer& commandBuffer) const;
        void Dispose();
        void RenderExtentChanged();

        static void SetStyle();

        static ImTextureID CreateDescriptorSet(const Renderer::Sampler& sampler, const Renderer::ImageView& imageView);
        static void DestroyDescriptorSet(ImTextureID textureId);

    private:
        Unique<Renderer::DescriptorPool> m_DescriptorPool = nullptr;
        Unique<Renderer::RenderPass> m_RenderPass = nullptr;
        vector<Unique<Renderer::Framebuffer>> m_Framebuffers{};

        void CreateResources();
        void DisposeResources();
    };
}
