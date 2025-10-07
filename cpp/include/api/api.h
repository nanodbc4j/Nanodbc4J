#pragma once
#include <string>

#ifdef _WIN32
typedef wchar_t ApiChar;
typedef std::wstring ApiString;
#else
typedef char16_t ApiChar;
typedef std::u16string ApiString;
#endif