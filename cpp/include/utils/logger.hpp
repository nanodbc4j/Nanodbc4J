#pragma once
#include "spdlog/spdlog.h"
#include "utils/string_utils.hpp"
#include "utils/path_utils.h"

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)

#if defined(_MSC_VER) || defined(__GNUC__) || defined(__clang__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)
#else
#define __WFUNCTION__ utils::to_wstring(__func__)
#endif

#define __FILENAME__ utils::extract_filename(__FILE__)
#define __WFILENAME__ utils::extract_filename(WIDEN(__FILE__))

// Объявление функции инициализации логгера
void ensure_initialized();

// Установка уровня логирования
std::string set_spdlog_level(int level = SPDLOG_LEVEL_INFO);

// Ensure initialization before logging
#define LOG_WITH_INIT(level, ...) \
    do { \
        ensure_initialized(); \
        spdlog::log(level, __VA_ARGS__); \
    } while(0)


// Logging macros
#define LOG_TRACE(fmt, ...)    LOG_WITH_INIT(spdlog::level::trace, "[{}:{}] {}():\t" fmt, __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)    LOG_WITH_INIT(spdlog::level::debug, "[{}:{}] {}():\t" fmt, __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)     LOG_WITH_INIT(spdlog::level::info,  "[{}:{}] {}():\t" fmt, __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)     LOG_WITH_INIT(spdlog::level::warn,  "[{}:{}] {}():\t" fmt, __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)    LOG_WITH_INIT(spdlog::level::err, "[{}:{}] {}():\t" fmt, __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) LOG_WITH_INIT(spdlog::level::critical, "[{}:{}] {}():\t" fmt, __FILENAME__, __LINE__, __func__, ##__VA_ARGS__)

#define LOG_TRACE_W(fmt, ...)    LOG_WITH_INIT(spdlog::level::trace, L"[{}:{}] {}():\t" fmt, __WFILENAME__, __LINE__, __WFUNCTION__, ##__VA_ARGS__)
#define LOG_DEBUG_W(fmt, ...)    LOG_WITH_INIT(spdlog::level::debug, L"[{}:{}] {}():\t" fmt, __WFILENAME__, __LINE__, __WFUNCTION__, ##__VA_ARGS__)
#define LOG_INFO_W(fmt, ...)     LOG_WITH_INIT(spdlog::level::info,  L"[{}:{}] {}():\t" fmt, __WFILENAME__, __LINE__, __WFUNCTION__, ##__VA_ARGS__)
#define LOG_WARN_W(fmt, ...)     LOG_WITH_INIT(spdlog::level::warn,  L"[{}:{}] {}():\t" fmt, __WFILENAME__, __LINE__, __WFUNCTION__, ##__VA_ARGS__)
#define LOG_ERROR_W(fmt, ...)    LOG_WITH_INIT(spdlog::level::err, L"[{}:{}] {}():\t" fmt, __WFILENAME__, __LINE__, __WFUNCTION__, ##__VA_ARGS__)
#define LOG_CRITICAL_W(fmt, ...) LOG_WITH_INIT(spdlog::level::critical, L"[{}:{}] {}():\t" fmt, __WFILENAME__, __LINE__, __WFUNCTION__, ##__VA_ARGS__)