#include "utils/logger.hpp"
#include <mutex>

static std::once_flag g_init_flag;

void ensure_initialized() {
    std::call_once(g_init_flag, [] {
        spdlog::set_pattern("[%H:%M:%S] %^[%l]%$ %v");  // colored logging
        spdlog::set_level(spdlog::level::info);
        });
}

