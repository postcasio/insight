#pragma once

#include "../Insight.h"
#include "CommandPool.h"
#include "DescriptorPool.h"
#include "FinalCompositorPass.h"
#include "SynchronizationFrame.h"

#include "SwapChain.h"
#include "SwapChainSupport.h"
#include "../Windows/Window.h"

namespace Insight
{
    class Window;
}

namespace Insight::Renderer {
    struct QueueFamilyIndices
    {
        optional<u32> GraphicsFamily;
        optional<u32> PresentFamily;

        [[nodiscard]] bool IsComplete() const
        {
            return GraphicsFamily.has_value() && PresentFamily.has_value();
        }
    };

    struct ContextInfo
    {
        string ApplicationName;
        uvec2 InternalRenderExtent;
        Insight::WindowInfo WindowInfo;

        ContextInfo(string applicationName, uvec2 internalRenderExtent, Insight::WindowInfo windowInfo) :
            ApplicationName(std::move(applicationName)),
            InternalRenderExtent(internalRenderExtent),
            WindowInfo(std::move(windowInfo))
        {
        }
    };

class Context {
public:
    Unique<Window> Initialize(const ContextInfo &info);
    void DisposeResources();
    void Dispose();

    [[nodiscard]] const vk::Instance &GetVkInstance() const { return m_Instance; }
    [[nodiscard]] const vk::PhysicalDevice &GetVkPhysicalDevice() const { return m_PhysicalDevice; }
    [[nodiscard]] const vk::Device &GetVkDevice() const { return m_Device; }
    [[nodiscard]] const vk::Queue &GetVkGraphicsQueue() const { return m_GraphicsQueue; }
    [[nodiscard]] const vk::Queue &GetVkPresentQueue() const { return m_PresentQueue; }
    [[nodiscard]] const vk::SurfaceKHR &GetVkSurface() const { return m_Surface; }
    [[nodiscard]] const QueueFamilyIndices &GetQueueFamilies() const { return m_QueueFamilies; }
    [[nodiscard]] VmaAllocator GetAllocator() const { return m_Allocator; }
    [[nodiscard]] const SwapChain &GetSwapChain() const { return *m_SwapChain; }
    [[nodiscard]] const CommandPool &GetCommandPool() const { return *m_CommandPool; }
    [[nodiscard]] const DescriptorPool &GetDescriptorPool() const { return *m_DescriptorPool; }
    [[nodiscard]] const FinalCompositorPass &GetFinalCompositorPass() const { return *m_FinalCompositorPass; }
    [[nodiscard]] const vk::DynamicLoader &GetLoader() const { return *m_Loader; }

    SynchronizationFrame& AcquireNextFrame();
    SynchronizationFrame& GetCurrentFrame()
    {
        return m_SynchronizationFrames[m_CurrentFrameInFlight];
    }

    void SubmitFrame(const SynchronizationFrame& frame) const;
    void PresentFrame(const SynchronizationFrame& frame);
    void SubmitImmediateCommands(const CommandBuffer& commandBuffer) const;
    vk::Format GetOutputFormat() { return vk::Format::eR16G16B16A16Sfloat; }
    vk::Format GetDepthFormat() { return vk::Format::eD32Sfloat; }

    static vk::PresentModeKHR GetPresentMode(
        const vector<vk::PresentModeKHR> &availablePresentModes);
    void UpdateRenderExtent();

    static Context &Instance();

    [[nodiscard]] uvec2 GetInternalRenderExtent() const { return m_InternalRenderExtent; }
    [[nodiscard]] uvec2 GetRenderExtent() const { return m_RenderExtent; }

    [[nodiscard]] u32 GetMaxFramesInFlight() const { return m_MaxFramesInFlight; }
    [[nodiscard]] u32 GetCurrentFrameInFlight() const { return m_CurrentFrameInFlight; }
    [[nodiscard]] SynchronizationFrame &GetCurrentSynchronizationFrame() { return m_SynchronizationFrames[m_CurrentFrameInFlight]; }
    [[nodiscard]] DescriptorSetLayout &GetSceneDescriptorSetLayout() const { return *m_SceneDescriptorSetLayout; }
    [[nodiscard]] DescriptorSetLayout& GetEnvironmentDescriptorSetLayout() const { return *m_EnvironmentDescriptorSetLayout; }

    void AcquireNextImage(Semaphore& semaphore);
    void WaitIdle() const;

private:
    uvec2 m_InternalRenderExtent;
    uvec2 m_RenderExtent;

    vk::Instance m_Instance;
    vk::PhysicalDevice m_PhysicalDevice;
    vk::Device m_Device;
    vk::Queue m_GraphicsQueue;
    vk::Queue m_PresentQueue;
    vk::SurfaceKHR m_Surface;
    const vector<const char *> m_ValidationLayers = vector<const char*>({"VK_LAYER_KHRONOS_validation"});
    const vector<const char *> m_DeviceExtensions = vector<const char*>({
        "VK_KHR_swapchain",
        "VK_KHR_portability_subset",
        "VK_EXT_shader_viewport_index_layer",
        "VK_KHR_dynamic_rendering"
    });
    vector<const char *> m_RequiredExtensions;
    VkDebugUtilsMessengerEXT m_DebugMessenger = nullptr;
    QueueFamilyIndices m_QueueFamilies{};
    VmaAllocator m_Allocator = nullptr;
    Unique<SwapChain> m_SwapChain;
    Unique<CommandPool> m_CommandPool;
    Unique<DescriptorPool> m_DescriptorPool;
    Unique<FinalCompositorPass> m_FinalCompositorPass;
    Unique<DescriptorSetLayout> m_SceneDescriptorSetLayout;
    Unique<DescriptorSetLayout> m_EnvironmentDescriptorSetLayout;

    vector<SynchronizationFrame> m_SynchronizationFrames {};
    u32 m_CurrentFrameInFlight = 0;
    u32 m_MaxFramesInFlight = 2;

    bool m_RenderExtentChanged = false;

    Unique<vk::DynamicLoader> m_Loader;
    PFN_vkCmdBeginRendering m_CmdBeginRendering = nullptr;
    PFN_vkCmdEndRendering m_CmdEndRendering = nullptr;

    vector<const char *> &GetRequiredExtensions();
    vk::PhysicalDevice GetPhysicalDevice();
    bool IsDeviceSuitable(vk::PhysicalDevice device);
    QueueFamilyIndices &FindQueueFamilies(vk::PhysicalDevice device);
    [[nodiscard]] SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice device) const;
    [[nodiscard]] bool CheckDeviceExtensionSupport(vk::PhysicalDevice device) const;
    vk::Device CreateDevice();

    friend class SwapChain;
    friend class CommandBuffer;

};

}
