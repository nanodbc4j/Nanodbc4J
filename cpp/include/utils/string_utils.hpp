#pragma once
#include <string>
#include <iostream>

namespace utils {

	std::string to_string(const std::string& str);

	std::string to_string(const std::wstring& str);

	std::string to_string(const std::u16string& str);

	std::string to_string(const std::u32string& str);

	std::wstring to_wstring(const std::u16string& str);
	
	std::wstring to_wstring(const std::string& str);

	std::wstring to_wstring(const std::wstring& str);

	std::u16string to_u16string(const std::string& str);

	std::u16string to_u16string(const std::u32string& str);

	std::u16string to_u16string(const std::wstring& str);

	std::u16string to_u16string(const std::u16string& str);

	template<typename Container>
	auto join_strings(const Container& c, const typename Container::value_type& delimiter) -> typename Container::value_type;

	template <typename CharT>
	CharT* duplicate_string(const CharT* src);

	template <typename CharT>
	CharT* duplicate_string(const CharT* src, size_t length);
}
