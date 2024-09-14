#include "MaterialEditorProperties.h"

#include "Insight/Ui/Widgets.h"

namespace Portent::Editors {

    void MaterialEditorProperties::OnRenderContents()
    {
        if (Widgets::Button("Compile"))
        {
            m_MaterialEditor.Compile();
            m_MaterialEditor.UpdateModel();
        }

        if (Widgets::BeginPropertyTree("Properties"))
        {
            for (auto attribute : m_MaterialEditor.GetGraph().GetParameterizedAttributes())
            {
                ImGui::PushID(attribute->GetId());
                NodeGraph::Renderer::RenderParameter(attribute);
                ImGui::PopID();
            }

            Widgets::EndPropertyTree();
        }
    }
}
