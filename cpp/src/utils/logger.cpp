#include "utils/logger.hpp"
#include <mutex>

static std::once_flag g_init_flag;

std::string set_spdlog_level(int level) {
    ensure_initialized();

    spdlog::level::level_enum spdlog_level;

    switch (level) {
        case SPDLOG_LEVEL_TRACE:    spdlog_level = spdlog::level::trace; break;
        case SPDLOG_LEVEL_DEBUG:    spdlog_level = spdlog::level::debug; break;
        case SPDLOG_LEVEL_INFO:     spdlog_level = spdlog::level::info; break;
        case SPDLOG_LEVEL_WARN:     spdlog_level = spdlog::level::warn; break;
        case SPDLOG_LEVEL_ERROR:    spdlog_level = spdlog::level::err; break;
        case SPDLOG_LEVEL_CRITICAL: spdlog_level = spdlog::level::critical; break;
        case SPDLOG_LEVEL_OFF:      spdlog_level = spdlog::level::off; break;
        default:                    spdlog_level = spdlog::level::info; break;  // Default to info for unknown values
    }

    spdlog::set_level(spdlog_level);

    // Используем встроенную функцию spdlog для получения имени уровня
    const auto& sv = spdlog::level::to_string_view(spdlog_level);
    return std::string(sv.data(), sv.size());
}

void ensure_initialized() {
    std::call_once(g_init_flag, [] {
        spdlog::set_pattern("[%H:%M:%S] %^[%l]%$ %v");  // colored logging
        spdlog::set_level(spdlog::level::info);
        });
}