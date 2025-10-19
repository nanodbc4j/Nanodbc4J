#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "struct/nanodbc_c.h"
#include "struct/binary_array.h"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif

    ODBC_API void prepare_statement(nanodbc::statement* stmt, const ApiChar* sql, NativeError* error) noexcept;

    ODBC_API void set_int_value(nanodbc::statement* stmt, int index, int value, NativeError* error) noexcept;

    ODBC_API void set_long_value(nanodbc::statement* stmt, int index, long value, NativeError* error) noexcept;

    ODBC_API void set_double_value(nanodbc::statement* stmt, int index, double value, NativeError* error) noexcept;

    ODBC_API void set_bool_value(nanodbc::statement* stmt, int index, bool value, NativeError* error) noexcept;

    ODBC_API void set_float_value(nanodbc::statement* stmt, int index, float value, NativeError* error) noexcept;

    ODBC_API void set_short_value(nanodbc::statement* stmt, int index, short value, NativeError* error) noexcept;

    ODBC_API void set_string_value(nanodbc::statement* stmt, int index, const ApiChar* value, NativeError* error) noexcept;

    ODBC_API void set_date_value(nanodbc::statement* stmt, int index, CDate* value, NativeError* error) noexcept;

    ODBC_API void set_time_value(nanodbc::statement* stmt, int index, CTime* value, NativeError* error) noexcept;

    ODBC_API void set_timestamp_value(nanodbc::statement* stmt, int index, CTimestamp* value, NativeError* error) noexcept;

    ODBC_API void set_binary_array_value(nanodbc::statement* stmt, int index, BinaryArray* value, NativeError* error) noexcept;

    ODBC_API nanodbc::result* execute(nanodbc::statement* stmt, int timeout, NativeError* error) noexcept;

    ODBC_API void cancel_statement(nanodbc::statement* stmt, NativeError* error) noexcept;

    ODBC_API void close_statement(nanodbc::statement* stmt, NativeError* error) noexcept;

#ifdef __cplusplus
} // extern "C"
#endif