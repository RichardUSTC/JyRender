#include "Log.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> GetLogger(const char* moduleName)
{
    if (auto logger = spdlog::get(moduleName); logger != nullptr)
        return logger;

    static std::vector<spdlog::sink_ptr> sinks;

    if (sinks.empty())
    {
        sinks.reserve(2);

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_level(spdlog::level::trace);
        consoleSink->set_pattern("[%n][%^%T-%F %L%$] %@ %v"); // NOLINT(clang-diagnostic-undefined-func-template)

        const auto maxSize  = 1048576 * 5;
        const auto maxFiles = 3;
        auto       fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/log.txt", maxSize, maxFiles);
        fileSink->set_level(spdlog::level::trace);
        fileSink->set_pattern("[%n][%^%T-%F %L%$] %@ %v"); // NOLINT(clang-diagnostic-undefined-func-template)

        sinks.push_back(consoleSink);
        sinks.push_back(fileSink);
    }

    auto logger = std::make_shared<spdlog::logger>(moduleName, sinks.begin(), sinks.end());
    logger->flush_on(spdlog::level::err);
    logger->set_level(spdlog::level::trace);
    register_logger(logger);

    return logger;
}
