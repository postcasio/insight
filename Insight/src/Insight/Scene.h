#pragma once

#include <entt/entt.hpp>

#include "Uuid.h"
#include "Assets/Prefab.h"

namespace Insight
{
    namespace Renderer
    {
        class SceneRenderer;
    }

    class Entity;

    struct SceneInfo
    {
    };

    class Scene final
    {
    public:
        static Ref<Scene> Create(const SceneInfo& info = {}) { return CreateRef<Scene>(info); }

        template <typename T>
        void OnComponentAdded(Entity entity, T& component);
        void CreateRoot();
        void Clear();
        Entity CreateEntity(const std::string& name);
        Entity CreateEntity(const std::string& name, entt::entity parent);
        Entity CreateEntity(Uuid id, const std::string& name);
        Entity GetRoot();
        Entity CreateEntity(Uuid id, const std::string& name, entt::entity parent);

        Entity InstantiatePrefab(const Ref<Prefab>& prefab);
        void OnUpdate(float delta);

        explicit Scene(const SceneInfo& info = {});

        entt::registry& GetRegistry() { return m_Registry; }

    private:
        entt::registry m_Registry;
        entt::entity m_Root;

        friend class Entity;
        friend class Renderer::SceneRenderer;
    };
}
