#include "Insight/Ui/ImGui.h"

#include "EditorApplication.h"
#include "Editor.h"


namespace Portent {
    void EditorApplication::Initialize()
    {
        Application::Initialize();

        optional<path> projectPath;
        if (GetArgumentCount() > 1)
        {
            projectPath = std::filesystem::canonical(GetArgument(1));
        }

        m_Editor = new Editor(projectPath);

        AddController(m_Editor);
    }

    void EditorApplication::RenderUi(Insight::Renderer::CommandBuffer& commandBuffer)
    {
        Application::RenderUi(commandBuffer);
    }
} // Portent