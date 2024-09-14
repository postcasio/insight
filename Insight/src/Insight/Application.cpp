#include "Application.h"

#include "Time.h"
#include "Renderer/Command.h"
#include "Renderer/Context.h"

namespace Insight
{
    void Application::Initialize()
    {
        Time::Initialize();
        Log::Initialize();

        m_Window = m_Context.Initialize({
            m_Name,
            { 1280, 720 },
            {
                .Width = 1280,
                .Height = 720,
                .Resizable = true,
                .EventCallback = [](Event &event) {},
                .Title = Application::Instance().GetName(),
                .CaptureMouse = false
            }
        });

        m_UiContext.Initialize();

        Renderer::Command::Initialize();
    }

    void Application::OnFrame()
    {
        float delta = Time::Update();

        Update(delta);

        Render();
    }

    void Application::Update(float delta)
    {
        m_Window->Update();

        m_UiContext.Update();

        for (auto &controller : m_Controllers)
        {
            controller->OnUpdate(delta);
        }
    }

    void Application::RenderUi(Renderer::CommandBuffer& commandBuffer)
    {
        for (auto &controller : m_Controllers)
        {
            controller->OnRenderUi();
        }

        m_UiContext.Render(commandBuffer);
    }

    void Application::Render()
    {
        auto &commandBuffer = Renderer::Command::BeginFrame();

        RenderUi(commandBuffer);

        Renderer::Command::CompositeFrame(false, true);

        Renderer::Command::EndFrame();
    }

    void Application::Dispose()
    {
        ClearControllers();

        Renderer::Command::Dispose();

        m_Context.DisposeResources();

        m_UiContext.Dispose();

        m_Context.Dispose();

        m_Window->Dispose();

        m_Window.reset();
    }

    void Application::Run()
    {
        Initialize();

        while (m_Window->IsOpen())
        {
            OnFrame();
        }

        m_Context.WaitIdle();

        Dispose();
    }
} // Insight