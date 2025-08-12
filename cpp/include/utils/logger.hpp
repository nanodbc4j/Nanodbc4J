#pragma once
#include "spdlog/spdlog.h"
#include "utils/string_utils.hpp"

// Объявление функции инициализации логгера
void ensure_initialized();

// Ensure initialization before logging
#define LOG_WITH_INIT(level, ...) \
    do { \
        ensure_initialized(); \
        spdlog::log(level, __VA_ARGS__); \
    } while(0)

// Logging macros
#define LOG_TRACE(...)    LOG_WITH_INIT(spdlog::level::trace, "[{}:{}] {}():\t{}", __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_DEBUG(...)    LOG_WITH_INIT(spdlog::level::debug, "[{}:{}] {}():\t{}", __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_INFO(...)     LOG_WITH_INIT(spdlog::level::info,  "[{}:{}] {}():\t{}", __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_WARN(...)     LOG_WITH_INIT(spdlog::level::warn,  "[{}:{}] {}():\t{}", __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_ERROR(...)    LOG_WITH_INIT(spdlog::level::error, "[{}:{}] {}():\t{}", __FILE__, __LINE__, __func__, __VA_ARGS__)
#define LOG_CRITICAL(...) LOG_WITH_INIT(spdlog::level::critical, "[{}:{}] {}():\t{}", __FILE__, __LINE__, __func__, __VA_ARGS__)

#define LOG_TRACE_W(...)    LOG_WITH_INIT(spdlog::level::trace, L"[{}:{}] {}()", utils::to_wstring(__FILE__), __LINE__, utils::to_wstring(__func__), __VA_ARGS__)
#define LOG_DEBUG_W(...)    LOG_WITH_INIT(spdlog::level::debug, L"[{}:{}] {}():\t{}", utils::to_wstring(__FILE__), __LINE__, utils::to_wstring(__func__), __VA_ARGS__)
#define LOG_INFO_W(...)     LOG_WITH_INIT(spdlog::level::info,  L"[{}:{}] {}():\t{}", utils::to_wstring(__FILE__), __LINE__, utils::to_wstring(__func__), __VA_ARGS__)
#define LOG_WARN_W(...)     LOG_WITH_INIT(spdlog::level::warn,  L"[{}:{}] {}():\t{}", utils::to_wstring(__FILE__), __LINE__, utils::to_wstring(__func__), __VA_ARGS__)
#define LOG_ERROR_W(...)    LOG_WITH_INIT(spdlog::level::error, L"[{}:{}] {}():\t{}", utils::to_wstring(__FILE__), __LINE__, utils::to_wstring(__func__), __VA_ARGS__)
#define LOG_CRITICAL_W(...) LOG_WITH_INIT(spdlog::level::critical, L"[{}:{}] {}():\t{}", utils::to_wstring(__FILE__), __LINE__, utils::to_wstring(__func__), __VA_ARGS__)