#include "Scene.h"
#include "../Scene.h"
#include "../Application.h"

namespace Insight::Scripting
{
    using namespace NativeApi;

    JSObject* ScriptScene::Create(const Ref<Scene>& scene) {
        auto cx = Application::Instance().GetScriptHost().GetContext();

        JS::Rooted<JSObject*> obj(cx, JS_NewObject(cx, &Clasp));

        if (!obj) {
            return nullptr;
        }

        auto scriptScene = FromObject(obj);
        scriptScene->SetPayload(new ScriptScenePayload {scene });

        return obj;
    }

    bool ScriptScene::Constructor(JSContext* cx, unsigned argc, JS::Value* vp) {
        INAPI_REQUIRED_ARGS("Scene", 0);
        INAPI_REQUIRE_CONSTRUCTING();

        INAPI_CREATE_THIS_OBJECT();

        INS_ENGINE_INFO("ScriptScene::Constructor called");

        auto scriptScene = FromObject(thisObj);
        scriptScene->SetPayload(new ScriptScenePayload { Scene::Create({}, thisObj) });

        args.rval().setObject(*thisObj);

        return true;
    }

    ScriptScenePayload* ScriptScene::GetPayload()
    {
        return JS::GetMaybePtrFromReservedSlot<ScriptScenePayload>(AsObject(this), ScriptScenePayloadSlot);
    }

    void ScriptScene::SetPayload(ScriptScenePayload* scene)
    {
        JS_SetReservedSlot(AsObject(this), ScriptScenePayloadSlot, JS::PrivateValue(scene));
    }

    void ScriptScene::Finalize(JS::GCContext* gcx, JSObject* obj) {
        delete FromObject(obj)->GetPayload();
    }

    void ScriptScene::Trace(JSTracer* trc, JSObject* obj) {

    }
}
