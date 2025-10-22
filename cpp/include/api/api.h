#pragma once
#include <nanodbc/nanodbc.h>

typedef nanodbc::string::value_type ApiChar;
typedef nanodbc::string ApiString;


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