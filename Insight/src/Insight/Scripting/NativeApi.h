#pragma once

#include "JSEngine.h"
#include "ScriptHost.h"

#define INAPI_ARGS() \
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
#define INAPI_REQUIRED_ARGS(name, count) \
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);\
    if (!args.requireAtLeast(cx, name, count)) return false;

#define INAPI_REQUIRE_CONSTRUCTING() \
    {\
        if (!args.isConstructing()) {\
            JS_ReportErrorASCII(cx, "You must call this constructor with 'new'");\
            return false;\
        }\
    }

#define INAPI_NO_CONSTRUCT(name) \
    {\
        JS_ReportErrorASCII(cx, name " cannot be constructed" );\
        return false;\
    }

#define INAPI_CREATE_THIS_OBJECT() \
    JS::RootedObject thisObj(cx, JS_NewObjectForConstructor(cx, &Clasp, args));\
    if (!thisObj) return false

#define INAPI_GET_THIS_OBJECT() \
    JS::RootedObject thisObj(cx, args.thisv().toObjectOrNull());\

#define CONSTRUCTOR_ARGS(name, minimumArgc) \
JS::CallArgs args = JS::CallArgsFromVp(argc, vp);\
if (!args.requireAtLeast(cx, name, minimumArgc)) return false;\
if (!args.isConstructing()) {\
JS_ReportErrorASCII(cx, "You must call this constructor with 'new'");\
return false;\
}\
JS::RootedObject thisObj(cx, JS_NewObjectForConstructor(cx, &Clasp, args));\
if (!thisObj) return false


#define NATIVEAPI_CONSTRUCTOR_PREAMBLE(name, minimumArgc) \
JS::CallArgs args = JS::CallArgsFromVp(argc, vp);\
if (!args.requireAtLeast(cx, #name, minimumArgc)) return false;\
if (!args.isConstructing()) {\
JS_ReportErrorASCII(cx, "You must call this constructor with 'new'");\
return false;\
}\
JS::RootedObject thisObj(cx, JS_NewObjectForConstructor(cx, &Script##name##Class, args));\
if (!thisObj) return false

#define NATIVEAPI_CLASS(name, flags) \
static JSClass Script##name##Class {\
#name,\
flags,\
nullptr\
}

#define NATIVEAPI_CLASS_WITH_FINALIZER(name, flags) \
    static constexpr JSClassOps Script##name##ClassOps = {\
        .finalize = Script##name##Finalizer\
        };\
static JSClass Script##name##Class {\
#name,\
flags | JSCLASS_FOREGROUND_FINALIZE,\
&Script##name##ClassOps\
}

#define NATIVEAPI_CLASS_METHODS(name) static JSFunctionSpec Script##name##Methods[]

#define NATIVEAPI_CLASS_PROPERTIES(name) static JSPropertySpec Script##name##Properties[]

#define NATIVEAPI_CLASS_STATIC_METHODS(name) static JSFunctionSpec Script##name##StaticMethods[]

#define NATIVEAPI_CLASS_STATIC_PROPERTIES(name) static JSPropertySpec Script##name##StaticProperties[]

#define NATIVEAPI_CLASS_CONSTRUCTOR(name) static bool Script##name##Constructor(JSContext* cx, unsigned argc, JS::Value* vp)

#define NATIVEAPI_CLASS_INTERNAL if (!s_Initializing) { JS_ReportErrorASCII(cx, "This class cannot be constructed"); return false; }

#define NATIVEAPI_INIT_CLASS(className, argc) \
JS::RootedObject proto##className(cx, JS_InitClass(cx, global, nullptr, nullptr, Script##className##Class.name, \
Script##className##Constructor, argc, \
Script##className##Properties, \
Script##className##Methods, \
Script##className##StaticProperties, \
Script##className##StaticMethods)); \
if (!proto##className) { throw std::runtime_error(fmt::format("Failed to initialize class Script{0}", #className)); }

#define NATIVEAPI_FUNCTION(name) static bool name(JSContext* cx, unsigned argc, JS::Value* vp)

#define NATIVEAPI_FUNCTION_ARGS(count) \
JS::CallArgs args = JS::CallArgsFromVp(count, vp);\
if (!args.requireAtLeast(cx, __FUNCTION__, count)) return false

#define NATIVEAPI_SLOT(index) static_cast<size_t>(index)

#define NATIVEAPI_SET_SLOT_OBJECT(obj, slot, other)\
{\
JS::RootedValue s(cx, JS::GetReservedSlot(obj, NATIVEAPI_SLOT(slot)));\
s.setObject(other);\
}

#define NATIVEAPI_SET_SLOT_PRIVATEUINT32(obj, slot, value)\
{\
JS::RootedValue s(cx, JS::GetReservedSlot(obj, NATIVEAPI_SLOT(slot)));\
s.setPrivateUint32(value);\
}

#define NATIVEAPI_SET_SLOT_PRIVATE(obj, slot, value)\
{\
JS::RootedValue s(cx, JS::GetReservedSlot(obj, NATIVEAPI_SLOT(slot)));\
s.setPrivate(value);\
}

#define NATIVEAPI_SET_SLOT_BOOLEAN(obj, slot, value)\
{\
JS::RootedValue s(cx, JS::GetReservedSlot(obj, NATIVEAPI_SLOT(slot)));\
s.setBoolean(value);\
}

#define NATIVEAPI_GET_SLOT_PRIVATE(obj, slot) JS::GetReservedSlot(obj, NATIVEAPI_SLOT(slot)).toPrivate()

#define NATIVEAPI_RETURN_OBJECT(obj) \
    args.rval().setObject(obj);

namespace Insight::Scripting::NativeApi
{
    void Initialize(JSContext* cx, const JS::RootedObject& global);

    inline string NATIVEAPI_STRING(JSContext* cx, JS::HandleValue val)
    {
        JS::UniqueChars str(JS_EncodeStringToUTF8(cx, JS::RootedString{cx, JS::ToString(cx, val)}));

        return string(str.get());
    }

    string NAPI_GetString(JSContext* cx, JS::HandleValue str);
}
