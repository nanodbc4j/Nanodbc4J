#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "struct/nanodbc_c.h"
#include "api/export.h"

extern "C" {

    ODBC_API void prepare_statement(nanodbc::statement* stmt, const char16_t* sql, long timeout, NativeError* error);

    ODBC_API void set_int_value(nanodbc::statement* stmt, int index, int value, NativeError* error);

    ODBC_API void set_long_value(nanodbc::statement* stmt, int index, long value, NativeError* error);

    ODBC_API void set_double_value(nanodbc::statement* stmt, int index, double value, NativeError* error);

    ODBC_API void set_bool_value(nanodbc::statement* stmt, int index, bool value, NativeError* error);

    ODBC_API void set_float_value(nanodbc::statement* stmt, int index, float value, NativeError* error);

    ODBC_API void set_short_value(nanodbc::statement* stmt, int index, short value, NativeError* error);

    ODBC_API void set_string_value(nanodbc::statement* stmt, int index, const char16_t* value, NativeError* error);

    ODBC_API void set_date_value(nanodbc::statement* stmt, int index, CDate* value, NativeError* error);

    ODBC_API void set_time_value(nanodbc::statement* stmt, int index, CTime* value, NativeError* error);

    ODBC_API void set_timestamp_value(nanodbc::statement* stmt, int index, CTimestamp* value, NativeError* error);

    ODBC_API nanodbc::result* execute(nanodbc::statement* stmt, NativeError* error);

    ODBC_API int execute_update(nanodbc::statement* stmt, NativeError* error);

    ODBC_API void close_statement(nanodbc::statement* stmt, NativeError* error);

}