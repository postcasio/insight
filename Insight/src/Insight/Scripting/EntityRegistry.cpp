#include "EntityRegistry.h"

#include "Scene.h"
#include "../Scene.h"

namespace Insight::Scripting
{
    void EntityRegistry::Register(const EntityRegistryEntry& entry)
    {
        m_Registry.push_back(entry);
    }

    void EntityRegistry::Clear()
    {
        m_Registry.clear();
    }

    JSObject* EntityRegistry::Instantiate(EntityRegistryEntry& entry, const JS::HandleObject& scene,
                                          const string& name)
    {
        auto cx = Application::Instance().GetScriptHost().GetContext();

        JS::RootedValue constructor(cx, entry.JSConstructor);

        JS::RootedObject jsScene(cx, scene);
        JS::RootedString jsName(cx, JS_NewStringCopyZ(cx, name.c_str()));

        JS::RootedValueArray<2> args(cx);
        args[0].setObject(*jsScene);
        args[1].setString(jsName);

        JS::RootedObject entity(cx);
        if (!JS::Construct(cx, constructor, args, &entity))
        {
            INS_ENGINE_ERROR("Failed to instantiate entity");
            return nullptr;
        }

        return entity;
    }

    void EntityRegistry::ScriptTrace(JSTracer* tracer)
    {
        for (auto& entry : m_Registry)
        {
            JS::TraceEdge(tracer, &entry.JSConstructor, "EntityRegistryEntry::JSConstructor");
        }
    }
}
