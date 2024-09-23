#pragma once

#include <spdlog/spdlog.h>

#include "Insight.h"

namespace Insight
{
    enum class LogSource
    {
        Engine,
        Client
    };

    using LogLevel = spdlog::level::level_enum;

    struct LogEntry
    {
        LogSource Source;
        LogLevel Level;
        string Message;
    };

    class Log
    {
    public:
        static void Initialize();

        static spdlog::logger &GetEngineLog() { return *s_EngineLog; }
        static spdlog::logger &GetClientLog() { return *s_ClientLog; }

    private:
        static inline Ref<spdlog::logger> s_EngineLog;
        static inline Ref<spdlog::logger> s_ClientLog;
        static inline vector<LogEntry> s_LogEntries{};
        static inline i32 s_MaxLogEntries = 1000;
    };
};

// Core log macros
#define INS_ENGINE_TRACE(...) ::Insight::Log::GetEngineLog().trace(__VA_ARGS__)
#define INS_ENGINE_INFO(...) ::Insight::Log::GetEngineLog().info(__VA_ARGS__)
#define INS_ENGINE_WARN(...) ::Insight::Log::GetEngineLog().warn(__VA_ARGS__)
#define INS_ENGINE_ERROR(...) ::Insight::Log::GetEngineLog().error(__VA_ARGS__)
#define INS_ENGINE_CRITICAL(...) ::Insight::Log::GetEngineLog().critical(__VA_ARGS__)

// Client log macros
#define INS_TRACE(...) ::Insight::Log::GetClientLog().trace(__VA_ARGS__)
#define INS_INFO(...) ::Insight::Log::GetClientLog().info(__VA_ARGS__)
#define INS_WARN(...) ::Insight::Log::GetClientLog().warn(__VA_ARGS__)
#define INS_ERROR(...) ::Insight::Log::GetClientLog().error(__VA_ARGS__)
#define INS_CRITICAL(...) ::Insight::Log::GetClientLog().critical(__VA_ARGS__)