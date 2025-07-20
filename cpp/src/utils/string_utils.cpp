#include "utils/string_utils.h"
#include <type_traits>
#include <memory>
#include <cstring>
#include <codecvt>

using namespace std;

wide_string_t utils::to_wide_string(const char16_t* string) {
    // Преобразуем входящую строку в wide_string (используется UTF-16LE)
    return basic_string<wide_char_t>(reinterpret_cast<const wide_char_t*>(string));
}


u16string utils::to_u16string(const string& str) {
    wstring_convert<std::codecvt_utf8<char16_t>, char16_t> conv;
    return conv.from_bytes(str);
}

u16string utils::to_u16string(const u16string& str) {
    return str;
}

u16string utils::to_u16string(const u32string& str) {
    wstring_convert<codecvt_utf16<char32_t>, char32_t> conv;
    string utf8 = conv.to_bytes(str.c_str(), str.c_str() + str.size());
    return to_u16string(utf8); // повторно используем UTF-8 -> UTF-16
}

u16string utils::to_u16string(const wstring& str) {
#ifdef _WIN32
    // Windows wchar_t == UTF-16
    return reinterpret_cast<const char16_t*>(str.c_str());
#else
    // Linux wchar_t == UTF-32
    wstring_convert<codecvt_utf16<wchar_t>, wchar_t> conv;
    string utf16 = conv.to_bytes(str.c_str(), str.c_str() + str.size());
    return u16string(reinterpret_cast<const char16_t*>(utf16.c_str()), utf16.size() / sizeof(char16_t));
#endif
}

template <typename CharT>
CharT* utils::duplicate_string(const CharT* src) {
    if (!src) return nullptr;

    size_t length = std::char_traits<CharT>::length(src);

    // Защита от переполнения
    if (length > std::numeric_limits<size_t>::max() / sizeof(CharT)) {
        return nullptr;
    }

    CharT* dest = static_cast<CharT*>(malloc(sizeof(CharT) * (length + 1)));
    if (!dest) return nullptr;

    std::char_traits<CharT>::copy(dest, src, length + 1);

    return dest;
}

template char* utils::duplicate_string(const char* src);
template char16_t* utils::duplicate_string(const char16_t* src);
template wchar_t* utils::duplicate_string(const wchar_t* src);