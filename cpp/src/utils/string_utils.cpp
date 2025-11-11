#include "utils/string_utils.hpp"
#include <vector>
#include <algorithm>
#include <exception>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fmt/xchar.h>
#include <utf8.h>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "utils/logger.hpp"

namespace {

class MallocException : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "Memory allocation failure: insufficient memory or heap corruption";
    }
};

class StringOverflowException : public std::exception {
    [[nodiscard]] const char* what() const noexcept override {
        return "String too long, potential overflow";
    }
};

} // namespace

namespace utils::unicode {

    // Code point limits
    inline constexpr char32_t MAX_CODEPOINT        = 0x10FFFF;
    inline constexpr char32_t BMP_MAX              = 0xFFFF;
    inline constexpr char32_t SURROGATE_MIN        = 0xD800;
    inline constexpr char32_t SURROGATE_MAX        = 0xDFFF;
    inline constexpr char32_t REPLACEMENT_CHAR     = 0xFFFD;
    inline constexpr char32_t ASTRAL_PLANE_START   = 0x10000;

    // Surrogate encoding specifics
    inline constexpr char16_t HIGH_SURROGATE_MIN   = 0xD800;
    inline constexpr char16_t HIGH_SURROGATE_MAX   = 0xDBFF;
    inline constexpr char16_t LOW_SURROGATE_MIN    = 0xDC00;
    inline constexpr char16_t LOW_SURROGATE_MAX    = 0xDFFF;

    inline constexpr char32_t SURROGATE_OFFSET     = ASTRAL_PLANE_START;  // 0x10000
    inline constexpr char32_t HIGH_SURROGATE_BASE  = HIGH_SURROGATE_MIN;  // 0xD800
    inline constexpr char32_t LOW_SURROGATE_BASE   = LOW_SURROGATE_MIN;   // 0xDC00
    inline constexpr int      SURROGATE_SHIFT_BITS = 10;
    inline constexpr char32_t SURROGATE_MASK       = 0x3FF;  // = 0b11'1111'1111

}  // namespace utils::unicode

std::string utils::to_string(const std::string& str) {
    return str;
}

std::string utils::to_string(const std::wstring& str) {
#ifdef _WIN32
    // Windows: wstring is UTF-16
    static_assert(sizeof(wchar_t) == sizeof(char16_t), "wchar_t must be 16-bit on Windows");
    static_assert(alignof(wchar_t) == alignof(char16_t), "alignment mismatch");
    std::string result;
    result.reserve(str.size() * 2);
    utf8::unchecked::utf16to8(str.begin(), str.end(), std::back_inserter(result));
    return result;
#else
    // Linux/macOS: wstring is UTF-32
    static_assert(sizeof(wchar_t) == sizeof(char32_t), "wchar_t must be 32-bit on this platform");
    static_assert(alignof(wchar_t) == alignof(char32_t), "alignment mismatch");
    std::string result;
    result.reserve(str.size() * 4);
    utf8::unchecked::utf32to8(str.begin(), str.end(), std::back_inserter(result));
    return result;
#endif
}

std::string utils::to_string(const std::u16string& str) {
    std::string result;
    result.reserve(str.size() * 3);
    utf8::unchecked::utf16to8(str.begin(), str.end(), std::back_inserter(result));
    return result;
}

std::string utils::to_string(const std::u32string& str) {
    std::string result;
    result.reserve(str.size() * 4);
    utf8::unchecked::utf32to8(str.begin(), str.end(), std::back_inserter(result));
    return result;
}

