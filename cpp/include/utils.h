#pragma once
#include <string>

// Тип wide_string зависит от архитектуры и платформы
#ifdef _WIN32
typedef wchar_t wide_char_t;
#else
typedef char16_t wide_char_t;
#endif

typedef std::basic_string<wide_char_t> wide_string_t;

wide_string_t to_wide_string(const char16_t* string);