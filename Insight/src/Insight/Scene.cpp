#include "Scene.h"

#include "Entity.h"
#include "Uuid.h"
#include "Assets/Prefab.h"
#include "Components/CameraComponent.h"
#include "Components/NameComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/EnvironmentComponent.h"
#include "Components/HierarchyComponent.h"
#include "Components/PrefabComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TransformComponent.h"
#include "Components/UuidComponent.h"

namespace Insight
{
    void Scene::CreateRoot()
    {
        m_Root = CreateEntity("Root", Entity::NullHandle).GetHandle();
    }

    void Scene::Clear()
    {
        m_Registry.clear();

        CreateRoot();
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        return CreateEntity(Uuid::CreateNew(), name);
    }

    Entity Scene::CreateEntity(const std::string& name, EntityHandle parent)
    {
        return CreateEntity(Uuid::CreateNew(), name, parent);
    }

    Entity Scene::InstantiatePrefab(const Ref<Prefab>& prefab)
    {
        const auto entity = CreateEntity(prefab->GetName());

        entity.AddComponent<PrefabComponent>(prefab);

        return entity;
    }

    Entity Scene::CreateEntity(Uuid id, const std::string& name)
    {
        return CreateEntity(id, name, m_Root);
    }

    Entity Scene::GetRoot()
    {
        return {m_Root, this};
    }

    Entity Scene::CreateEntity(Uuid id, const std::string& name, EntityHandle parent)
    {
        Entity entity = {m_Registry.create(), this};
        entity.AddComponent<UuidComponent>(id);
        entity.AddComponent<NameComponent>(name.empty() ? "Entity" : name);
        entity.AddComponent<TransformComponent>();
        entity.AddComponent<HierarchyComponent>();

        if (parent != Entity::NullHandle)
        {
            auto& hierarchy = m_Registry.get<HierarchyComponent>(entity.GetHandle());
            auto& parentHierarchy = m_Registry.get<HierarchyComponent>(parent);

            hierarchy.Parent = parent;
            hierarchy.PrevSibling = parentHierarchy.LastChild;

            if (parentHierarchy.LastChild == Entity::NullHandle)
            {
                parentHierarchy.FirstChild = entity.GetHandle();
            }
            else
            {
                auto& lastChildHierarchy = m_Registry.get<HierarchyComponent>(parentHierarchy.LastChild);
                lastChildHierarchy.NextSibling = entity.GetHandle();
            }

            parentHierarchy.Children += 1;
            parentHierarchy.LastChild = entity.GetHandle();
        }

        return entity;
    }

    void Scene::OnUpdate(float delta)
    {
    }

    Scene::Scene(const SceneInfo& info)
    {
        CreateRoot();
    }

    template <>
    void Scene::OnComponentAdded<UuidComponent>(Entity entity, UuidComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<NameComponent>(Entity entity, NameComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<StaticMeshComponent>(Entity entity, StaticMeshComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<PointLightComponent>(Entity entity, PointLightComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<DirectionalLightComponent>(Entity entity, DirectionalLightComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<PrefabComponent>(Entity entity, PrefabComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<HierarchyComponent>(Entity entity, HierarchyComponent& component)
    {
    }

    template <>
    void Scene::OnComponentAdded<EnvironmentComponent>(Entity entity, EnvironmentComponent& component)
    {
    }
}