std::wstring utils::to_wstring(const std::u16string& str) {
    LOG_TRACE("str={}", !str.empty() ? utils::to_string(str) : "(empty)");
    if (str.empty()) return {};

#ifdef _WIN32
    // On Windows wchar_t == 16 bits, can copy directly
    LOG_TRACE("Platform: Windows, direct cast (UTF-16 -> wchar_t)");
    return std::wstring(reinterpret_cast<const wchar_t*>(str.c_str()), str.length());
#else
    // On Linux wchar_t == 32 bits, needs converter with surrogate pairs expansion
    LOG_TRACE("Platform: Unix, converting UTF-16 to UTF-32 wchar_t");

    using namespace unicode;

    std::wstring result;
    result.reserve(str.size() * 2);
    for (size_t i = 0; i < str.length(); ++i) {
        const char16_t ch = str[i];
        // Handle surrogate pairs (UTF-16 -> UTF-32)
        if (ch >= HIGH_SURROGATE_MIN && ch <= HIGH_SURROGATE_MAX && i + 1 < str.length()) {
            char16_t ch2 = str[i + 1];
            if (ch2 >= LOW_SURROGATE_MIN && ch2 <= LOW_SURROGATE_MAX) {
                // Valid surrogate pair → decode to full code point
                const auto high = static_cast<char32_t>(ch - HIGH_SURROGATE_BASE);
                const auto low  = static_cast<char32_t>(ch2 - LOW_SURROGATE_BASE);
                const wchar_t codepoint = static_cast<wchar_t>(high << SURROGATE_SHIFT_BITS | low) + static_cast<wchar_t>(SURROGATE_OFFSET);
                result.push_back(codepoint);
                ++i; // skip the second element of the pair
                continue;
            }
        }
        result.push_back(ch);
    }
    return result;
#endif
}

std::wstring utils::to_wstring(const std::string& str) {
    LOG_TRACE("str={}", !str.empty() ? str : "(empty)");
    if (str.empty()) return {};

    //Replaces all invalid sequences
    std::string cleaned;
    cleaned.reserve(str.size());
    utf8::unchecked::replace_invalid(str.begin(), str.end(), std::back_inserter(cleaned));

#ifdef _WIN32
    // Windows: wstring is UTF-16
    static_assert(sizeof(wchar_t) == sizeof(char16_t), "wchar_t must be 16-bit on Windows");
    static_assert(alignof(wchar_t) == alignof(char16_t), "alignment mismatch");
    std::wstring result;
    result.reserve(cleaned.size());
    utf8::unchecked::utf8to16(cleaned.begin(), cleaned.end(), std::back_inserter(result));
    return result;
#else
    // Linux/macOS: wstring is UTF-32
    static_assert(sizeof(wchar_t) == sizeof(char32_t), "wchar_t must be 32-bit on this platform");
    static_assert(alignof(wchar_t) == alignof(char32_t), "alignment mismatch");
    std::wstring result;
    result.reserve(cleaned.size());
    utf8::unchecked::utf8to32(cleaned.begin(), cleaned.end(), std::back_inserter(result));
    return result;
#endif
}

std::wstring utils::to_wstring(const std::wstring& str) {
    return str;
}

std::u16string utils::to_u16string(const std::string& str) {
    LOG_TRACE("input string length = {}", str.length());
    //Replaces all invalid sequences
    std::string cleaned;
    cleaned.reserve(str.size());
    utf8::unchecked::replace_invalid(str.begin(), str.end(), std::back_inserter(cleaned));

    std::u16string result;
    result.reserve(cleaned.size());
    utf8::unchecked::utf8to16(cleaned.begin(), cleaned.end(), std::back_inserter(result));
    return result;
}

std::u16string utils::to_u16string(const std::u32string& str) {
    LOG_TRACE("u32string length = {}", str.length());

    using namespace unicode;

    // Reserve approximate capacity: each code point becomes 1 or 2 char16_t units.
    std::u16string result;
    result.reserve(str.size() * 2);

    for (const char32_t codepoint : str) {
        if (codepoint <= BMP_MAX) {
            // Basic Multilingual Plane (U+0000 to U+FFFF)
            if (codepoint >= SURROGATE_MIN && codepoint <= SURROGATE_MAX) {
                // Surrogates are invalid in UTF-32 → replace with U+FFFD
                result.push_back(REPLACEMENT_CHAR);
            } else {
                result.push_back(static_cast<char16_t>(codepoint));
            }
        } else if (codepoint <= MAX_CODEPOINT) {
            // Astral planes: U+10000 to U+10FFFF → encode as surrogate pair
            const char32_t offset = codepoint - SURROGATE_OFFSET; // 20-bit value (0x00000–0xFFFFF)
            const auto high_surrogate = static_cast<char16_t>((offset >> SURROGATE_SHIFT_BITS) + HIGH_SURROGATE_BASE);
            const auto low_surrogate  = static_cast<char16_t>((offset & SURROGATE_MASK) + LOW_SURROGATE_BASE);
            result.push_back(high_surrogate);
            result.push_back(low_surrogate);
        } else {
            // Invalid code point (> U+10FFFF)
            result.push_back(REPLACEMENT_CHAR);
        }
    }

    return result;
}

