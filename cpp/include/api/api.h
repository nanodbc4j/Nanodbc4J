#pragma once
#include <string>

#ifdef _WIN32
#define ODBC_TEXT(s) L##s
typedef wchar_t ApiChar;
typedef std::wstring ApiString;
#else
#define ODBC_TEXT(s) u##s
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
#elif defined(ODBC_EXPORTS) && ((defined(__GNUC__) && __GNUC__ >= 4) || defined(__clang__))
	// #pragma message("ODBC_API will be visibility default")
	#define ODBC_API __attribute__((visibility("default")))
#else
	#define ODBC_API
#endif