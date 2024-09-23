#include "ScriptHost.h"

#include "../Application.h"
#include "../Assert.h"
#include "Modules.h"
#include "NativeApi.h"

namespace Insight::Scripting
{
    ScriptHost::ScriptHost()
    {
        INS_ENGINE_ASSERT(JS_Init(), "Failed to initialize SpiderMonkey");
    }

    ScriptHost::~ScriptHost()
    {
        Dispose();
        JS_ShutDown();
    }

    void ScriptHost::Initialize()
    {
        INS_ENGINE_ASSERT(m_Context = JS_NewContext(JS::DefaultHeapMaxBytes), "Failed to create SpiderMonkey context");

        INS_ENGINE_ASSERT(js::UseInternalJobQueues(m_Context), "Failed to use internal job queues");

        INS_ENGINE_ASSERT(JS::InitSelfHostedCode(m_Context), "Failed to initialize self-hosted code");

        INS_ENGINE_ASSERT(m_GlobalObject = CreateUnique<JS::RootedObject>(m_Context, CreateGlobalObject(m_Context)));

        m_Realm = CreateUnique<JSAutoRealm>(m_Context, *m_GlobalObject);

        JSRuntime* rt = JS_GetRuntime(m_Context);

        JS::SetModuleResolveHook(rt, JSResolveHook);
        JS::SetModuleDynamicImportHook(rt, JSDynamicImportHook);

        JS_AddExtraGCRootsTracer(m_Context, &ScriptHost::Trace, this);

        NativeApi::Initialize(m_Context, *m_GlobalObject);
    }

    void ScriptHost::Dispose()
    {
        m_EntityRegistry.Clear();

        for (auto module : ModuleRegistry | std::views::values)
        {
            delete module;
        }

        ModuleRegistry.clear();

        if (m_Realm)
        {
            m_Realm.reset();
        }

        if (m_GlobalObject)
        {
            m_GlobalObject.reset();
        }

        if (m_Context)
        {
            JS_DestroyContext(m_Context);
            m_Context = nullptr;
        }
    }

    void ScriptHost::ReportAndClearException()
    {
        JS::ExceptionStack stack(m_Context);

        INS_ENGINE_ASSERT(JS::StealPendingExceptionStack(m_Context, &stack), "Failed to steal pending exception stack");

        JS::ErrorReportBuilder report(m_Context);

        INS_ENGINE_ASSERT(report.init(m_Context, stack, JS::ErrorReportBuilder::WithSideEffects),
                          "Couldn't build error report");

        JS::PrintError(stderr, report, false);
    }

    bool ScriptHost::LoadModule(const path& sourcePath)
    {
        auto mod = JSResolveModule(m_Context, sourcePath);

        if (!mod)
        {
            INS_ENGINE_ERROR("Failed to resolve module: {0}", sourcePath.string());

            return false;
        }

        auto module = new Module{sourcePath, JS::PersistentRootedObject{m_Context, mod}};

        JS::SetModulePrivate(module->JSObject, JS::PrivateValue(module));
        ScriptHost::ModuleRegistry.emplace(sourcePath, module);

        // Resolve imports by loading and compiling additional scripts.
        if (!JS::ModuleLink(m_Context, module->JSObject))
        {
            ReportAndClearException();
            return false;
        }

        // Result value, used for top-level await.
        JS::RootedValue rval(m_Context);

        // Execute the module bytecode.
        if (!JS::ModuleEvaluate(m_Context, module->JSObject, &rval))
        {
            ReportAndClearException();
            return false;
        }

        js::RunJobs(m_Context);

        if (rval.isObject())
        {
            JS::Rooted<JSObject*> evaluationPromise{m_Context, &rval.toObject()};
            if (!JS::ThrowOnModuleEvaluationFailure(
                m_Context, evaluationPromise,
                JS::ModuleErrorBehaviour::ThrowModuleErrorsSync))
            {
                ReportAndClearException();
                return false;
            }
        }

        return true;
    }

    void ScriptHost::Update()
    {
        js::RunJobs(m_Context);
        JS_MaybeGC(m_Context);
    }

    void ScriptHost::Trace(JSTracer* tracer, void* data)
    {
        auto& app = Application::Instance();

        auto& host = app.GetScriptHost();
        host.GetEntityRegistry().ScriptTrace(tracer);

        app.ScriptTrace(tracer);
    }

    JSObject* ScriptHost::CreateGlobalObject(JSContext* cx)
    {
        JS::RealmOptions options;

        static JSClass globalClass = {
            "InsightGlobal",
            JSCLASS_GLOBAL_FLAGS,
            &JS::DefaultGlobalClassOps
        };

        return JS_NewGlobalObject(cx, &globalClass, nullptr, JS::FireOnNewGlobalHook, options);
    }
}
