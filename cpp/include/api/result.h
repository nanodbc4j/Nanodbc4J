#pragma once
#include "core/result_set.hpp"
#include "struct/error_info.h"
#include "api/api.h"
#include "struct/nanodbc_c.h"
#include "struct/binary_array.h"
#include "core/chunked_binary_stream.hpp"

#ifdef __cplusplus
extern "C" {
#endif

    /// \brief Moves to the next result set in a multiple-result operation.
    /// \param results Pointer to the result set object.
    /// \param error Error information structure to populate on failure.
    /// \return true if another result set exists, false otherwise.
    ODBC_API bool next_result(ResultSet* results, NativeError* error) noexcept;

    /// \brief Moves to the previous result set in a multiple-result operation.
    /// \param results Pointer to the result set object.
    /// \param error Error information structure to populate on failure.
    /// \return true if a previous result set exists, false otherwise.
    ODBC_API bool previous_result(ResultSet* results, NativeError* error) noexcept;

    /// \brief Moves to the first result set in a multiple-result operation.
    /// \param results Pointer to the result set object.
    /// \param error Error information structure to populate on failure.
    /// \return true if operation succeeded, false otherwise.
    ODBC_API bool first_result(ResultSet* results, NativeError* error) noexcept;

    /// \brief Moves to the last result set in a multiple-result operation.
    /// \param results Pointer to the result set object.
    /// \param error Error information structure to populate on failure.
    /// \return true if operation succeeded, false otherwise.
    ODBC_API bool last_result(ResultSet* results, NativeError* error) noexcept;

    /// \brief Moves to a specific result set by absolute position.
    /// \param results Pointer to the result set object.
    /// \param row The absolute position of the result set.
    /// \param error Error information structure to populate on failure.
    /// \return true if operation succeeded, false otherwise.
    ODBC_API bool absolute_result(ResultSet* results, int row, NativeError* error) noexcept;

    /// \brief Returns the current row position in the result set.
    /// \param results Pointer to the result set object.
    /// \param error Error information structure to populate on failure.
    /// \return Current row position, -1 on error.
    ODBC_API int get_row_position_result(ResultSet* results, NativeError* error) noexcept;

    /// \brief Returns the number of rows affected by the query.
    /// \param results Pointer to the result set object.
    /// \param error Error information structure to populate on failure.
    /// \return Number of affected rows, -1 on error.
    ODBC_API int affected_rows_result(ResultSet* results, NativeError* error) noexcept;

    /// \brief Retrieves integer value from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Integer value from specified column.
    ODBC_API int get_int_value_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves long value from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Long value from specified column.
    ODBC_API long get_long_value_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves double value from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Double value from specified column.
    ODBC_API double get_double_value_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves boolean value from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Boolean value from specified column.
    ODBC_API bool get_bool_value_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves float value from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Float value from specified column.
    ODBC_API float get_float_value_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves short value from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Short value from specified column.
    ODBC_API short get_short_value_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves string value from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return String value from specified column.
    ODBC_API const ApiChar* get_string_value_by_index(const ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves date value from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Date value from specified column.
    ODBC_API CDate* get_date_value_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves time value from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Time value from specified column.
    ODBC_API CTime* get_time_value_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves timestamp value from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Timestamp value from specified column.
    ODBC_API CTimestamp* get_timestamp_value_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves binary array from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Binary array from specified column.
    ODBC_API BinaryArray* get_bytes_array_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves binary stream from result set by column index.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return Binary stream from specified column.
    ODBC_API ChunkedBinaryStream* get_binary_stream_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Reads data from binary stream into buffer.
    /// \param stream Pointer to the binary stream object.
    /// \param buffer Destination buffer for read data.
    /// \param offset Offset in buffer to start writing.
    /// \param length Maximum number of bytes to read.
    /// \param error Error information structure to populate on failure.
    /// \return Number of bytes actually read, -1 on error.
    ODBC_API int read_binary_stream(ChunkedBinaryStream* stream, uint8_t* buffer, int offset, int length, NativeError* error) noexcept;

    /// \brief Checks if the value at specified column index was NULL.
    /// \param results Pointer to the result set object.
    /// \param index Zero-based column index.
    /// \param error Error information structure to populate on failure.
    /// \return true if value was NULL, false otherwise.
    ODBC_API bool was_null_by_index(ResultSet* results, int index, NativeError* error) noexcept;

    /// \brief Retrieves integer value from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Integer value from specified column.
    ODBC_API int get_int_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves long value from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Long value from specified column.
    ODBC_API long get_long_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves double value from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Double value from specified column.
    ODBC_API double get_double_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves boolean value from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Boolean value from specified column.
    ODBC_API bool get_bool_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves float value from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Float value from specified column.
    ODBC_API float get_float_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves short value from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Short value from specified column.
    ODBC_API short get_short_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves string value from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return String value from specified column.
    ODBC_API const ApiChar* get_string_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves date value from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Date value from specified column.
    ODBC_API CDate* get_date_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves time value from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Time value from specified column.
    ODBC_API CTime* get_time_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves timestamp value from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Timestamp value from specified column.
    ODBC_API CTimestamp* get_timestamp_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves binary array from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Binary array from specified column.
    ODBC_API BinaryArray* get_bytes_array_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Retrieves binary stream from result set by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return Binary stream from specified column.
    ODBC_API ChunkedBinaryStream* get_binary_stream_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Finds column index by column name.
    /// \param results Pointer to the result set object.
    /// \param name Column name to find.
    /// \param error Error information structure to populate on failure.
    /// \return Zero-based column index, -1 if not found.
    ODBC_API int find_column_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Checks if the value at specified column name was NULL.
    /// \param results Pointer to the result set object.
    /// \param name Column name.
    /// \param error Error information structure to populate on failure.
    /// \return true if value was NULL, false otherwise.
    ODBC_API bool was_null_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept;

    /// \brief Sets an alias name for the specified column in the rowset.
    ///
    /// If the specified column number is out of range, no alias is set and
    /// the original column name will be used in subsequent operations.
    /// \param alias_column_name The alias name to assign to the column.
    /// \param column Column position (0-indexed).
    ODBC_API void set_alias_column_name(ResultSet* results, const ApiChar* alias_column_name, short column, NativeError* error) noexcept;

    /// \brief Maps a column name to its alias for the specified column position.
    ///
    /// If no alias has been set for the column, or if the column number is
    /// out of range, the original column name is returned unchanged.
    /// \param column_name Original column name from the result set.
    /// \param column Column position (0-indexed).
    /// \return The alias name if set, otherwise the original column name.
    ODBC_API const ApiChar* map_column_name(ResultSet* results, const ApiChar* column_name, short column, NativeError* error) noexcept;

    /// \brief Closes and releases result set resources.
    /// \param results Pointer to the result set object.
    /// \param error Error information structure to populate on failure.
    ODBC_API void close_result(ResultSet* results, NativeError* error) noexcept;

    /// \brief Releases binary array resources.
    /// \param array Pointer to BinaryArray object to delete.
    ODBC_API void delete_binary_array(BinaryArray* array) noexcept;

    /// \brief Releases date object resources.
    /// \param date Pointer to CDate object to delete.
    ODBC_API void delete_date(CDate* date) noexcept;

    /// \brief Releases time object resources.
    /// \param time Pointer to CTime object to delete.
    ODBC_API void delete_time(CTime* time) noexcept;

    /// \brief Releases timestamp object resources.
    /// \param timestamp Pointer to CTimestamp object to delete.
    ODBC_API void delete_timestamp(CTimestamp* timestamp) noexcept;

    /// \brief Closes and releases binary stream resources.
    /// \param stream Pointer to ChunkedBinaryStream object to close.
    ODBC_API void close_binary_stream(ChunkedBinaryStream* stream) noexcept;

#ifdef __cplusplus
} // extern "C"
#endif