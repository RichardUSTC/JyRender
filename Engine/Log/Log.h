#pragma once
#include <spdlog/spdlog.h>

std::shared_ptr<spdlog::logger> GetLogger(const char* moduleName);

#define LOG_TRACE(moduleName, ...) do { static auto logger = GetLogger(moduleName); SPDLOG_LOGGER_LOGGER_TRACE(logger, __VA_ARGS__); } while(false)
#define LOG_DEBUG(moduleName, ...) do { static auto logger = GetLogger(moduleName); SPDLOG_LOGGER_DEBUG(logger, __VA_ARGS__); } while(false)
#define LOG_INFO(moduleName, ...) do { static auto logger = GetLogger(moduleName); SPDLOG_LOGGER_INFO(logger, __VA_ARGS__); } while(false)
#define LOG_WARN(moduleName, ...) do { static auto logger = GetLogger(moduleName); SPDLOG_LOGGER_WARN(logger, __VA_ARGS__); } while(false)
#define LOG_ERROR(moduleName, ...) do { static auto logger = GetLogger(moduleName); SPDLOG_LOGGER_ERROR(logger, __VA_ARGS__); } while(false)
#define LOG_CRITICAL(moduleName, ...) do { static auto logger = GetLogger(moduleName); SPDLOG_LOGGER_CRITICAL(logger, __VA_ARGS__); } while(false)
#define LOG_ASSERT_MSG(condition, moduleName, ...) do { if(!condition) { static auto logger = GetLogger(moduleName); SPDLOG_LOGGER_CRITICAL(logger, __VA_ARGS__); } } while(false)
