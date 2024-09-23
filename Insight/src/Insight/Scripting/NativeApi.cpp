#include "NativeApi.h"

#include "Entity.h"
#include "Scene.h"
#include "../Assert.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TransformComponent.h"

static bool s_Initializing = false;

namespace Insight::Scripting::NativeApi
{
    NATIVEAPI_CLASS(Console, JSCLASS_HAS_RESERVED_SLOTS(0));

    NATIVEAPI_FUNCTION(Console_Log)
    {
        NATIVEAPI_FUNCTION_ARGS(1);

        JS::RootedString strRoot(cx, JS::ToString(cx, args[0]));
        JS::UniqueChars utf8(JS_EncodeStringToUTF8(cx, strRoot));

        Log::GetClientLog().info("[JS] {0}", utf8.get());

        return true;
    }

    NATIVEAPI_CLASS_METHODS(Console) = {JS_FS_END};
    NATIVEAPI_CLASS_PROPERTIES(Console) = {JS_PS_END};
    NATIVEAPI_CLASS_STATIC_METHODS(Console) = {
        JS_FN("Log", Console_Log, 0, JSPROP_ENUMERATE),
        JS_FS_END
    };
    NATIVEAPI_CLASS_STATIC_PROPERTIES(Console) = {JS_PS_END};

    NATIVEAPI_CLASS_CONSTRUCTOR(Console)
    {
        NATIVEAPI_CONSTRUCTOR_PREAMBLE(Console, 0);

        args.rval().setObject(*thisObj);
        return true;
    }

    void Initialize(JSContext* cx, const JS::RootedObject& global)
    {
        s_Initializing = true;

        NATIVEAPI_INIT_CLASS(Console, 0);

        ScriptEntity::Initialize(cx, global);
        ScriptScene::Initialize(cx, global);
        ScriptTransformComponent::Initialize(cx, global);
        ScriptStaticMeshComponent::Initialize(cx, global);

        s_Initializing = false;
    }

    string NAPI_GetString(JSContext* cx, JS::HandleValue str)
    {
        JS::RootedString strRoot(cx, JS::ToString(cx, str));
        JS::UniqueChars utf8(JS_EncodeStringToUTF8(cx, strRoot));

        return utf8.get();
    }

}
