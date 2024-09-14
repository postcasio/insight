#include "SceneExplorer.h"

#include "../../Editor.h"
#include "Insight/Components/HierarchyComponent.h"
#include "Insight/Components/NameComponent.h"
#include "Insight/Ui/Widgets.h"


namespace Portent::Editors
{
    void SceneExplorer::RenderObject(Entity entity, Scene& scene)
    {
        auto& nameComponent = entity.GetComponent<NameComponent>();
        auto selectedEntity = m_PrefabEditor.GetSelectedEntity();

        const auto& hierarchy = entity.GetComponent<HierarchyComponent>();

        if (hierarchy.Children > 0)
        {
            if (Widgets::ExpandingPropertyTreeLabel(nameComponent.Name))
            {
                Entity child = {hierarchy.FirstChild, &scene};

                while (child.GetHandle() != entt::null)
                {
                    RenderObject(child, scene);

                    child = {child.GetComponent<HierarchyComponent>().NextSibling, &scene};
                }
            }
        }
        else
        {
            if (Widgets::PropertyTreeLabel(nameComponent.Name, selectedEntity == entity))
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

        if (Widgets::BeginPropertyTree("Objects"))
        {
            RenderObject(scene->GetRoot(), *scene);

            Widgets::EndPropertyTree();
        }
    }
}
