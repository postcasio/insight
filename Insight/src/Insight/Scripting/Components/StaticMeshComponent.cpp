#include "StaticMeshComponent.h"
#include "../NativeApi.h"
#include "../Entity.h"
#include "../../Components/StaticMeshComponent.h"

namespace Insight::Scripting
{
    using namespace NativeApi;

    bool ScriptStaticMeshComponent::Constructor(JSContext* cx, unsigned argc, JS::Value* vp) {
        INAPI_NO_CONSTRUCT();
    }

    bool ScriptStaticMeshComponent::IdGetter(JSContext* cx, unsigned argc, JS::Value* vp)
    {
        INAPI_ARGS();
        args.rval().setString(JS_NewStringCopyZ(cx, StaticMeshComponent::ComponentId.ToString().c_str()));
        return true;
    }

    bool ScriptStaticMeshComponent::NameGetter(JSContext* cx, unsigned argc, JS::Value* vp)
    {
        INAPI_ARGS();
        args.rval().setString(JS_NewStringCopyZ(cx, StaticMeshComponent::ComponentName.c_str()));
        return true;
    }

    void ScriptStaticMeshComponent::Finalize(JS::GCContext* gcx, JSObject* obj) {

    }

    void ScriptStaticMeshComponent::Trace(JSTracer* trc, JSObject* obj) {

    }
}
