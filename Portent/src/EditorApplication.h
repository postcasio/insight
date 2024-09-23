#pragma once

#include "Editor.h"
#include "Portent.h"
#include "Insight/Application.h"

namespace Portent
{
    class EditorApplication final : public Insight::Application
    {
    public:
        explicit EditorApplication(const Insight::ApplicationInfo& info) : Application(info)
        {
        };

        [[nodiscard]] Editor &GetEditor() const { return *m_Editor; }

    protected:
        void Initialize() override;
        void RenderUi(Insight::Renderer::CommandBuffer& commandBuffer) override;


    private:
        Editor* m_Editor;
    };
} // Portent
