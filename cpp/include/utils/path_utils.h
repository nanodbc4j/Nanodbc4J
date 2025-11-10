#pragma once
#include <string_view>

namespace utils {

    constexpr std::string_view extract_filename(std::string_view path) noexcept {
        if (path.empty()) return std::string_view{};

        // Find the last forward slash or backslash
        size_t last_slash = std::string_view::npos;
        for (size_t i = path.size(); i-- > 0;) {
            if (path[i] == '/' || path[i] == '\\') {
                last_slash = i;
                break;
            }
        }

        if (last_slash == std::string_view::npos) {
            return path; // If no slashes found, return the entire string
        }

        return path.substr(last_slash + 1);
    }

    constexpr std::wstring_view extract_filename(std::wstring_view path) noexcept {
        if (path.empty()) return std::wstring_view{};

        // Find the last forward slash or backslash
        size_t last_slash = std::wstring_view::npos;
        for (size_t i = path.size(); i-- > 0;) {
            if (path[i] == L'/' || path[i] == L'\\') {
                last_slash = i;
                break;
            }
        }

        if (last_slash == std::wstring_view::npos) {
            return path; // If no slashes found, return the entire string
        }

        return path.substr(last_slash + 1);
    }

} // namespace utils