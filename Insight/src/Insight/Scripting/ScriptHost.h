#pragma once

#include "EntityRegistry.h"
#include "JSEngineInit.h"
#include "Modules.h"
#include "../Insight.h"

namespace Insight::Scripting
{

    class ScriptHost
    {
    public:
        ScriptHost();
        ~ScriptHost();

        void Initialize();
        void Dispose();

        void ReportAndClearException();
        bool LoadModule(const path& sourcePath);

        [[nodiscard]] EntityRegistry& GetEntityRegistry() { return m_EntityRegistry; }
        [[nodiscard]] JSContext* GetContext() { return m_Context; }
        void Update();

        static inline map<std::string, Module*> ModuleRegistry{};
        static void Trace(JSTracer* tracer, void* data);

    private:
        JSObject* CreateGlobalObject(JSContext* cx);
        JSContext* m_Context = nullptr;
        Unique<JS::RootedObject> m_GlobalObject = nullptr;
        Unique<JSAutoRealm> m_Realm = nullptr;
        EntityRegistry m_EntityRegistry;
    };
}
