#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "api/api.h"
#include "struct/nanodbc_c.h"
#include "struct/binary_array.h"

#ifdef __cplusplus
extern "C" {
#endif

    ODBC_API bool next_result(nanodbc::result* results, NativeError* error);

    ODBC_API int get_int_value_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API long get_long_value_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API double get_double_value_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API bool get_bool_value_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API float get_float_value_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API short get_short_value_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API const ApiChar* get_string_value_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API CDate* get_date_value_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API CTime* get_time_value_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API CTimestamp* get_timestamp_value_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API BinaryArray* get_bytes_array_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API int find_column_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API bool was_null_by_index(nanodbc::result* results, int index, NativeError* error);

    ODBC_API int get_int_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API long get_long_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API double get_double_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API bool get_bool_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API float get_float_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API short get_short_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API const ApiChar* get_string_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API CDate* get_date_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API CTime* get_time_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API CTimestamp* get_timestamp_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API BinaryArray* get_bytes_array_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API int find_column_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API bool was_null_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error);

    ODBC_API void close_result(nanodbc::result* results, NativeError* error);

    ODBC_API void delete_binary_array(BinaryArray* array);

    ODBC_API void delete_date(CDate* date);

    ODBC_API void delete_time(CTime* time);

    ODBC_API void delete_timestamp(CTimestamp* timestamp);

#ifdef __cplusplus
} // extern "C"
#endif