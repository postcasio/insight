#include "Context.h"
#include "ImGui.h"
#include "ImNodes.h"
#include "../Application.h"
#include "../Renderer/Context.h"

#include "FontAwesome6.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/CommandBuffer.h"
#include "Utils.h"

namespace Insight::Ui
{

    void Context::CreateResources()
    {
        const auto renderExtent = Renderer::Context::Instance().GetRenderExtent();
        const auto& finalCompositorPass = Renderer::Context::Instance().GetFinalCompositorPass();
        const auto& swapChain = Renderer::Context::Instance().GetSwapChain();

        m_RenderPass = Renderer::RenderPass::Create({
            .Name = "ImGUI RenderPass",
            .Attachments = {
                {
                    .format = Renderer::Context::Instance().GetFinalCompositorPass().GetUiView().GetFormat(),
                    .samples = vk::SampleCountFlagBits::e1,
                    .loadOp = vk::AttachmentLoadOp::eClear,
                    .storeOp = vk::AttachmentStoreOp::eStore,
                    .stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
                    .stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
                    .initialLayout = vk::ImageLayout::eUndefined,
                    .finalLayout = vk::ImageLayout::ePresentSrcKHR,
                }
            }
        });

        m_Framebuffers.clear();
        m_Framebuffers.reserve(swapChain.GetImageCount());

        for (auto i = 0; i < swapChain.GetImageCount(); i++)
        {
            m_Framebuffers.push_back(Renderer::Framebuffer::Create({
                .Name = "ImGUI Framebuffer",
                .RenderPass = *m_RenderPass,
                // .Attachments = {{*swapChain.GetImageView(i)}},
                .Attachments = {{Renderer::Context::Instance().GetFinalCompositorPass().GetUiView()}},
                .Width = renderExtent.x,
                .Height = renderExtent.y,
                .Layers = 1,
            }));
        }
    }

    void Context::SetFont()
    {
        ImGuiIO &io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("Engine/Fonts/Roboto/Roboto-Regular.ttf", 14.0f);

        ImFontConfig config;
        config.MergeMode = true;
        config.GlyphMinAdvanceX = 13.0f; // Use if you want to make the icon monospaced
        static constexpr ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
        io.Fonts->AddFontFromFileTTF("Engine/Fonts/FontAwesome6/fa-solid-900.ttf", 14.0f, &config, icon_ranges);

    }

    void Context::Initialize()
    {
        INS_ENGINE_INFO("Initializing UI context");

        m_DescriptorPool = Renderer::DescriptorPool::Create({
            .Name = "ImGUI Descriptor Pool",
            .Flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet | vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind,
            .MaxSets = 100000,
            .PoolSizes = {
                {
                    .type = vk::DescriptorType::eUniformBuffer,
                    .descriptorCount = 100000,
                },
                {
                    .type = vk::DescriptorType::eCombinedImageSampler,
                    .descriptorCount = 100000,
                }
            }
        });

        CreateResources();

        INS_ENGINE_INFO("Initializing ImGUI");

        ImGui::CreateContext();
        ImNodes::CreateContext();

        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        GLFWwindow* handle = Application::Instance().GetWindow().GetHandle();

        ImGui_ImplGlfw_InitForVulkan(handle, true);

        auto& rendererContext = Renderer::Context::Instance();

        auto& swapChain = rendererContext.GetSwapChain();

        ImGui_ImplVulkan_InitInfo initInfo = {
            .Instance = rendererContext.GetVkInstance(),
            .PhysicalDevice = rendererContext.GetVkPhysicalDevice(),
            .Device = rendererContext.GetVkDevice(),
            .Queue = rendererContext.GetVkGraphicsQueue(),
            .RenderPass = m_RenderPass->GetVkRenderPass(),
            .DescriptorPool = m_DescriptorPool->GetVkDescriptorPool(),
            .MinImageCount = swapChain.GetImageCount(),
            .ImageCount = swapChain.GetImageCount(),
            .MSAASamples = VK_SAMPLE_COUNT_1_BIT
        };

        ImGui_ImplVulkan_Init(&initInfo);

        SetStyle();

        SetFont();

        INS_ENGINE_INFO("UI initialized");
    }

