#pragma once

#include "JSEngine.h"
#include "../Insight.h"

namespace Insight::Scripting
{
    struct Module
    {
        path SourcePath;
        JS::PersistentRootedObject JSObject;
    };

    // Callback for embedding to provide modules for import statements. This example
    // hardcodes sources, but an embedding would normally load files here.
    JSObject* JSResolveHook(JSContext* cx,
                                        JS::HandleValue modulePrivate,
                                        JS::HandleObject moduleRequest);

    // Callback for embedding to implement an asynchronous dynamic import. This must
    // do the same thing as the module resolve hook, but also link and evaluate the
    // module, and it must always call JS::FinishDynamicModuleImport when done.
    bool JSDynamicImportHook(JSContext* cx,
                                         JS::Handle<JS::Value> referencingPrivate,
                                         JS::Handle<JSObject*> moduleRequest,
                                         JS::Handle<JSObject*> promise);

    // Translates source code into a JSObject representing the compiled module. This
    // module is not yet linked/instantiated.
    JSObject* CompileJSModule(JSContext* cx, const char* filename,
                                          const char* code);

    JSObject* JSResolveModule(JSContext* cx, path modulePath);
}
