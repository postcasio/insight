#include "Modules.h"

#include "../Application.h"
#include "../Insight.h"

#include <js/SourceText.h>

namespace Insight::Scripting
{
    JSObject* JSResolveModule(JSContext* cx, path modulePath)
    {
        // Get the requested path relative to the requesting module's path
        path resolvedPath = canonical(modulePath);

        // If we already resolved before, return same module.
        auto search = ScriptHost::ModuleRegistry.find(resolvedPath);
        if (search != ScriptHost::ModuleRegistry.end())
        {
            return search->second->JSObject;
        }

        JS::RootedObject mod(cx);

        INS_ENGINE_INFO("Loading module: {0}", resolvedPath.string());

        mod = CompileJSModule(cx, resolvedPath.c_str(), Fs::ReadTextFile(resolvedPath).c_str());

        if (mod)
        {
            auto module = new Module{resolvedPath, JS::PersistentRootedObject(cx, mod)};

            JS::SetModulePrivate(module->JSObject, JS::PrivateValue(module));

            ScriptHost::ModuleRegistry.emplace(resolvedPath, module);
            return mod;
        }

        return nullptr;
    }

    JSObject* JSResolveHook(JSContext* cx, JS::HandleValue modulePrivate, JS::HandleObject moduleRequest)
    {
        // Get requesting module from private value.
        auto requestingModule = static_cast<Module*>(modulePrivate.toPrivate());

        // Extract module specifier string.
        JS::Rooted<JSString*> specifierString(
            cx, JS::GetModuleRequestSpecifier(cx, moduleRequest));
        if (!specifierString)
        {
            return nullptr;
        }

        // Convert specifier to a std::u16char for simplicity.
        JS::UniqueTwoByteChars specChars(JS_CopyStringCharsZ(cx, specifierString));
        if (!specChars)
        {
            return nullptr;
        }

        std::u16string filename(specChars.get());

        const string extension = path(filename).extension();

        const path relativePath = requestingModule->SourcePath.parent_path() / filename;

        vector<path> searchPaths = {
            relativePath
        };

        if (extension != ".js")
        {
            searchPaths.emplace_back(relativePath.string() + ".js");
        }

        searchPaths.emplace_back(relativePath.string() + "/index.js");

        for (auto& path : searchPaths)
        {
            if (Fs::FileExists(path))
            {
                auto mod = JSResolveModule(cx, path);

                if (mod)
                {
                    return mod;
                }

                Application::Instance().GetScriptHost().ReportAndClearException();

                JS_ReportErrorASCII(cx, "Failure loading module");
                return nullptr;
            }
        }

        JS_ReportErrorASCII(cx, "Cannot resolve import specifier");
        return nullptr;
    }

    bool JSDynamicImportHook(JSContext* cx, JS::Handle<JS::Value> referencingPrivate,
                             JS::Handle<JSObject*> moduleRequest, JS::Handle<JSObject*> promise)
    {
        JS::Rooted<JSObject*> mod{
            cx, JSResolveHook(cx, referencingPrivate, moduleRequest)
        };
        if (!mod || !JS::ModuleLink(cx, mod))
        {
            return JS::FinishDynamicModuleImport(cx, nullptr, referencingPrivate,
                                                 moduleRequest, promise);
        }

        JS::Rooted<JS::Value> rval{cx};
        if (!JS::ModuleEvaluate(cx, mod, &rval))
        {
            return JS::FinishDynamicModuleImport(cx, nullptr, referencingPrivate,
                                                 moduleRequest, promise);
        }
        if (rval.isObject())
        {
            JS::Rooted<JSObject*> evaluationPromise{cx, &rval.toObject()};
            return JS::FinishDynamicModuleImport(
                cx, evaluationPromise, referencingPrivate, moduleRequest, promise);
        }
        return JS::FinishDynamicModuleImport(cx, nullptr, referencingPrivate,
                                             moduleRequest, promise);
    }

    JSObject* CompileJSModule(JSContext* cx, const char* filename, const char* code)
    {
        JS::CompileOptions options(cx);
        options.setFileAndLine(filename, 1);

        JS::SourceText<mozilla::Utf8Unit> source;
        if (!source.init(cx, code, strlen(code), JS::SourceOwnership::Borrowed))
        {
            return nullptr;
        }

        // Compile the module source to bytecode.
        //
        // NOTE: This generates a JSObject instead of a JSScript. This contains
        // additional metadata to resolve imports/exports. This object should not be
        // exposed to other JS code or unexpected behaviour may occur.
        return JS::CompileModule(cx, options, source);
    }
}
