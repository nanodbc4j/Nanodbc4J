#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "api/export.h"

extern "C" {

    ODBC_API void prepare_statement(nanodbc::statement* stmt, const char16_t* sql, long timeout, NativeError* error);

    ODBC_API nanodbc::result* execute(nanodbc::statement* stmt, NativeError* error);

    ODBC_API int execute_update(nanodbc::statement* stmt, NativeError* error);

    ODBC_API void close_statement(nanodbc::statement* stmt, NativeError* error);

}