    void Context::Update()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void Context::Render(const Renderer::CommandBuffer& commandBuffer) const
    {
        commandBuffer.BeginRenderPass(*m_RenderPass,
                                      *m_Framebuffers[Renderer::Context::Instance().GetSwapChain().
                                          GetCurrentImageIndex()]);

        ImGui::Render();

        ImDrawData* drawData = ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer.GetVkCommandBuffer());

        commandBuffer.EndRenderPass();
    }

    void Context::Dispose()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();

        ImNodes::DestroyContext();
        ImGui::DestroyContext();

        DisposeResources();

        m_DescriptorPool.reset();
    }

    void Context::DisposeResources()
    {
        m_Framebuffers.clear();
        m_RenderPass.reset();
    }

    void Context::RenderExtentChanged()
    {
        DisposeResources();
        CreateResources();
    }

    void Context::SetStyle()
    {
        // Set default style
        ImGui::StyleColorsDark();

        auto& style = ImGui::GetStyle();
        auto colors = style.Colors;

        style.FrameRounding = 3;

        colors[ImGuiCol_WindowBg] = ImVec4(0.01f, 0.01f, 0.03f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.31f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.01f, 0.02f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.04f, 0.05f, 0.10f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.07f, 0.09f, 0.17f, 0.79f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.17f, 0.22f, 0.94f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.26f, 0.80f, 0.49f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.26f, 0.80f, 0.49f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.26f, 0.80f, 0.77f);
        colors[ImGuiCol_Button] = ImVec4(0.00f, 0.26f, 0.80f, 0.49f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.26f, 0.80f, 0.82f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.26f, 0.80f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(1.00f, 1.00f, 1.00f, 0.02f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.09f);
        colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.14f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.26f, 0.80f, 0.49f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.26f, 0.80f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.26f, 0.80f, 0.49f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 0.26f, 0.80f, 0.75f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.26f, 0.80f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.00f, 0.26f, 0.80f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.17f, 0.17f, 0.17f, 0.49f);
        colors[ImGuiCol_TabSelected] = ImVec4(0.00f, 0.26f, 0.80f, 0.74f);
        colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.00f, 0.26f, 0.80f, 0.00f);
        colors[ImGuiCol_TabDimmed] = ImVec4(0.17f, 0.17f, 0.17f, 0.49f);
        colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.00f, 0.26f, 0.80f, 0.25f);
        colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.00f, 0.26f, 0.80f, 0.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 0.26f, 0.80f, 0.49f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.17f, 0.22f, 0.51f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 0.26f, 0.80f, 0.74f);

        ImNodesStyle& nodesStyle = ImNodes::GetStyle();
        nodesStyle.Colors[ImNodesCol_TitleBar] = vec2int(colors[ImGuiCol_TabUnfocusedActive]);
        nodesStyle.Colors[ImNodesCol_TitleBarHovered] = vec2int(colors[ImGuiCol_TabHovered]);
        nodesStyle.Colors[ImNodesCol_TitleBarSelected] = vec2int(colors[ImGuiCol_TabSelected]);
        nodesStyle.Colors[ImNodesCol_NodeBackground] = vec2int(colors[ImGuiCol_WindowBg]);
        nodesStyle.Colors[ImNodesCol_NodeBackgroundHovered] = vec2int(colors[ImGuiCol_WindowBg]);
        nodesStyle.Colors[ImNodesCol_NodeBackgroundSelected] = vec2int(colors[ImGuiCol_WindowBg]);
        nodesStyle.Colors[ImNodesCol_Link] = vec2int(colors[ImGuiCol_Text]);
    }

    ImTextureID Context::CreateDescriptorSet(const Renderer::Sampler& sampler, const Renderer::ImageView& imageView)
    {
        return ImGui_ImplVulkan_AddTexture(sampler.GetVkSampler(), imageView.GetVkImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    void Context::DestroyDescriptorSet(const ImTextureID textureId)
    {
        ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(textureId));
    }
}
