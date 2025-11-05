#include "utils/string_utils.hpp"
#include <type_traits>
#include <memory>
#include <cstring>
#include <vector>
#include <algorithm>
#include <exception>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/xchar.h>
#include <utf8.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
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

std::string utils::to_string(const std::string& str) {
    return str;
}

std::string utils::to_string(const std::wstring& str) {
#ifdef _WIN32
    // Windows: wstring is UTF-16
    static_assert(sizeof(wchar_t) == 2, "wchar_t must be 16-bit on Windows");
    std::string result;
    utf8::utf16to8(str.begin(), str.end(), std::back_inserter(result));
    return result;
#else
    // Linux/macOS: wstring is UTF-32
    static_assert(sizeof(wchar_t) == 4, "wchar_t must be 32-bit on Unix-like systems");
    std::string result;
    utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
    return result;
#endif
}

std::string utils::to_string(const std::u16string& str) {
    std::string result;
    utf8::utf16to8(str.begin(), str.end(), std::back_inserter(result));
    return result;
}

std::string utils::to_string(const std::u32string& str) {
    std::string result;
    utf8::utf32to8(str.begin(), str.end(), std::back_inserter(result));
    return result;
}

std::wstring utils::to_wstring(const std::u16string& str) {
    LOG_TRACE("str={}", !str.empty() ? utils::to_string(str) : "(empty)");
    if (str.empty()) return std::wstring();

#ifdef _WIN32
    // На Windows wchar_t == 16 бит, можно копировать напрямую
    LOG_TRACE("Platform: Windows, direct cast (UTF-16 -> wchar_t)");
    return std::wstring(reinterpret_cast<const wchar_t*>(str.c_str()), str.length());
#else
    // На Linux wchar_t == 32 бита, нужен конвертер с расширением surrogate pairs
    LOG_TRACE("Platform: Unix, converting UTF-16 to UTF-32 wchar_t");
    std::wstring result;
    result.reserve(str.size() * 2);
    for (size_t i = 0; i < str.length(); ++i) {
        char16_t ch = str[i];
        // Обработка суррогатных пар (UTF-16 -> UTF-32)
        if (ch >= 0xD800 && ch <= 0xDBFF && i + 1 < str.length()) {
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

std::wstring utils::to_wstring(const std::string& str) {
    LOG_TRACE("str={}", !str.empty() ? str : "(empty)");
    if (str.empty()) return std::wstring();

    //Replaces all invalid sequences
    std::string cleaned;
    utf8::replace_invalid(str.begin(), str.end(), std::back_inserter(cleaned));

#ifdef _WIN32
    // Windows: wstring is UTF-16
    static_assert(sizeof(wchar_t) == 2, "wchar_t must be 16-bit on Windows");
    std::wstring result;
    utf8::utf8to16(cleaned.begin(), cleaned.end(), std::back_inserter(result));
    return result;
#else
    // Linux/macOS: wstring is UTF-32
    static_assert(sizeof(wchar_t) == 4, "wchar_t must be 32-bit on Unix-like systems");
    std::wstring result;
    utf8::utf8to32(cleaned.begin(), cleaned.end(), std::back_inserter(result));
    return result;
#endif
}

std::wstring utils::to_wstring(const std::wstring& str) {
    return str;
}

std::u16string utils::to_u16string(const std::string& str) {
    LOG_TRACE("input string length = {}", str.length());
    //Replaces all invalid sequences
    std::string cleaned = utf8::replace_invalid(str);
    return utf8::utf8to16(cleaned);
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
    result.reserve(str.size() * 2);

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
    LOG_TRACE("input wstring length = {}", str.length());

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
    result.reserve(str.size() * 2);

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

std::u16string utils::to_u16string(const std::u16string& str) {
    return str;
}


#ifdef WIN32
std::string utils::utf8_to_ansi(const std::string& utf8) {
    int wn = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), NULL, 0);
    LPWSTR pwB = new WCHAR[wn];
    wn = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), utf8.size(), pwB, wn);

    int an = WideCharToMultiByte(CP_ACP, 0, pwB, wn, NULL, 0, NULL, NULL);
    LPSTR pB = new CHAR[an];
    an = WideCharToMultiByte(CP_ACP, 0, pwB, wn, pB, an, NULL, NULL);

    std::string tmp;
    tmp.assign(pB, an);

    delete[] pwB;
    delete[] pB;

    return tmp;
}

std::string utils::ansi_to_utf8(const std::string& ansi) {
    int wn = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), ansi.size(), NULL, 0);
    LPWSTR pwB = new WCHAR[wn];
    wn = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), ansi.size(), pwB, wn);

    int an = WideCharToMultiByte(CP_UTF8, 0, pwB, wn, NULL, 0, NULL, NULL);
    LPSTR pB = new CHAR[an];
    an = WideCharToMultiByte(CP_UTF8, 0, pwB, wn, pB, an, NULL, NULL);

    std::string tmp;
    tmp.assign(pB, an);

    delete[] pwB;
    delete[] pB;

    return tmp;
}
#endif

std::string utils::to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { 
        return std::tolower(c);
    });
    return str;
}

template<>
std::string utils::join_strings<std::vector<std::string>>(const std::vector<std::string>& c, const std::string& delimiter) {
    return fmt::format("{}", fmt::join(c, delimiter));
}

template<>
std::wstring utils::join_strings<std::vector<std::wstring>>(const std::vector<std::wstring>& c, const std::wstring& delimiter) {
    return fmt::format(L"{}", fmt::join(c, delimiter));
}

template<>
std::u16string utils::join_strings<std::vector<std::u16string>>(const std::vector<std::u16string>& c, const std::u16string& delimiter) {
    if (c.empty()) return {};
    std::u16string result = c[0];
    for (size_t i = 1; i < c.size(); ++i) {
        result += delimiter;
        result += c[i];
    }
    return result;
}

template<>
std::u32string utils::join_strings<std::vector<std::u32string>>(const std::vector<std::u32string>& c, const std::u32string& delimiter) {
    if (c.empty()) return {};
    std::u32string result = c[0];
    for (size_t i = 1; i < c.size(); ++i) {
        result += delimiter;
        result += c[i];
    }
    return result;
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
