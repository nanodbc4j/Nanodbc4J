#pragma once
#include <string_view>
#include <algorithm>

namespace utils {

    constexpr std::string_view extract_filename(std::string_view path) noexcept {
        if (path.empty()) return std::string_view{};

        // Находим последний слеш или обратный слеш
        size_t last_slash = std::string_view::npos;
        for (size_t i = path.size(); i-- > 0;) {
            if (path[i] == '/' || path[i] == '\\') {
                last_slash = i;
                break;
            }
        }

        if (last_slash == std::string_view::npos) {
            return path; // Если нет слешей, возвращаем всю строку
        }

        return path.substr(last_slash + 1);
    }

    // Для C - строк(const char*)
    constexpr std::string_view extract_filename(const char* path) noexcept {
        return path ? extract_filename(std::string_view(path)) : std::string_view{};
    }

    constexpr std::wstring_view extract_filename(std::wstring_view path) noexcept {
        if (path.empty()) return std::wstring_view{};

        // Находим последний слеш или обратный слеш
        size_t last_slash = std::wstring_view::npos;
        for (size_t i = path.size(); i-- > 0;) {
            if (path[i] == L'/' || path[i] == L'\\') {
                last_slash = i;
                break;
            }
        }

        if (last_slash == std::wstring_view::npos) {
            return path; // Если нет слешей, возвращаем всю строку
        }

        return path.substr(last_slash + 1);
    }

    // Для C-строк (const wchar_t*)
    constexpr std::wstring_view extract_filename(const wchar_t* path) noexcept {
        return path ? extract_filename(std::wstring_view(path)) : std::wstring_view{};
    }

} // namespace utils