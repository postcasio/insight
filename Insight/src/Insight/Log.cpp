#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/callback_sink.h>

#include "Log.h"
#include "Insight.h"

namespace Insight
{

    void Log::Initialize()
    {
        vector<spdlog::sink_ptr> sinks = {
            CreateRef<spdlog::sinks::stdout_color_sink_mt>()};

        sinks[0]->set_pattern("%^[%T] %n: %v%$");

        constexpr auto consoleLogLevel = LogLevel::info;

        s_EngineLog = CreateRef<spdlog::logger>("ENGINE", begin(sinks), end(sinks));
        spdlog::register_logger(s_EngineLog);
        s_EngineLog->set_level(consoleLogLevel);
        s_EngineLog->flush_on(consoleLogLevel);

        s_ClientLog = CreateRef<spdlog::logger>("CLIENT", begin(sinks), end(sinks));
        spdlog::register_logger(s_ClientLog);
        s_ClientLog->set_level(consoleLogLevel);
        s_ClientLog->flush_on(consoleLogLevel);
    }
}
