#include "FinalCompositorPass.h"

#include <unistd.h>

#include "Context.h"
#include "DescriptorSet.h"
#include "Vertex3D.h"

namespace Insight::Renderer
{
    struct FinalCompositorPushConstants
    {
        alignas(4) u32 EnableFinalRender;
        alignas(4) u32 EnableUi;
    };

    FinalCompositorPass::FinalCompositorPass()
    {
        m_DescriptorSetLayout = DescriptorSetLayout::Create({
            .Name = "Final Compositor Descriptor Set Layout",
            .Bindings = {
                {0, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment},
                {1, vk::DescriptorType::eCombinedImageSampler, 1, vk::ShaderStageFlagBits::eFragment}
            }
        });

        m_DescriptorSet = DescriptorSet::Create({
            .Name = "Final Compositor Descriptor Set",
            .Layout = *m_DescriptorSetLayout
        });

        m_PipelineLayout = PipelineLayout::Create({
            .Name = "Final Compositor Pipeline Layout",
            .PushConstantRanges = {
                {
                    .Stages = vk::ShaderStageFlagBits::eFragment,
                    .Offset = 0,
                    .Size = sizeof(FinalCompositorPushConstants)
                }
            },
            .DescriptorSetLayouts = {
                {
                    .Layout = *m_DescriptorSetLayout
                }
            }
        });

        m_RenderPass = RenderPass::Create({
            .Name = "Final Compositor RenderPass",
            .Attachments = {
                {
                    .format = Context::Instance().GetSwapChain().GetFormat(),
                    .samples = vk::SampleCountFlagBits::e1,
                    .loadOp = vk::AttachmentLoadOp::eDontCare,
                    .storeOp = vk::AttachmentStoreOp::eStore,
                    .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
                    .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
                    .initialLayout = vk::ImageLayout::eUndefined,
                    .finalLayout = vk::ImageLayout::ePresentSrcKHR,
                }
            }
        });

        const auto vertexShader = Shader::Create({
            .Name = "Final Compositor Pass (Vertex Shader)",
            .Path = "Engine/Shaders/FullscreenTriangle.vert.spv",
            .EntryPoint = "main"
        });

        const auto fragmentShader = Shader::Create({
            .Name = "Final Compositor Pass (Fragment Shader)",
            .Path = "Engine/Shaders/FinalCompositorPass.frag.spv",
            .EntryPoint = "main"
        });

        m_Pipeline = GraphicsPipeline::Create({
            .Name = "Final Compositor Pipeline",
            .CullMode = vk::CullModeFlagBits::eNone,
            .PipelineLayout = *m_PipelineLayout,
            .ShaderStages{
                {
                    .Stage = vk::ShaderStageFlagBits::eVertex,
                    .Module = *vertexShader,
                },
                {
                    .Stage = vk::ShaderStageFlagBits::eFragment,
                    .Module = *fragmentShader,
                }
            },
            .DepthTestEnable = false,
            .DepthWriteEnable = false,
            .DepthCompareOp = vk::CompareOp::eAlways,
            .RenderPass = *m_RenderPass,
            .ColorBlendAttachments = {
                {
                    .blendEnable = false,
                    .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
                }
            }
        });

        m_Sampler = Sampler::Create({
            .Name = "Final Compositor Sampler",
            .AddressModeU = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeV = vk::SamplerAddressMode::eClampToEdge,
            .AddressModeW = vk::SamplerAddressMode::eClampToEdge,
        });

        CreateResources();
    }

    void FinalCompositorPass::CreateResources()
    {
        auto internalRenderExtent = Context::Instance().GetInternalRenderExtent();
        auto renderExtent = Context::Instance().GetRenderExtent();
        auto &swapChain = Context::Instance().GetSwapChain();

        m_FinalRender = Image::Create({
            .Name = "Final Render",
            .Extent = {internalRenderExtent, 1},
            .Format = vk::Format::eR16G16B16A16Sfloat,
            .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
        });

        m_FinalRenderView = ImageView::Create({
            .Name = "Final Render View",
            .Image = *m_FinalRender
        });

        m_Ui = Image::Create({
            .Name = "UI",
            .Extent = {renderExtent, 1},
            .Format = vk::Format::eR16G16B16A16Sfloat,
            .Usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled
        });

        m_UiView = ImageView::Create({
            .Name = "UI View",
            .Image = *m_Ui
        });

        m_DescriptorSet->UpdateDescriptorSet({
            {
                {
                    .Type = vk::DescriptorType::eCombinedImageSampler,
                    .Binding = 0,
                    .Data = vector<DescriptorImageInfo>{
                        {
                            .ImageView = *m_FinalRenderView,
                            .Sampler = *m_Sampler
                        },
                    }
                },
                {
                    .Type = vk::DescriptorType::eCombinedImageSampler,
                    .Binding = 1,
                    .Data = vector<DescriptorImageInfo>{
                        {
                            .ImageView = *m_UiView,
                            .Sampler = *m_Sampler
                        }
                    }
                }
            }
        });

        m_Framebuffers.clear();
        m_Framebuffers.reserve(swapChain.GetImageCount());

        for (auto i = 0; i < swapChain.GetImageCount(); i++)
        {
            m_Framebuffers.push_back(Framebuffer::Create({
                .Name = "Final Compositor Framebuffer [" + std::to_string(i) + "]",
                .RenderPass = *m_RenderPass,
                .Attachments = {
                    {
                        .ImageView = *swapChain.GetImageView(i),
                    }
                },
                .Width = renderExtent.x,
                .Height = renderExtent.y,
                .Layers = 1
            }));
        }
    }

    void FinalCompositorPass::DisposeResources()
    {
        m_UiView.reset();
        m_Ui.reset();

        m_FinalRenderView.reset();
        m_FinalRender.reset();
    }

    void FinalCompositorPass::Run(const FinalCompositorPassInfo& info) const
    {
        const auto& swapChain = Context::Instance().GetSwapChain();

        info.CommandBuffer.PipelineBarrier({
            .Image = *m_Ui,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });

        info.CommandBuffer.PipelineBarrier({
            .Image = *m_FinalRender,
            .NewLayout = vk::ImageLayout::eShaderReadOnlyOptimal
        });

        info.CommandBuffer.BeginRenderPass(*m_RenderPass, *m_Framebuffers[swapChain.GetCurrentImageIndex()]);

        info.CommandBuffer.BindPipeline(*m_Pipeline);

        info.CommandBuffer.BindDescriptorSets({
            .Sets = {
                {
                    *m_DescriptorSet
                },
            },
            .PipelineLayout = *m_PipelineLayout
        });

        const FinalCompositorPushConstants pushConstants{
            .EnableFinalRender = info.EnableFinalRender ? 1u : 0u,
            .EnableUi = info.EnableUi ? 1u : 0u
        };

        info.CommandBuffer.PushConstants({
            .PipelineLayout = *m_PipelineLayout,
            .StageFlags = vk::ShaderStageFlagBits::eFragment,
            .Offset = 0,
            .Size = sizeof(FinalCompositorPushConstants),
            .Data = &pushConstants
        });

        info.CommandBuffer.DrawFullscreenTriangle();

        info.CommandBuffer.EndRenderPass();
    }

    void FinalCompositorPass::RenderExtentChanged()
    {
        DisposeResources();
        CreateResources();
    }
}
