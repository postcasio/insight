#pragma once

#include "PrefabEditor.h"
#include "../EditorPanel.h"
#include "Insight/Application.h"
#include "Insight/Entity.h"
#include "Insight/Ui/Panel.h"
#include "Insight/Ui/Widgets.h"

namespace Portent::Editors
{
    class SceneExplorer final : public Insight::Panel
    {
    public:
        explicit SceneExplorer(PrefabEditor& sceneEditor) : Panel("Explorer##PrefabEditor"),
                                                           m_PrefabEditor(sceneEditor)
        {
            m_WindowClass = "PrefabEditor";

            auto& entries = Application::Instance().GetScriptHost().GetEntityRegistry().GetEntries();

            for (auto i = 0 ; i < entries.size(); i++)
            {
                m_EntityOptions.push_back({
                    i, entries[i].Name
                });
            }
        };

        void OnAdded() override
        {
            const auto editorPanel = reinterpret_cast<PrefabEditor*>(m_Parent);

            m_Title = "Explorer##" + editorPanel->GetTitle();
            m_WindowClass = editorPanel->GetDockSpaceClass();
        }

        ~SceneExplorer() override = default;

        void RenderObject(Insight::Entity entity, Insight::Scene& scene);
        void OnRenderContents() override;

    private:
        PrefabEditor& m_PrefabEditor;

        vector<Widgets::ComboOption<i32>> m_EntityOptions{};
    };
}
