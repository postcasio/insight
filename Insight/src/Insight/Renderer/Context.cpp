#define VMA_IMPLEMENTATION
#include "Context.h"

#include <set>

#include "DebugMarkers.h"
#include "DefaultResources.h"
#include "../Application.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace Insight::Renderer
{
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                  void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            Insight::Log::GetEngineLog().error("Vulkan validation layer error: {}", pCallbackData->pMessage);
            throw std::runtime_error("Vulkan validation layer error!");
            INS_DEBUGBREAK();
        }

        return VK_FALSE;
    }


    static void GLFWErrorCallback(int err, const char* message)
    {
        throw std::runtime_error(fmt::format("GLFW error ({0}): {1}", err, message));
    }

    vk::PhysicalDevice Context::GetPhysicalDevice()
    {
        auto physicalDevices = m_Instance.enumeratePhysicalDevices();

        for (auto& device : physicalDevices)
        {
            if (IsDeviceSuitable(device))
            {
                return device;
            }
        }

        throw std::runtime_error("Failed to find a suitable GPU!");
    }

    bool Context::IsDeviceSuitable(vk::PhysicalDevice device)
    {
        QueueFamilyIndices indices = FindQueueFamilies(device);

        auto supportedFeatures = device.getFeatures();

        bool extensionsSupported = CheckDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.Formats.empty() &&
                !swapChainSupport.PresentModes.empty();
        }

        return indices.IsComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
    }

    Unique<Window> Context::Initialize(const ContextInfo& info)
    {
        auto applicationName = info.ApplicationName;

        INS_ENGINE_INFO("Initializing Vulkan context");

        VULKAN_HPP_DEFAULT_DISPATCHER.init();

        INS_ENGINE_INFO("Initializing GLFW");

        glfwSetErrorCallback(GLFWErrorCallback);

        if (glfwInit() != GLFW_TRUE)
        {
            throw std::runtime_error("Failed to initialize GLFW!");
        }

        if (glfwVulkanSupported() != GLFW_TRUE)
        {
            throw std::runtime_error("Vulkan is not supported on this system!");
        }

        auto window = CreateUnique<Window>(info.WindowInfo);

        window->Initialize(*this);

        auto extensions = GetRequiredExtensions();

        const vk::ApplicationInfo appInfo{
            .pApplicationName = applicationName.c_str(),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName = "Insight",
            .engineVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_3
        };

        vk::InstanceCreateInfo instanceCreateInfo{
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = static_cast<u32>(extensions.size()),
            .ppEnabledExtensionNames = extensions.data(),
        };

#ifdef INS_ENABLE_VALIDATION_LAYERS
        INS_ENGINE_INFO("Enabling validation layers");
        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{
            .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
            .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
            .pfnUserCallback = debugCallback
        };

        instanceCreateInfo.pNext = &debugCreateInfo;
        instanceCreateInfo.enabledLayerCount =
            static_cast<u32>(m_ValidationLayers.size());
        instanceCreateInfo.ppEnabledLayerNames = m_ValidationLayers.data();
#else
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.enabledLayerCount = 0;
        instanceCreateInfo.ppEnabledLayerNames = nullptr;
#endif

        instanceCreateInfo.flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;

        m_Instance = createInstance(instanceCreateInfo);

        VULKAN_HPP_DEFAULT_DISPATCHER.init(m_Instance);

        DebugMarkers::Initialize();

        window->CreateSurface(*this);

        INS_ENGINE_INFO("Initializing surface");

        m_Surface = window->GetSurface();

        m_PhysicalDevice = GetPhysicalDevice();

        m_Device = CreateDevice();

        VULKAN_HPP_DEFAULT_DISPATCHER.init(m_Device);

        VmaAllocatorCreateInfo allocatorInfo{
            .physicalDevice = m_PhysicalDevice,
            .device = m_Device,
            .instance = m_Instance,
            .vulkanApiVersion = VK_API_VERSION_1_3,
        };

        vmaCreateAllocator(&allocatorInfo, &m_Allocator);

        m_GraphicsQueue = m_Device.getQueue(m_QueueFamilies.GraphicsFamily.value(), 0);
        m_PresentQueue = m_Device.getQueue(m_QueueFamilies.PresentFamily.value(), 0);

        m_SwapChain = SwapChain::Create({
            .Format = vk::Format::eR16G16B16A16Sfloat,
            .MaxImageCount = 2,
            .Width = window->GetWidth(),
            .Height = window->GetHeight(),
        });

        Log::GetEngineLog().trace("Created {0} swap chain images ({1}x{2})", m_SwapChain->GetImageCount(),
                                  m_SwapChain->GetWidth(), m_SwapChain->GetHeight());

        INS_ENGINE_INFO("Setting initial render extents:");
        m_RenderExtent = m_SwapChain->GetExtent();
        m_InternalRenderExtent = info.InternalRenderExtent;
        INS_ENGINE_INFO("  Internal: {0}x{1}", m_InternalRenderExtent.x, m_InternalRenderExtent.y);
        INS_ENGINE_INFO("  Output: {0}x{1}", m_RenderExtent.x, m_RenderExtent.y);

        INS_ENGINE_INFO("Creating command pool");

        m_CommandPool = CommandPool::Create();

        m_DescriptorPool = DescriptorPool::Create({
            .Name = "Primary Pool",
            .MaxSets = 100000,
            .PoolSizes{
                {
                    .type = vk::DescriptorType::eUniformBuffer,
                    .descriptorCount = 10000
                },
                {
                    .type = vk::DescriptorType::eStorageBuffer,
                    .descriptorCount = 10000
                },
                {
                    .type = vk::DescriptorType::eCombinedImageSampler,
                    .descriptorCount = 10000
                }
            },
            .Flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet
        });

        m_SynchronizationFrames.resize(m_MaxFramesInFlight);

        m_FinalCompositorPass = FinalCompositorPass::Create();

        m_SceneDescriptorSetLayout = DescriptorSetLayout::Create({
            .Name = "Scene Data Descriptor Set Layout",
            .Bindings = {
                {
                    // Scene data
                    .binding = 0,
                    .descriptorType = vk::DescriptorType::eUniformBuffer,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eVertex,
                },
                {
                    // Irradiance map
                    .binding = 1,
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment,
                },
                {
                    // Prefiltered environment map
                    .binding = 2,
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment,
                },
                {
                    // BRDF LUT
                    .binding = 3,
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment,
                }
            }
        });

        m_EnvironmentDescriptorSetLayout = DescriptorSetLayout::Create({
            .Name = "Environment Descriptor Set Layout",
            .Bindings = {
                {
                    // Environment data
                    .binding = 0,
                    .descriptorType = vk::DescriptorType::eUniformBuffer,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment,
                },
                {
                    // Environment map
                    .binding = 1,
                    .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                    .descriptorCount = 1,
                    .stageFlags = vk::ShaderStageFlagBits::eFragment,
                }
            }
        });

        DefaultResources::Initialize();

        return window;
    }

    void Context::DisposeResources()
    {
        m_FinalCompositorPass.reset();
        m_SynchronizationFrames.clear();
    }

    void Context::Dispose()
    {
        INS_ENGINE_INFO("Disposing rendering context...");
        DefaultResources::Dispose();

        m_SceneDescriptorSetLayout.reset();
        m_EnvironmentDescriptorSetLayout.reset();
        m_DescriptorPool.reset();
        m_CommandPool.reset();
        m_SwapChain.reset();
        m_PresentQueue = nullptr;
        m_GraphicsQueue = nullptr;
        vmaDestroyAllocator(m_Allocator);
        m_Device.destroy();
        m_Instance.destroySurfaceKHR(m_Surface);
        m_PhysicalDevice = nullptr;
        m_Instance.destroy();
    }

    Context& Context::Instance()
    {
        return Application::Instance().GetContext();
    }

    void Context::AcquireNextImage(Semaphore& semaphore)
    {
        auto result = m_SwapChain->AcquireNextImage(semaphore);

        if (result == vk::Result::eErrorOutOfDateKHR)
        {
            INS_ENGINE_WARN("Out of date swap chain image!");

            UpdateRenderExtent();
        }
        else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }
    }

    void Context::WaitIdle() const
    {
        m_Device.waitIdle();
    }

    vector<const char*>& Context::GetRequiredExtensions()
    {
        if (m_RequiredExtensions.empty())
        {
            u32 glfwExtensionCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            m_RequiredExtensions = vector<const char*>(glfwExtensions,
                                                       glfwExtensions + glfwExtensionCount);

#ifdef INS_ENABLE_VALIDATION_LAYERS
            m_RequiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

            m_RequiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            m_RequiredExtensions.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            m_RequiredExtensions.emplace_back("VK_EXT_swapchain_colorspace");
            m_RequiredExtensions.emplace_back("VK_EXT_debug_utils");
        }

        return m_RequiredExtensions;
    }

    QueueFamilyIndices& Context::FindQueueFamilies(vk::PhysicalDevice device)
    {
        u32 queueFamilyCount = 0;
        device.getQueueFamilyProperties(&queueFamilyCount, nullptr);

        vector<vk::QueueFamilyProperties> queueFamilies(queueFamilyCount);
        device.getQueueFamilyProperties(&queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                m_QueueFamilies.GraphicsFamily = i;
            }

            if (vk::Bool32 presentSupport = device.getSurfaceSupportKHR(i, m_Surface))
            {
                m_QueueFamilies.PresentFamily = i;
            }

            if (m_QueueFamilies.IsComplete())
            {
                break;
            }

            i++;
        }

        return m_QueueFamilies;
    }

    SwapChainSupportDetails Context::QuerySwapChainSupport(vk::PhysicalDevice device) const
    {
        return SwapChainSupportDetails{
            .Capabilities = device.getSurfaceCapabilitiesKHR(m_Surface),
            .Formats = device.getSurfaceFormatsKHR(m_Surface),
            .PresentModes = device.getSurfacePresentModesKHR(m_Surface)
        };
    }

    bool Context::CheckDeviceExtensionSupport(vk::PhysicalDevice device) const
    {
        auto availableExtensions = device.enumerateDeviceExtensionProperties(nullptr);

        auto& deviceExtensions = m_DeviceExtensions;

        set<string> requiredExtensions(deviceExtensions.begin(),
                                       deviceExtensions.end());

        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    vk::Device Context::CreateDevice()
    {
        QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

        vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        set<u32> uniqueQueueFamilies{
            indices.GraphicsFamily.value(),
            indices.PresentFamily.value()
        };

        float queuePriority = 1.0f;
        for (u32 queueFamily : uniqueQueueFamilies)
        {
            vk::DeviceQueueCreateInfo queueCreateInfo{
                .queueFamilyIndex = queueFamily,
                .queueCount = 1,
                .pQueuePriorities = &queuePriority
            };

            queueCreateInfos.push_back(queueCreateInfo);
        }

        vk::PhysicalDeviceFeatures deviceFeatures{
            .samplerAnisotropy = VK_TRUE,
            .sampleRateShading = VK_TRUE,
            .shaderSampledImageArrayDynamicIndexing = VK_TRUE
        };

        auto deviceExtensions = m_DeviceExtensions;

        vk::PhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeatures{
            .dynamicRendering = vk::True
        };

        vk::DeviceCreateInfo deviceCreateInfo{
            .queueCreateInfoCount =
            static_cast<u32>(queueCreateInfos.size()),
            .pQueueCreateInfos = queueCreateInfos.data(),
            .pEnabledFeatures = &deviceFeatures,
            .enabledExtensionCount =
            static_cast<u32>(deviceExtensions.size()),
            .ppEnabledExtensionNames = deviceExtensions.data(),
            .enabledLayerCount = 0,
            .pNext = &dynamicRenderingFeatures,
        };

#ifdef INS_ENABLE_VALIDATION_LAYERS
        auto validationLayers = m_ValidationLayers;
        deviceCreateInfo.enabledLayerCount =
            static_cast<u32>(validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
        deviceCreateInfo.enabledLayerCount = 0;
#endif

        return m_PhysicalDevice.createDevice(deviceCreateInfo, nullptr);
    }

    SynchronizationFrame& Context::AcquireNextFrame()
    {
        m_SynchronizationFrames[m_CurrentFrameInFlight].GetInFlightFence().Wait();

        return m_SynchronizationFrames[m_CurrentFrameInFlight];
    }

    void Context::SubmitFrame(const SynchronizationFrame& frame) const
    {
        const auto commandBuffer = frame.GetCommandBuffer().GetVkCommandBuffer();
        const auto renderFinishedSemaphore = frame.GetRenderFinishedSemaphore().GetVkSemaphore();
        const auto imageAvailableSemaphore = frame.GetImageAvailableSemaphore().GetVkSemaphore();

        const vk::PipelineStageFlags mask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

        const vk::SubmitInfo submitInfo{
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffer,
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = &renderFinishedSemaphore,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &imageAvailableSemaphore,
            .pWaitDstStageMask = &mask,
        };

        VK_ASSERT(m_GraphicsQueue.submit(1, &submitInfo, frame.GetInFlightFence().GetVkFence()),
                  "Failed to submit draw command buffer!");
    }

    void Context::PresentFrame(const SynchronizationFrame& frame)
    {
        auto renderFinishedSemaphore = frame.GetRenderFinishedSemaphore().GetVkSemaphore();
        auto swapChain = m_SwapChain->GetVkSwapChain();
        auto imageIndex = m_SwapChain->GetCurrentImageIndex();

        vk::PresentInfoKHR presentInfo{
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &renderFinishedSemaphore,
            .swapchainCount = 1,
            .pSwapchains = &swapChain,
            .pImageIndices = &imageIndex
        };

        auto result = m_PresentQueue.presentKHR(presentInfo);

        if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR ||
            m_RenderExtentChanged)
        {
            m_RenderExtentChanged = false;

            Application::Instance().GetWindow().SpinUntilValidSize();

            WaitIdle();

            UpdateRenderExtent();
        }
        else if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        m_CurrentFrameInFlight = (m_CurrentFrameInFlight + 1) % m_MaxFramesInFlight;
    }

    void Context::SubmitImmediateCommands(const CommandBuffer& commandBuffer) const
    {
        const auto commandBufferHandle = commandBuffer.GetVkCommandBuffer();

        const vk::SubmitInfo submitInfo = {
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBufferHandle
        };

        VK_ASSERT(m_GraphicsQueue.submit(1, &submitInfo, VK_NULL_HANDLE), "failed to submit to queue!");

        WaitIdle();
    }

    vk::PresentModeKHR Context::GetPresentMode(
        const vector<vk::PresentModeKHR>& availablePresentModes)
    {
        for (const auto& availablePresentMode : availablePresentModes)
        {
            if (availablePresentMode == vk::PresentModeKHR::eMailbox)
            {
                return availablePresentMode;
            }
        }

        return vk::PresentModeKHR::eFifo;
    }

    void Context::UpdateRenderExtent()
    {
        auto& window = Application::Instance().GetWindow();

        INS_ENGINE_INFO("Updating extent {0}x{1}", m_RenderExtent.x, m_RenderExtent.y);

        m_SwapChain->RenderExtentChanged();

        m_RenderExtent = m_SwapChain->GetExtent();

        m_FinalCompositorPass->RenderExtentChanged();

        Application::Instance().GetUiContext().RenderExtentChanged();

        m_RenderExtentChanged = false;
    }
}