std::u16string utils::to_u16string(const std::wstring& str) {
    LOG_TRACE("input wstring length = {}", str.length());

#ifdef _WIN32
    // On Windows, wchar_t is UTF-16 (16 bits)
    LOG_TRACE("Platform: Windows, wchar_t is UTF-16, direct cast");
    static_assert(sizeof(wchar_t) == sizeof(char16_t), "wchar_t must be 16-bit on Windows");
    static_assert(alignof(wchar_t) == alignof(char16_t), "alignment mismatch");
    return std::u16string(reinterpret_cast<const char16_t*>(str.c_str()), str.size());
#else
    // On Unix-like systems, wchar_t is UTF-32 (32 bits)
    LOG_TRACE("Platform: Unix, converting UTF-32 wchar_t to UTF-16");

    // Reuse the same logic as to_u16string(const std::u32string&)
    // by treating wstring as u32string (since wchar_t == char32_t on Unix)
    static_assert(sizeof(wchar_t) == sizeof(char32_t), "wchar_t must be 32-bit on this platform");
    static_assert(alignof(wchar_t) == alignof(char32_t), "alignment mismatch");

    const std::u32string u32str{reinterpret_cast<const char32_t*>(str.c_str()), str.size()};
    return to_u16string(u32str);
#endif
}

std::u16string utils::to_u16string(const std::u16string& str) {
    return str;
}


#ifdef WIN32
std::string utils::wstring_to_ansi(const std::wstring& wstr) {
    if (wstr.empty()) return {};   

    int len = WideCharToMultiByte(CP_ACP, 0, wstr.data(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    if (len <= 0) {
        DWORD err = GetLastError();
        LOG_ERROR("WideCharToMultiByte(CP_ACP) length query failed, error={}", err);
        throw std::runtime_error(fmt::format("wstring_to_ansi length query failed (error {})", err));
    }
    std::string result(len, 0);
    int written = WideCharToMultiByte(CP_ACP, 0, wstr.data(), static_cast<int>(wstr.size()), result.data(), len, nullptr, nullptr);
    if (written != len) {
        DWORD err = GetLastError();
        LOG_ERROR("WideCharToMultiByte(CP_ACP) conversion failed, expected={}, written={}, error={}", len, written, err);
        throw std::runtime_error(fmt::format("wstring_to_ansi conversion failed (error {})", err));
    }
    return result;
}

std::wstring utils::ansi_to_wstring(const std::string& ansi) {
    if (ansi.empty()) return {};

    int wlen = MultiByteToWideChar(CP_ACP, 0, ansi.data(), static_cast<int>(ansi.size()), nullptr, 0);
    if (wlen <= 0) {
        DWORD err = GetLastError();
        LOG_ERROR("MultiByteToWideChar(CP_ACP) length query failed, error={}", err);
        throw std::runtime_error(fmt::format("ansi_to_wstring length query failed (error {})", err));
    }
    std::wstring result(wlen, 0);
    int written = MultiByteToWideChar(CP_ACP, 0, ansi.data(), static_cast<int>(ansi.size()), result.data(), wlen);
    if (written != wlen) {
        DWORD err = GetLastError();
        LOG_ERROR("MultiByteToWideChar(CP_ACP) conversion failed, expected={}, written={}, error={}", wlen, written, err);
        throw std::runtime_error(fmt::format("ansi_to_wstring conversion failed (error {})", err));
    }
    return result;
}
#endif

std::string utils::to_lower(std::string str) {
    std::ranges::transform(str, str.begin(), [](unsigned char c) {
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

    // Overflow protection
    constexpr size_t max_allowed = std::numeric_limits<size_t>::max() / sizeof(CharT);
    if (length > max_allowed) {
        LOG_ERROR("String length {} exceeds maximum allowed {}", length, max_allowed);
        throw StringOverflowException();
    }

    auto* dest = static_cast<CharT*>(malloc(sizeof(CharT) * (length + 1)));
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
