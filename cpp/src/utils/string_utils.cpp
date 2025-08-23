#include "utils/string_utils.hpp"
#include <type_traits>
#include <memory>
#include <cstring>
#include <vector>
#include <codecvt>

#ifdef _WIN32
#include <windows.h>
#endif

#include "utils/logger.hpp"

wide_string_t utils::to_wide_string(const char16_t* string) {
    LOG_TRACE("string={}", string ? reinterpret_cast<const char*>(string) : "(null)");
    // Преобразуем входящую строку в wide_string (используется UTF-16LE)
    return std::basic_string<wide_char_t>(reinterpret_cast<const wide_char_t*>(string));
}

std::wstring utils::to_wstring(const char16_t* str) {
    LOG_TRACE("str={}", str ? reinterpret_cast<const char*>(str) : "(null)");
    if (!str) return std::wstring();

    size_t len = std::char_traits<char16_t>::length(str);

#if defined(_WIN32) || defined(_WIN64)
    // На Windows wchar_t == 16 бит, можно копировать напрямую
    LOG_TRACE("Platform: Windows, direct cast (UTF-16 -> wchar_t)");
    return std::wstring(reinterpret_cast<const wchar_t*>(str), len);
#else
    // На Linux wchar_t == 32 бита, нужен конвертер с расширением surrogate pairs
    LOG_TRACE("Platform: Unix, converting UTF-16 to UTF-32 wchar_t");
    std::wstring result;
    result.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        char16_t ch = str[i];
        // Обработка суррогатных пар (UTF-16 -> UTF-32)
        if (ch >= 0xD800 && ch <= 0xDBFF && i + 1 < len) {
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
    return to_wstring(str.c_str());
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
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
    return conv.from_bytes(str);
}

std::u16string utils::to_u16string(const std::u16string& str) {
    LOG_TRACE("u16string length = {}", str.length());
    return str;
}

std::u16string utils::to_u16string(const std::u32string& str) {
    LOG_TRACE("u32string length = {}", str.length());
    std::wstring_convert<std::codecvt_utf16<char32_t>, char32_t> conv;
    std::string utf8 = conv.to_bytes(str.c_str(), str.c_str() + str.size());
    return to_u16string(utf8); // повторно используем UTF-8 -> UTF-16
}

std::u16string utils::to_u16string(const std::wstring& str) {
    LOG_TRACE_W(L"input wstring length = {}", str.length());
#ifdef _WIN32
    // Windows wchar_t == UTF-16
    LOG_TRACE("Platform: Windows, wchar_t is UTF-16, direct cast");
    return std::u16string(reinterpret_cast<const char16_t*>(str.c_str()));
#else
    // Linux wchar_t == UTF-32
    LOG_TRACE("Platform: Unix, converting UTF-32 wchar_t to UTF-16");
    wstring_convert<codecvt_utf16<wchar_t>, wchar_t> conv;
    string utf16 = conv.to_bytes(str.c_str(), str.c_str() + str.size());
    return std::u16string(reinterpret_cast<const char16_t*>(utf16.c_str()), utf16.size() / sizeof(char16_t));
#endif
}

template <typename CharT>
CharT* utils::duplicate_string(const CharT* src) {
    LOG_TRACE("src={}", src ? "non-null" : "null");
    if (!src) return nullptr;

    size_t length = std::char_traits<CharT>::length(src);

    // Защита от переполнения
    if (length > std::numeric_limits<size_t>::max() / sizeof(CharT)) {
        LOG_TRACE("String too long, potential overflow");
        return nullptr;
    }

    CharT* dest = static_cast<CharT*>(malloc(sizeof(CharT) * (length + 1)));
    if (!dest) {
        LOG_TRACE("malloc failed");
        return nullptr;
    }

    std::char_traits<CharT>::copy(dest, src, length + 1);

    LOG_TRACE("String duplicated, length = {}", length);
    return dest;
}

template char* utils::duplicate_string(const char* src);
template char16_t* utils::duplicate_string(const char16_t* src);
template wchar_t* utils::duplicate_string(const wchar_t* src);
