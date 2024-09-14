#pragma once

#include "../Insight.h"
#include "Vulkan.h"
#include "CommandBuffer.h"
#include "DescriptorSet.h"
#include "GraphicsPipeline.h"
#include "Image.h"
#include "ImageView.h"
#include "PipelineLayout.h"
#include "Sampler.h"

namespace Insight::Renderer {

struct FinalCompositorPassInfo {
    CommandBuffer &CommandBuffer;
    bool EnableFinalRender;
    bool EnableUi;
};

class FinalCompositorPass {
public:
    static Unique<FinalCompositorPass> Create()
    {
        return CreateUnique<FinalCompositorPass>();
    }

    FinalCompositorPass();
    ~FinalCompositorPass() = default;

    void Run(const FinalCompositorPassInfo& info) const;

    [[nodiscard]] Image &GetFinalRender() const { return *m_FinalRender; }
    [[nodiscard]] ImageView &GetFinalRenderView() const { return *m_FinalRenderView; }

    [[nodiscard]] Image &GetUi() const { return *m_Ui; }
    [[nodiscard]] ImageView &GetUiView() const { return *m_UiView; }

    void RenderExtentChanged();

private:
    Unique<PipelineLayout> m_PipelineLayout;
    Unique<GraphicsPipeline> m_Pipeline;
    Unique<RenderPass> m_RenderPass;
    vector<Unique<Framebuffer>> m_Framebuffers;

    Ref<Image> m_FinalRender;
    Ref<ImageView> m_FinalRenderView;

    Ref<Image> m_Ui;
    Ref<ImageView> m_UiView;

    Unique<DescriptorSetLayout> m_DescriptorSetLayout;
    Ref<DescriptorSet> m_DescriptorSet;

    Ref<Sampler> m_Sampler;

    void CreateResources();
    void DisposeResources();

};

}
