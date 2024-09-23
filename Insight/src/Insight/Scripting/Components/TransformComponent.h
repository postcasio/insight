#pragma once

#include "../../Insight.h"
#include "../JSEngine.h"

namespace Insight::Scripting
{
    struct ScriptTransformComponent
    {
        enum Slots { EntitySlot, ComponentIndexSlot, SlotCount };

        // When the CustomObject is collected, delete the stored box.
        static void Finalize(JS::GCContext* gcx, JSObject* obj);

        // When a CustomObject is traced, it must trace the stored box.
        static void Trace(JSTracer* trc, JSObject* obj);

        static constexpr JSClassOps ClassOps = {
            // Use .finalize if CustomObject owns the C++ object and should delete it
            // when the CustomObject dies.
            .finalize = Finalize,

            // Use .trace whenever the JS object points to a C++ object that can reach
            // other JS objects.
            .trace = Trace
        };


        static constexpr JSClass Clasp = {
            .name = "TransformComponent",
            .flags = JSCLASS_HAS_RESERVED_SLOTS(SlotCount),
            .cOps = &ClassOps
        };

        static bool Constructor(JSContext* cx, unsigned argc, JS::Value* vp);

        // Full type of JSObject is not known, so we can't inherit.
        static ScriptTransformComponent* FromObject(JSObject* obj)
        {
            return reinterpret_cast<ScriptTransformComponent*>(obj);
        }

        static JSObject* AsObject(ScriptTransformComponent* custom)
        {
            return reinterpret_cast<JSObject*>(custom);
        }

        JSObject* GetEntity()
        {
            return &JS::GetReservedSlot(AsObject(this), EntitySlot).toObject();
        }

        i32 GetComponentIndex()
        {
            return JS::GetReservedSlot(AsObject(this), ComponentIndexSlot).toInt32();
        }

        void SetEntity(JSObject* entity)
        {
            JS::SetReservedSlot(AsObject(this), EntitySlot, JS::ObjectValue(*entity));
        }

        void SetComponentIndex(i32 componentIndex)
        {
            JS::SetReservedSlot(AsObject(this), ComponentIndexSlot, JS::Int32Value(componentIndex));
        }

        static bool IdGetter(JSContext* cx, unsigned argc, JS::Value* vp);
        static bool NameGetter(JSContext* cx, unsigned argc, JS::Value* vp);

        static constexpr JSFunctionSpec Methods[] = {JS_FS_END};
        static constexpr JSPropertySpec Properties[] = {JS_PS_END};
        static constexpr JSFunctionSpec StaticMethods[] = {JS_FS_END};
        static constexpr JSPropertySpec StaticProperties[] = {
            JS_PSG("Id", IdGetter, JSPROP_ENUMERATE),
            JS_PSG("Name", NameGetter, JSPROP_ENUMERATE),
            JS_PS_END
        };

        static void Initialize(JSContext* cx, const JS::RootedObject& global)
        {
            JS_InitClass(cx, global, nullptr, nullptr, Clasp.name, &Constructor, 0, Properties, Methods,
                         StaticProperties, StaticMethods);
        }
    };
}
