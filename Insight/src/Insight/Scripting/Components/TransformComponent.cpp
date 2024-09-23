#include "TransformComponent.h"
#include "../NativeApi.h"
#include "../Entity.h"
#include "../../Components/TransformComponent.h"

namespace Insight::Scripting
{
    using namespace NativeApi;

    bool ScriptTransformComponent::Constructor(JSContext* cx, unsigned argc, JS::Value* vp) {
        INAPI_NO_CONSTRUCT();
    }

    bool ScriptTransformComponent::IdGetter(JSContext* cx, unsigned argc, JS::Value* vp)
    {
        INAPI_ARGS();
        args.rval().setString(JS_NewStringCopyZ(cx, TransformComponent::ComponentId.ToString().c_str()));
        return true;
    }

    bool ScriptTransformComponent::NameGetter(JSContext* cx, unsigned argc, JS::Value* vp)
    {
        INAPI_ARGS();
        args.rval().setString(JS_NewStringCopyZ(cx, TransformComponent::ComponentName.c_str()));
        return true;
    }

    void ScriptTransformComponent::Finalize(JS::GCContext* gcx, JSObject* obj) {

    }

    void ScriptTransformComponent::Trace(JSTracer* trc, JSObject* obj) {

    }
}
