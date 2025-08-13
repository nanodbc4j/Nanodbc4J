#pragma once
#include "spdlog/spdlog.h"
#include "utils/string_utils.hpp"
#include "utils/path_utils.h"

// Объявление функции инициализации логгера
void ensure_initialized();

// Ensure initialization before logging
#define LOG_WITH_INIT(level, ...) \
    do { \
        ensure_initialized(); \
        spdlog::log(level, __VA_ARGS__); \
    } while(0)

#define FMT_STRING_FORMAT(str, ...) fmt::format(fmt::runtime(str), __VA_ARGS__)
#define __FILENAME__ utils::extract_filename(__FILE__)

// Logging macros
#define LOG_TRACE(...)    LOG_WITH_INIT(spdlog::level::trace, "[{}:{}] {}():\t{}", __FILENAME__, __LINE__, __func__, FMT_STRING_FORMAT(__VA_ARGS__))
#define LOG_DEBUG(...)    LOG_WITH_INIT(spdlog::level::debug, "[{}:{}] {}():\t{}", __FILENAME__, __LINE__, __func__, FMT_STRING_FORMAT(__VA_ARGS__))
#define LOG_INFO(...)     LOG_WITH_INIT(spdlog::level::info,  "[{}:{}] {}():\t{}", __FILENAME__, __LINE__, __func__, FMT_STRING_FORMAT(__VA_ARGS__))
#define LOG_WARN(...)     LOG_WITH_INIT(spdlog::level::warn,  "[{}:{}] {}():\t{}", __FILENAME__, __LINE__, __func__, FMT_STRING_FORMAT(__VA_ARGS__))
#define LOG_ERROR(...)    LOG_WITH_INIT(spdlog::level::error, "[{}:{}] {}():\t{}", __FILENAME__, __LINE__, __func__, FMT_STRING_FORMAT(__VA_ARGS__))
#define LOG_CRITICAL(...) LOG_WITH_INIT(spdlog::level::critical, "[{}:{}] {}():\t{}", __FILENAME__, __LINE__, __func__, FMT_STRING_FORMAT(__VA_ARGS__))

#define LOG_TRACE_W(...)    LOG_WITH_INIT(spdlog::level::trace, L"[{}:{}] {}()", utils::to_wstring(__FILENAME__), __LINE__, utils::to_wstring(__func__), FMT_STRING_FORMAT(__VA_ARGS__))
#define LOG_DEBUG_W(...)    LOG_WITH_INIT(spdlog::level::debug, L"[{}:{}] {}():\t{}", utils::to_wstring(__FILENAME__), __LINE__, utils::to_wstring(__func__), FMT_STRING_FORMAT(__VA_ARGS__))
#define LOG_INFO_W(...)     LOG_WITH_INIT(spdlog::level::info,  L"[{}:{}] {}():\t{}", utils::to_wstring(__FILENAME__), __LINE__, utils::to_wstring(__func__), FMT_STRING_FORMAT(__VA_ARGS__))
#define LOG_WARN_W(...)     LOG_WITH_INIT(spdlog::level::warn,  L"[{}:{}] {}():\t{}", utils::to_wstring(__FILENAME__), __LINE__, utils::to_wstring(__func__), FMT_STRING_FORMAT(__VA_ARGS__))
#define LOG_ERROR_W(...)    LOG_WITH_INIT(spdlog::level::error, L"[{}:{}] {}():\t{}", utils::to_wstring(__FILENAME__), __LINE__, utils::to_wstring(__func__), FMT_STRING_FORMAT(__VA_ARGS__))
#define LOG_CRITICAL_W(...) LOG_WITH_INIT(spdlog::level::critical, L"[{}:{}] {}():\t{}", utils::to_wstring(__FILENAME__), __LINE__, utils::to_wstring(__func__), FMT_STRING_FORMAT(__VA_ARGS__))