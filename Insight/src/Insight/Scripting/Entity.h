#pragma once

#include "NativeApi.h"
#include "Scene.h"
#include "../Entity.h"
#include "../Insight.h"

namespace Insight::Scripting
{
    struct ScriptEntity
    {
        enum Slots { SceneSlot, EntityHandleSlot, SlotCount };

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
            .name = "Entity",
            .flags = JSCLASS_HAS_RESERVED_SLOTS(SlotCount),
            .cOps = &ClassOps
        };

        static JSObject* Create(Scene& scene, EntityHandle entityHandle);
        static bool Constructor(JSContext* cx, unsigned argc, JS::Value* vp);
        static bool RegisterInternal(JSContext* cx, unsigned argc, JS::Value* vp);
        static bool AddComponent(JSContext* cx, unsigned argc, JS::Value* vp);

        // Full type of JSObject is not known, so we can't inherit.
        static ScriptEntity* FromObject(JSObject* obj)
        {
            return reinterpret_cast<ScriptEntity*>(obj);
        }

        static JSObject* AsObject(ScriptEntity* custom)
        {
            return reinterpret_cast<JSObject*>(custom);
        }

        JSObject* GetScene()
        {
            return &JS::GetReservedSlot(AsObject(this), SceneSlot).toObject();
        }

        EntityHandle GetEntityHandle()
        {
            return static_cast<EntityHandle>(JS::GetReservedSlot(AsObject(this), EntityHandleSlot).toInt32());
        }

        Entity GetEntity()
        {
            return Entity{ GetEntityHandle(), ScriptScene::FromObject(GetScene())->GetPayload()->Scene.get()};
        }

        void SetScene(JSObject* scene)
        {
            JS::SetReservedSlot(AsObject(this), SceneSlot, JS::ObjectValue(*scene));
        }

        void SetEntityHandle(EntityHandle entityHandle)
        {
            JS::SetReservedSlot(AsObject(this), EntityHandleSlot, JS::Int32Value(static_cast<i32>(entityHandle)));;
        }

        static constexpr JSFunctionSpec Methods[] = {
            JS_FN("AddComponent", AddComponent, 1, 0),
            JS_FS_END
        };
        static constexpr JSPropertySpec Properties[] = {JS_PS_END};
        static constexpr JSFunctionSpec StaticMethods[] = {
            JS_FN("RegisterInternal", RegisterInternal, 2, 0),
            JS_FS_END
        };
        static constexpr JSPropertySpec StaticProperties[] = {JS_PS_END};

        static void Initialize(JSContext* cx, const JS::RootedObject& global)
        {
            JS_InitClass(cx, global, nullptr, nullptr, Clasp.name, &Constructor, 0, Properties, Methods, StaticProperties, StaticMethods);
        }

    };
}
