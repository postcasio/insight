#pragma once

#include <functional>

#include "../Insight.h"
#include "../Renderer/Vulkan.h"
#include "../Math.h"
#include "../Events/Event.h"

#define GLFW_BOOL(x) ((x) ? GLFW_TRUE : GLFW_FALSE)

namespace Insight {

    namespace Renderer
    {
        class Context;
    }

    struct WindowInfo
    {
        u32 Width;
        u32 Height;
        bool Resizable;
        EventCallback EventCallback;
        string Title;
        bool CaptureMouse;
    };

class Window {
public:
    explicit Window(const WindowInfo &info) :
        m_Width(info.Width),
        m_Height(info.Height),
        m_Resizable(info.Resizable),
        m_EventCallback(info.EventCallback),
        m_Title(info.Title),
        m_MouseCaptured(info.CaptureMouse)
    {
    }

    ~Window() { Close(); }

    void Initialize(const Renderer::Context &context);
    void CreateSurface(const Renderer::Context &context);
    void SpinUntilValidSize();

    [[nodiscard]] bool IsMinimized() const
    {
        return m_Minimized;
    }

    void CaptureMouse();
    void ReleaseMouse();
    [[nodiscard]] bool IsMouseCaptured() const { return m_MouseCaptured; }
    void Update();
    void Close();
    void Dispose() const;
    [[nodiscard]] bool ShouldClose() const;
    [[nodiscard]] bool KeyPressed(i32 key) const;

    [[nodiscard]] VkSurfaceKHR GetSurface() const
    {
        return m_Surface;
    }

    [[nodiscard]] uvec2 GetExtent() const
    {
        return {m_Width, m_Height};
    }

    [[nodiscard]] u32 GetWidth() const
    {
        return m_Width;
    }

    [[nodiscard]] u32 GetHeight() const
    {
        return m_Height;
    }

    [[nodiscard]] GLFWwindow *GetHandle() const
    {
        return m_Handle;
    }

    [[nodiscard]] bool IsOpen() const
    {
        return m_Open;
    }

    [[nodiscard]] vec2 GetMousePosition() const
    {
        return m_MousePosition;
    }
protected:
private:
    bool m_Open = true;
    bool m_Minimized = false;
    i32 m_Width;
    i32 m_Height;
    bool m_Resizable;
    std::function<void(Event &)> m_EventCallback;
    string m_Title;
    bool m_MouseCaptured;
    GLFWwindow *m_Handle = nullptr;
    VkSurfaceKHR m_Surface = nullptr;
    static u32 s_WindowCount;
    vec2 m_MousePosition = {0, 0};


};

} // Insight
