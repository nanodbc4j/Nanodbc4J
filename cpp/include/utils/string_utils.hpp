#pragma once
#include <string>
#include <iostream>

// Тип wide_string зависит от архитектуры и платформы
#ifdef _WIN32
typedef wchar_t wide_char_t;
#else
typedef char16_t wide_char_t;
#endif

typedef std::basic_string<wide_char_t> wide_string_t;

namespace utils {

	wide_string_t to_wide_string(const char16_t* string);

	std::wstring to_wstring(const char16_t* str);

	std::wstring to_wstring(const char16_t* str, size_t length);

	std::wstring to_wstring(const std::u16string& str);

	std::wstring to_wstring(const char* str);
	
	std::wstring to_wstring(const std::string& str);

	std::wstring to_wstring(std::string_view str);

	std::wstring to_wstring(std::wstring_view str);

	std::u16string to_u16string(const std::string& str);

	std::u16string to_u16string(const std::u16string& str);

	std::u16string to_u16string(const std::u32string& str);

	std::u16string to_u16string(const std::wstring& str);

	template <typename CharT>
	CharT* duplicate_string(const CharT* src);

	template <typename CharT>
	CharT* duplicate_string(const CharT* src, size_t length);
}
