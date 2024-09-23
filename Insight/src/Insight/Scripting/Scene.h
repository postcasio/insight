#pragma once

#include "NativeApi.h"
#include "../Insight.h"

namespace Insight::Scripting
{
    struct ScriptScenePayload
    {
        Ref<Scene> Scene;
    };

    struct ScriptScene
    {
        enum Slots { ScriptScenePayloadSlot, SlotCount };

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

        static constexpr JSFunctionSpec Methods[] = {JS_FS_END};
        static constexpr JSPropertySpec Properties[] = {JS_PS_END};
        static constexpr JSFunctionSpec StaticMethods[] = {JS_FS_END};
        static constexpr JSPropertySpec StaticProperties[] = {JS_PS_END};

        static constexpr JSClass Clasp = {
            .name = "Scene",
            .flags = JSCLASS_HAS_RESERVED_SLOTS(SlotCount) | JSCLASS_FOREGROUND_FINALIZE,
            .cOps = &ClassOps
        };

        static JSObject* Create(const Ref<Scene>& scene = nullptr);
        static bool Constructor(JSContext* cx, unsigned argc, JS::Value* vp);

        // Full type of JSObject is not known, so we can't inherit.
        static ScriptScene* FromObject(JSObject* obj)
        {
            return reinterpret_cast<ScriptScene*>(obj);
        }

        static JSObject* AsObject(ScriptScene* custom)
        {
            return reinterpret_cast<JSObject*>(custom);
        }

        ScriptScenePayload* GetPayload();
        void SetPayload(ScriptScenePayload* payload);

        static void Initialize(JSContext* cx, const JS::RootedObject& global)
        {
            JS_InitClass(cx, global, nullptr, nullptr, Clasp.name, &Constructor, 0, Properties, Methods, StaticProperties, StaticMethods);
        }
    };
}
