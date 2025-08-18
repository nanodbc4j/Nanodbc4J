#pragma once
#include <string_view>
#include <algorithm>

namespace utils {

    constexpr std::string_view extract_filename(std::string_view path) {
        if (path.empty()) return "";

        // Находим последний слеш или обратный слеш
        auto last_slash = path.find_last_of("/\\");
        if (last_slash == std::string_view::npos) {
            return path; // Если нет слешей, возвращаем всю строку
        }

        return path.substr(last_slash + 1);
    }

    // Перегрузка для C-строк
    constexpr std::string_view extract_filename(const char* path) {
        return path ? extract_filename(std::string_view(path)) : "";
    }

} // namespace utils