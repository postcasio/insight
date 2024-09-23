#pragma once

#include <entt/entt.hpp>

#include "Application.h"
#include "Uuid.h"
#include "Assets/Prefab.h"
#include "Scripting/Scene.h"

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

    struct EntityInfo
    {
        string Name = "Entity";
        Uuid Id = Uuid::CreateNew();
        entt::entity Parent = entt::null;
        JS::HandleObject ScriptObject = nullptr;
    };

    class Scene final
    {
    public:
        explicit Scene(const SceneInfo& info = {});
        ~Scene();

        static Ref<Scene> Create(const SceneInfo& info = {}, JSObject* scriptScene = nullptr)
        {
            auto scene = CreateRef<Scene>(info);

            scene->SetScriptScene(scriptScene ? scriptScene : Scripting::ScriptScene::Create(scene));

            return scene;
        }

        template <typename T>
        void OnComponentAdded(Entity entity, T& component);
        void CreateRoot();
        void Clear();
        Entity CreateEntity(const std::string& name);
        Entity CreateEntity(const std::string& name, entt::entity parent);
        Entity CreateEntity(Uuid id, const std::string& name);
        Entity GetRoot();
        Entity CreateEntity(Uuid id, const std::string& name, entt::entity parent);
        Entity CreateEntity(const EntityInfo& info);

        Entity InstantiatePrefab(const Ref<Prefab>& prefab);
        void OnUpdate(float delta);

        entt::registry& GetRegistry() { return m_Registry; }

        void SetScriptScene(JSObject* scriptScene) { m_ScriptScene = scriptScene; }
        JSObject* GetScriptScene() { return m_ScriptScene; }

        void ScriptTrace(JSTracer* tracer);


    private:
        entt::registry m_Registry;
        entt::entity m_Root = entt::null;
        JS::Heap<JSObject*> m_ScriptScene;

        friend class Entity;
        friend class Renderer::SceneRenderer;

    };
}
