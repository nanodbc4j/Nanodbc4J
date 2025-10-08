#pragma once
#include <string>

#ifdef _WIN32
typedef wchar_t ApiChar;
typedef std::wstring ApiString;
#else
typedef char16_t ApiChar;
typedef std::u16string ApiString;
#endif

#ifdef _WIN32
#ifdef ODBC_EXPORTS
// #pragma message("ODBC_API will be dllexport")
#define ODBC_API __declspec(dllexport)
#else
// #pragma message("ODBC_API will be dllimport")
#define ODBC_API __declspec(dllimport)
#endif
#else
#define ODBC_API
#endif