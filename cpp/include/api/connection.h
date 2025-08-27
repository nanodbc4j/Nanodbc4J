#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "api/export.h"

extern "C" { 

    ODBC_API nanodbc::connection* connection(const char16_t* connection_string, NativeError* error);

    ODBC_API nanodbc::connection* connection_with_timeout(const char16_t* connection_string, long timeout, NativeError* error);

    ODBC_API nanodbc::connection* connection_with_user_pass_timeout(const char16_t* dsn, const char16_t* user, const char16_t* pass, long timeout, NativeError* error);

    ODBC_API bool is_connected(nanodbc::connection* conn, NativeError* error);

    ODBC_API nanodbc::result* execute_request(nanodbc::connection* conn, const char16_t* sql, NativeError* error);

    ODBC_API int execute_request_update(nanodbc::connection* conn, const char16_t* sql, NativeError* error);

    ODBC_API nanodbc::statement* create_statement(nanodbc::connection* conn, NativeError* error);

    ODBC_API void disconnect(nanodbc::connection* conn, NativeError* error);

}