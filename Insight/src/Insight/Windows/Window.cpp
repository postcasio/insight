#include "Window.h"
#include "WindowEvents.h"
#include "../Renderer/Vulkan.h"
#include "../Renderer/Context.h"

namespace Insight {


    void Window::Initialize(const Renderer::Context &context)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_BOOL(m_Resizable));
        // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

        INS_ENGINE_INFO("Creating window: {0} ({1}x{2})", m_Title, m_Width, m_Height);

        m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

        if (!m_Handle)
        {
            INS_ENGINE_ERROR("Failed to create window!");

            throw std::runtime_error("Failed to create window!");
        }

        glfwSetWindowUserPointer(m_Handle, this);
        glfwSetFramebufferSizeCallback(m_Handle, [](GLFWwindow *glfwWindow, int width, int height)
                                       {
            auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));

            WindowResizeEvent event(width, height);
            window->m_EventCallback(event); });

        glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow *glfwWindow)
                                   {
            auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
            WindowCloseEvent event;
            window->m_EventCallback(event); });

        glfwSetCursorPosCallback(m_Handle, [](GLFWwindow *glfwWindow, double xpos, double ypos)
                                 {
            auto window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
            window->m_MousePosition = {xpos, ypos}; });

        glfwGetFramebufferSize(m_Handle, &m_Width, &m_Width);

        if (m_MouseCaptured)
        {
            CaptureMouse();
        }

        // glfwSetCursorPosCallback(m_Window, setCursorPosCallback);
    }

    void Window::CreateSurface(const Renderer::Context &context)
    {
        VK_RAW_ASSERT(glfwCreateWindowSurface(context.GetVkInstance(), m_Handle, nullptr, &m_Surface),
             "Failed to create window surface!");
    }

    void Window::CaptureMouse()
    {
        m_MouseCaptured = true;

        glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glfwRawMouseMotionSupported())
        {
            glfwSetInputMode(m_Handle, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }
    }

    void Window::ReleaseMouse()
    {
        m_MouseCaptured = false;

        glfwSetInputMode(m_Handle, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        glfwSetInputMode(m_Handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Window::Update()
    {
        glfwPollEvents();

        if (ShouldClose())
        {
            Close();
        }
    }

    void Window::Close()
    {
        if (!m_Open)
        {
            return;
        }

        m_Open = false;
    }

    void Window::Dispose() const
    {
        glfwDestroyWindow(m_Handle);
        glfwTerminate();
    }

    void Window::SpinUntilValidSize()
    {
        while (m_Width == 0 || m_Height == 0)
        {
            glfwGetFramebufferSize(m_Handle, &m_Width, &m_Height);
            glfwWaitEvents();
        }
    }


    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Handle);
    }

    bool Window::KeyPressed(const i32 key) const
    {
        return glfwGetKey(m_Handle, key) == GLFW_PRESS;
    }
} // Insight