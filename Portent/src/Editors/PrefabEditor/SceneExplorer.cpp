#include "SceneExplorer.h"

#include <Insight/Components/HierarchyComponent.h>
#include "Insight/Components/NameComponent.h"
#include "Insight/Ui/FontAwesome6.h"
#include "Insight/Ui/Widgets.h"

namespace Portent::Editors
{
    void SceneExplorer::RenderObject(Insight::Entity entity, Insight::Scene& scene)
    {
        auto& nameComponent = entity.GetComponent<Insight::NameComponent>();
        auto selectedEntity = m_PrefabEditor.GetSelectedEntity();

        const auto& hierarchy = entity.GetComponent<Insight::HierarchyComponent>();

        if (hierarchy.Children > 0)
        {
            if (Insight::Widgets::ExpandingPropertyTreeLabel(nameComponent.Name))
            {
                Insight::Entity child = {hierarchy.FirstChild, &scene};

                while (child.GetHandle() != entt::null)
                {
                    RenderObject(child, scene);

                    child = {child.GetComponent<Insight::HierarchyComponent>().NextSibling, &scene};
                }
            }
        }
        else
        {
            if (Insight::Widgets::PropertyTreeLabel(nameComponent.Name, selectedEntity == entity))
            {
                if (selectedEntity == entity)
                {
                    m_PrefabEditor.DeselectEntity();
                }
                else
                {
                    m_PrefabEditor.SelectEntity(entity);
                }
            }
        }
    }

    void SceneExplorer::OnRenderContents()
    {
        auto scene = m_PrefabEditor.GetScene();

        static i32 selectedEntity = 0;

        Insight::Widgets::Combo("Add Entity", &selectedEntity, m_EntityOptions.data(), m_EntityOptions.size());

        ImGui::SameLine();

        if (Insight::Widgets::SmallButton(ICON_FA_PLUS))
        {
            auto& scriptHost = Application::Instance().GetScriptHost();
            auto& entityScriptDef = scriptHost.GetEntityRegistry().GetEntries()[selectedEntity];
            auto cx = scriptHost.GetContext();

            JS::RootedObject scriptScene = {cx, scene->GetScriptScene()};
            JS::RootedObject scriptEntity = {cx, scriptHost.GetEntityRegistry().Instantiate(entityScriptDef, scriptScene, "New Entity")};
        }

        if (Insight::Widgets::BeginPropertyTree("Objects"))
        {
            RenderObject(scene->GetRoot(), *scene);

            Insight::Widgets::EndPropertyTree();
        }
    }
}
