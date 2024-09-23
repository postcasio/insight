#pragma once

#include "../Scripting/NativeApi.h"
#include "../Uuid.h"


namespace Insight
{
    class Entity;

    struct ComponentRegistryEntry
    {
        Uuid ComponentId;
        string ComponentName;
        std::function<void(Entity*)> AddFunction;
        const JSClass* JSClass;
    };

    class ComponentRegistry
    {
    public:
        ComponentRegistry();
        ~ComponentRegistry() = default;

        [[nodiscard]] const vector<ComponentRegistryEntry>& GetRegistry() const { return m_Registry; }
        [[nodiscard]] const ComponentRegistryEntry& GetEntry(Uuid componentId) const;
        [[nodiscard]] const ComponentRegistryEntry& GetEntry(i32 index) const;
        [[nodiscard]] const i32 GetIndex(Uuid uuid);

    private:
        vector<ComponentRegistryEntry> m_Registry{};
    };
}
