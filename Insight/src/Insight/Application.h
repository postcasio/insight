#pragma once

#include "Controller.h"
#include "Insight.h"
#include "Windows/Window.h"
#include "Renderer/Context.h"
#include "Ui/Context.h"

namespace Insight
{
    namespace Renderer
    {
        class Context;
    }

    struct ApplicationInfo
    {
        string Name;
        vector<string> Arguments;
    };

    class Application
    {
    public:
        virtual ~Application() = default;
        Ui::Context &GetUiContext()
        {
            return m_UiContext;
        }

        template <Derived<Application> T>
        static T &Create(const ApplicationInfo &info)
        {
            s_Instance = static_cast<Unique<Application>>(CreateUnique<T>(info));

            return static_cast<T&>(*s_Instance);
        }

        [[nodiscard]] string GetName() const { return m_Name; }

        [[nodiscard]] Window &GetWindow() { return *m_Window; }
        [[nodiscard]] const Window &GetWindow() const { return *m_Window; }

        [[nodiscard]] Renderer::Context &GetContext() { return m_Context; }
        [[nodiscard]] const Renderer::Context &GetContext() const { return m_Context; }

        void Run();

        static Application &Instance() { return *s_Instance; }

        template<Derived<Application> T>
        static T &Instance() { return static_cast<T&>(*s_Instance); }

        void AddController(Controller* controller)
        {
            m_Controllers.emplace_back(controller);

            controller->OnAdded();
        }

        void RemoveController(Controller* controller)
        {
            controller->OnRemoved();

            std::erase_if(m_Controllers, [controller](const Unique<Controller>& c) { return c.get() == controller; });
        }

        void ClearControllers()
        {
            m_Controllers.clear();
        }

    protected:
        explicit Application(const ApplicationInfo &info) : m_Name(info.Name), m_Arguments(info.Arguments) {}

        virtual void Initialize();
        void Dispose();

        virtual void RenderUi(Renderer::CommandBuffer& commandBuffer);
        virtual void Update(float delta);
        virtual void Render();

        [[nodiscard]] u32 GetArgumentCount() const { return m_Arguments.size(); }
        [[nodiscard]] const string &GetArgument(u32 index) const { return m_Arguments[index]; }


    protected:
        const string m_Name;
        vector<string> m_Arguments{};
        void OnFrame();


        static inline Unique<Application> s_Instance;

        Unique<Window> m_Window = nullptr;
        Renderer::Context m_Context {};
        Ui::Context m_UiContext {};
        vector<Unique<Controller>> m_Controllers{};
    };
}
