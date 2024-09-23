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
#include "Components/ScriptingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TransformComponent.h"
#include "Components/UuidComponent.h"
#include "Scripting/Entity.h"

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
        return CreateEntity({ .Name = name });
    }

    Entity Scene::CreateEntity(const std::string& name, EntityHandle parent)
    {
        return CreateEntity({ .Name = name, .Parent = parent });
    }

    Entity Scene::InstantiatePrefab(const Ref<Prefab>& prefab)
    {
        const auto entity = CreateEntity(prefab->GetName());

        entity.AddComponent<PrefabComponent>(prefab);

        return entity;
    }

    Entity Scene::CreateEntity(Uuid id, const std::string& name)
    {
        return CreateEntity({ .Id = id, .Name = name });
    }

    Entity Scene::GetRoot()
    {
        return {m_Root, this};
    }

    Entity Scene::CreateEntity(Uuid id, const string& name, EntityHandle parent)
    {
        return CreateEntity(EntityInfo {
            .Name = name,
            .Id = id,
            .Parent = parent
        });
    }

    Entity Scene::CreateEntity(const EntityInfo& info)
    {
        Entity entity = {m_Registry.create(), this};
        entity.AddComponent<UuidComponent>(info.Id);
        entity.AddComponent<NameComponent>(info.Name);

        entity.AddComponent<TransformComponent>();
        entity.AddComponent<HierarchyComponent>();

        EntityHandle parentHandle = info.Parent;
        if (parentHandle == Entity::NullHandle && m_Root != Entity::NullHandle)
        {
            parentHandle = m_Root;
        }

        if (parentHandle != Entity::NullHandle)
        {
            auto& hierarchy = m_Registry.get<HierarchyComponent>(entity.GetHandle());
            auto& parentHierarchy = m_Registry.get<HierarchyComponent>(parentHandle);

            hierarchy.Parent = parentHandle;
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
g
            parentHierarchy.Children += 1;
            parentHierarchy.LastChild = entity.GetHandle();
        }
        else
        {

        }

        auto &scriptingComponent = entity.AddComponent<ScriptingComponent>();

        if (info.ScriptObject)
        {
            scriptingComponent.JSObject = info.ScriptObject;
        }
        else
        {
            scriptingComponent.JSObject = Scripting::ScriptEntity::Create(*this, entity.GetHandle());
        }

        return entity;
    }

    void Scene::OnUpdate(float delta)
    {
    }

    void Scene::ScriptTrace(JSTracer* tracer)
    {
        JS::TraceEdge(tracer, &m_ScriptScene, "Scene");

        m_Registry.view<ScriptingComponent>().each([tracer](auto entity, ScriptingComponent& scriptingComponent) mutable {
           JS::TraceEdge(tracer, &scriptingComponent.JSObject, "ScriptingComponent.JSObject");
        });
    }

    Scene::Scene(const SceneInfo& info)
    {
        INS_ENGINE_INFO("Scene()");
    }

    Scene::~Scene()
    {
        INS_ENGINE_INFO("~Scene()");
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

    template <>
    void Scene::OnComponentAdded<ScriptingComponent>(Entity entity, ScriptingComponent& component)
    {
    }
}
