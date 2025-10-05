#include "utils/string_utils.hpp"
#include <type_traits>
#include <memory>
#include <cstring>
#include <vector>
#include <exception>
#include "utf8.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "utils/logger.hpp"

namespace {

class MallocException : public std::exception {
    const char* what() const noexcept override {
        return "Memory allocation failure: insufficient memory or heap corruption";
    }
};

class StringOverflowException : public std::exception {
    const char* what() const noexcept override {
        return "String too long, potential overflow";
    }
};

} // namespace


std::wstring utils::to_wstring(const char16_t* str) {
    LOG_TRACE("str={}", str ? reinterpret_cast<const char*>(str) : "(null)");
    if (!str) return std::wstring();
    size_t length = std::char_traits<char16_t>::length(str);
    return utils::to_wstring(str, length);
}

std::wstring utils::to_wstring(const char16_t* str, size_t length) {
    LOG_TRACE("str={}", str ? reinterpret_cast<const char*>(str) : "(null)");
    if (!str) return std::wstring();

#if defined(_WIN32) || defined(_WIN64)
    // На Windows wchar_t == 16 бит, можно копировать напрямую
    LOG_TRACE("Platform: Windows, direct cast (UTF-16 -> wchar_t)");
    return std::wstring(reinterpret_cast<const wchar_t*>(str), length);
#else
    // На Linux wchar_t == 32 бита, нужен конвертер с расширением surrogate pairs
    LOG_TRACE("Platform: Unix, converting UTF-16 to UTF-32 wchar_t");
    std::wstring result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        char16_t ch = str[i];
        // Обработка суррогатных пар (UTF-16 -> UTF-32)
        if (ch >= 0xD800 && ch <= 0xDBFF && i + 1 < length) {
            char16_t ch2 = str[i + 1];
            if (ch2 >= 0xDC00 && ch2 <= 0xDFFF) {
                // Парная суррогатная пара
                wchar_t codepoint = (((ch - 0xD800) << 10) | (ch2 - 0xDC00)) + 0x10000;
                result.push_back(codepoint);
                ++i; // пропустить второй элемент пары
                continue;
            }
        }
        result.push_back(static_cast<wchar_t>(ch));
    }
    return result;
#endif
}

std::wstring utils::to_wstring(const std::u16string& str) {
    LOG_TRACE_W(L"str length = {}", str.length());
    return to_wstring(str.c_str(), str.length());
}

std::wstring utils::to_wstring(const char* str) {
    LOG_TRACE("str={}", str ? str : "(null)");
    if (!str) return std::wstring();
#ifdef _WIN32
    // На Windows используем MultiByteToWideChar для UTF-8
    LOG_TRACE("Platform: Windows, using MultiByteToWideChar (UTF-8 -> UTF-16)");
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    if (size_needed == 0) return std::wstring();
    std::wstring wstr(size_needed - 1, 0); // -1 чтобы убрать нуль-терминатор
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], size_needed);
    return wstr;
#else
    // На Linux считаем, что str в UTF-8, используем mbstowcs
    LOG_TRACE("Platform: Unix, using mbsrtowcs (UTF-8 -> wchar_t)");
    std::mbstate_t state = std::mbstate_t();
    size_t len = std::mbsrtowcs(nullptr, &str, 0, &state);
    if (len == (size_t)-1) return std::wstring();
    std::vector<wchar_t> buf(len + 1);
    state = std::mbstate_t();
    std::mbsrtowcs(buf.data(), &str, len + 1, &state);
    return std::wstring(buf.data());
#endif
}

std::wstring utils::to_wstring(const std::string& str) {
    LOG_TRACE("string length = {}", str.length());
    return to_wstring(str.c_str());
}

std::wstring utils::to_wstring(std::string_view str) {
    LOG_TRACE("string_view length = {}", str.length());
    return to_wstring(str.data());
}

std::wstring utils::to_wstring(std::wstring_view str) {
    LOG_TRACE_W(L"wstring_view length = {}", str.length());
    return std::wstring(str);
}

std::u16string utils::to_u16string(const std::string& str) {
    LOG_TRACE("input string length = {}", str.length());
    std::u16string result;
    utf8::utf8to16(str.begin(), str.end(), std::back_inserter(result));
    return result;
}

