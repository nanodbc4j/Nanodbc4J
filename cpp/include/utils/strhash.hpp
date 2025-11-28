#pragma once
#include <string_view>

constexpr auto hash_djb2a(const std::string_view sv) {
    unsigned long hash{ 5381 };
    for (const unsigned char c : sv) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}

constexpr auto operator"" _sh(const char* str, const size_t len) {
    return hash_djb2a(std::string_view{ str, len });
}

constexpr auto hash_djb2a(const std::wstring_view sv) {
    unsigned long hash{ 5381 };
    for (const wchar_t c : sv) {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash;
}

constexpr auto operator"" _sh(const wchar_t* str, const size_t len) {
    return hash_djb2a(std::wstring_view{ str, len });
}