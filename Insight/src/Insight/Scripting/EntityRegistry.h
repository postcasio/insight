#pragma once

#include "JSEngine.h"
#include "../Insight.h"

namespace Insight
{
    class Scene;
}

namespace Insight::Scripting
{
    struct EntityRegistryEntry
    {
        string Name;
        string Description;
        JS::Heap<JS::Value> JSConstructor;
    };

    class EntityRegistry
    {
    public:
        EntityRegistry() = default;
        ~EntityRegistry() = default;

        void Register(const EntityRegistryEntry& entry);
        [[nodiscard]] vector<EntityRegistryEntry>& GetEntries() { return m_Registry; }
        void Clear();
        JSObject* Instantiate(EntityRegistryEntry& entry,
                              const JS::HandleObject& scene, const string& name);
        void ScriptTrace(JSTracer* tracer);

    private:
        vector<EntityRegistryEntry> m_Registry{};
    };
}
