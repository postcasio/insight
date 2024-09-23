#pragma once

#include "../Insight.h"
#include "../Scripting/NativeApi.h"

namespace Insight
{
    class Entity;

    struct ScriptingComponent
    {
        static inline const Uuid ComponentId = Uuid {"8bda751f-b275-40f2-bd03-f5c65abe7688"};
        static inline const string ComponentName = "ScriptingComponent";
        static void AddFunction(Entity* entity);
        static inline const JSClass* JSClass = nullptr;

        JS::Heap<JSObject*> JSObject;

        ScriptingComponent() = default;
        ScriptingComponent(const ScriptingComponent&) = default;

        explicit ScriptingComponent(const JS::HandleObject& object) : JSObject(object)
        {
        }
    };
}
