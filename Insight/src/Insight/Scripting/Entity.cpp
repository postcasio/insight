#include "Entity.h"

#include "Scene.h"
#include "../Scene.h"
#include "../Application.h"
#include "../Components/ScriptingComponent.h"

namespace Insight::Scripting
{
    using namespace NativeApi;

    JSObject* ScriptEntity::Create(Scene& scene, EntityHandle entityHandle) {
        JSContext* cx = Application::Instance().GetScriptHost().GetContext();

        JS::Rooted<JSObject*> obj(cx, JS_NewObject(cx, &Clasp));

        if (!obj) {
            return nullptr;
        }

        auto scriptEntity = FromObject(obj);

        scriptEntity->SetScene(scene.GetScriptScene());
        scriptEntity->SetEntityHandle(entityHandle);

        return obj;
    }

    bool ScriptEntity::Constructor(JSContext* cx, unsigned argc, JS::Value* vp) {
        INAPI_REQUIRED_ARGS("Entity", 2);
        INAPI_REQUIRE_CONSTRUCTING();

        INAPI_CREATE_THIS_OBJECT();

        INS_ENGINE_INFO("ScriptEntity::Constructor called");

        JS::RootedObject scene{cx, JS::ToObject(cx, args[0])};

        auto scriptEntity = FromObject(thisObj);
        auto scriptScene = ScriptScene::FromObject(scene);

        scriptEntity->SetScene(scene);

        auto name = NAPI_GetString(cx, args[1]);

        auto entity = scriptScene->GetPayload()->Scene->CreateEntity(
            {
                .Name = name,
                .ScriptObject = thisObj
            });

        scriptEntity->SetEntityHandle(entity.GetHandle());

        args.rval().setObject(*thisObj);

        return true;
    }


    bool ScriptEntity::RegisterInternal(JSContext* cx, unsigned argc, JS::Value* vp)
    {
        INAPI_REQUIRED_ARGS("RegisterInternal", 2);

        JS::RootedObject info = {cx, &args[0].toObject()};
        JS::RootedValue entityClass = {cx, args[1]};

        JS::RootedValue nameVal{cx};
        JS_GetProperty(cx, info, "Name", &nameVal);
        auto name = NAPI_GetString(cx, nameVal);

        JS::RootedValue descriptionVal{cx};
        JS_GetProperty(cx, info, "Description", &descriptionVal);
        auto description = NAPI_GetString(cx, descriptionVal);

        Application::Instance().GetScriptHost().GetEntityRegistry().Register({
            .Name = name,
            .Description = description,
            .JSConstructor = JS::Heap<JS::Value>{ entityClass }
        });

        args.rval().setObject(entityClass.toObject());
        return true;
    }

    bool ScriptEntity::AddComponent(JSContext* cx, unsigned argc, JS::Value* vp)
    {
        INAPI_REQUIRED_ARGS("AddComponent", 1);

        auto scriptEntity = FromObject(&args.thisv().toObject());

        auto entity = scriptEntity->GetEntity();

        JS::RootedObject componentConstructor = {cx, &args[0].toObject()};

        JS::RootedValue componentIdVal{cx};
        JS_GetProperty(cx, componentConstructor, "Id", &componentIdVal);

        auto componentIdStr = NAPI_GetString(cx, componentIdVal);
        auto componentId = Uuid { componentIdStr };

        auto& registry = Application::Instance().GetComponentRegistry();

        auto& componentInfo = registry.GetEntry(componentId);

        componentInfo.AddFunction(&entity);

        JS::Rooted<JSObject*> component(cx, JS_NewObject(cx, componentInfo.JSClass));

        if (!component) {
            return false;
        }

        JS::SetReservedSlot(component, 0, args.thisv());
        JS::SetReservedSlot(component, 1, JS::Int32Value(registry.GetIndex(componentId)));
        INS_ENGINE_INFO("Added  component!");
        args.rval().setObject(*component);
        return true;
    }


    void ScriptEntity::Finalize(JS::GCContext* gcx, JSObject* obj) {

    }

    void ScriptEntity::Trace(JSTracer* trc, JSObject* obj) {

    }
}