std::u16string utils::to_u16string(const std::u32string& str) {
    LOG_TRACE("u32string length = {}", str.length());

    // Unicode code point boundaries (from the Unicode Standard)
    constexpr char32_t UNICODE_MAX_CODEPOINT = 0x10FFFF;
    constexpr char32_t BMP_MAX               = 0xFFFF;
    constexpr char32_t SURROGATE_MIN         = 0xD800;
    constexpr char32_t SURROGATE_MAX         = 0xDFFF;
    constexpr char32_t REPLACEMENT_CHAR      = 0xFFFD;
    constexpr char32_t ASTRAL_PLANE_START    = 0x10000;

    // Reserve approximate capacity: each code point becomes 1 or 2 char16_t units.
    std::u16string result;
    result.reserve(str.size());

    for (char32_t codepoint : str) {
        if (codepoint <= BMP_MAX) {
            // Basic Multilingual Plane (U+0000 to U+FFFF)
            if (codepoint >= SURROGATE_MIN && codepoint <= SURROGATE_MAX) {
                // Surrogates are invalid in UTF-32 → replace with U+FFFD
                result.push_back(static_cast<char16_t>(REPLACEMENT_CHAR));
            } else {
                result.push_back(static_cast<char16_t>(codepoint));
            }
        } else if (codepoint <= UNICODE_MAX_CODEPOINT) {
            // Astral planes: U+10000 to U+10FFFF → encode as surrogate pair
            char32_t offset = codepoint - ASTRAL_PLANE_START; // 20-bit value (0x00000–0xFFFFF)
            char16_t high_surrogate = static_cast<char16_t>((offset >> 10) + 0xD800);
            char16_t low_surrogate  = static_cast<char16_t>((offset & 0x3FF) + 0xDC00);
            result.push_back(high_surrogate);
            result.push_back(low_surrogate);
        } else {
            // Invalid code point (> U+10FFFF)
            result.push_back(static_cast<char16_t>(REPLACEMENT_CHAR));
        }
    }

    return result;
}

std::u16string utils::to_u16string(const std::wstring& str) {
    LOG_TRACE_W(L"input wstring length = {}", str.length());

#ifdef _WIN32
    // On Windows, wchar_t is UTF-16 (16 bits)
    LOG_TRACE("Platform: Windows, wchar_t is UTF-16, direct cast");
    return std::u16string(reinterpret_cast<const char16_t*>(str.c_str()), str.size());
#else
    // On Unix-like systems, wchar_t is UTF-32 (32 bits)
    LOG_TRACE("Platform: Unix, converting UTF-32 wchar_t to UTF-16");

    // Reuse the same logic as to_u16string(const std::u32string&)
    // by treating wstring as u32string (since wchar_t == char32_t on Unix)
    constexpr char32_t UNICODE_MAX_CODEPOINT = 0x10FFFF;
    constexpr char32_t BMP_MAX               = 0xFFFF;
    constexpr char32_t SURROGATE_MIN         = 0xD800;
    constexpr char32_t SURROGATE_MAX         = 0xDFFF;
    constexpr char32_t REPLACEMENT_CHAR      = 0xFFFD;
    constexpr char32_t ASTRAL_PLANE_START    = 0x10000;

    std::u16string result;
    result.reserve(str.size());

    for (wchar_t wc : str) {
        char32_t codepoint = static_cast<char32_t>(wc);

        if (codepoint <= BMP_MAX) {
            if (codepoint >= SURROGATE_MIN && codepoint <= SURROGATE_MAX) {
                result.push_back(static_cast<char16_t>(REPLACEMENT_CHAR));
            } else {
                result.push_back(static_cast<char16_t>(codepoint));
            }
        } else if (codepoint <= UNICODE_MAX_CODEPOINT) {
            char32_t offset = codepoint - ASTRAL_PLANE_START;
            result.push_back(static_cast<char16_t>((offset >> 10) + 0xD800));
            result.push_back(static_cast<char16_t>((offset & 0x3FF) + 0xDC00));
        } else {
            result.push_back(static_cast<char16_t>(REPLACEMENT_CHAR));
        }
    }

    return result;
#endif
}

template <typename CharT>
CharT* utils::duplicate_string(const CharT* src) {
    LOG_TRACE("src={}", src ? "non-null" : "null");
    if (!src) return nullptr;

    size_t length = std::char_traits<CharT>::length(src);

    return utils::duplicate_string(src, length);
}

template <typename CharT>
CharT* utils::duplicate_string(const CharT* src, size_t length) {
    LOG_TRACE("src={}", src ? "non-null" : "null");
    if (!src) return nullptr;

    // Защита от переполнения
    constexpr size_t max_allowed = std::numeric_limits<size_t>::max() / sizeof(CharT);
    if (length > max_allowed) {
        LOG_ERROR("String length {} exceeds maximum allowed {}", length, max_allowed);
        throw StringOverflowException();
    }

    CharT* dest = static_cast<CharT*>(malloc(sizeof(CharT) * (length + 1)));
    if (!dest) {
        LOG_TRACE("malloc failed");
        throw MallocException();
    }

    std::char_traits<CharT>::copy(dest, src, length + 1);

    LOG_TRACE("String duplicated, length = {}", length);
    return dest;
}

template char* utils::duplicate_string(const char* src);
template char16_t* utils::duplicate_string(const char16_t* src);
template wchar_t* utils::duplicate_string(const wchar_t* src);
template char* utils::duplicate_string(const char* src, size_t length);
template char16_t* utils::duplicate_string(const char16_t* src, size_t length);
template wchar_t* utils::duplicate_string(const wchar_t* src, size_t length);
