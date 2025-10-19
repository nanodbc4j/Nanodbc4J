#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "api/api.h"
#include "struct/nanodbc_c.h"
#include "struct/binary_array.h"
#include "core/chunked_binary_stream.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    ODBC_API bool next_result(nanodbc::result* results, NativeError* error) noexcept;

    ODBC_API bool previous_result(nanodbc::result* results, NativeError* error) noexcept;

    ODBC_API bool first_result(nanodbc::result* results, NativeError* error) noexcept;

    ODBC_API bool last_result(nanodbc::result* results, NativeError* error) noexcept;

    ODBC_API bool absolute_result(nanodbc::result* results, int row, NativeError* error) noexcept;

    ODBC_API int get_row_position_result(nanodbc::result* results, NativeError* error) noexcept;

    ODBC_API int affected_rows_result(nanodbc::result* results, NativeError* error) noexcept;

    ODBC_API int get_int_value_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API long get_long_value_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API double get_double_value_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API bool get_bool_value_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API float get_float_value_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API short get_short_value_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API const ApiChar* get_string_value_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API CDate* get_date_value_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API CTime* get_time_value_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API CTimestamp* get_timestamp_value_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API BinaryArray* get_bytes_array_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API ChunkedBinaryStream* get_binary_stream_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;

    ODBC_API int read_binary_stream(ChunkedBinaryStream* stream, uint8_t* buffer, int offset, int length, NativeError* error) noexcept;

    ODBC_API bool was_null_by_index(nanodbc::result* results, int index, NativeError* error) noexcept;    

    ODBC_API int get_int_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API long get_long_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API double get_double_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API bool get_bool_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API float get_float_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API short get_short_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API const ApiChar* get_string_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API CDate* get_date_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API CTime* get_time_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API CTimestamp* get_timestamp_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API BinaryArray* get_bytes_array_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API ChunkedBinaryStream* get_binary_stream_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API int find_column_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API bool was_null_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) noexcept;

    ODBC_API void close_result(nanodbc::result* results, NativeError* error) noexcept;

    ODBC_API void delete_binary_array(BinaryArray* array) noexcept;

    ODBC_API void delete_date(CDate* date) noexcept;

    ODBC_API void delete_time(CTime* time) noexcept;

    ODBC_API void delete_timestamp(CTimestamp* timestamp) noexcept;

    ODBC_API void close_binary_stream(ChunkedBinaryStream* stream) noexcept;

#ifdef __cplusplus
} // extern "C"
#endif