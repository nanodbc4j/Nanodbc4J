package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import io.github.nanodbc4j.internal.cstruct.BinaryArray;
import io.github.nanodbc4j.internal.cstruct.DateStruct;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.cstruct.TimeStruct;
import io.github.nanodbc4j.internal.cstruct.TimestampStruct;
import io.github.nanodbc4j.internal.pointer.BinaryStreamPtr;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;

/**
 * JNA interface for ODBC result set operations.
 * Maps to native ODBC result set data retrieval functions.
 */
public interface ResultApi extends Library {
    ResultApi INSTANCE = initialize();

    private static ResultApi initialize() {
        try {
            LibraryLoader.load();
            return Native.load(ResultApi.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    /**
     * Moves to next result set row.
     *
     * @param results result set pointer
     * @param error error information output
     * @return 1 if successful
     */
    byte next_result(ResultSetPtr results, NativeError error);

    /**
     * Moves to previous result set row.
     *
     * @param results result set pointer
     * @param error error information output
     * @return 1 if successful
     */
    byte previous_result(ResultSetPtr results, NativeError error);

    /**
     * Moves to first result set row.
     *
     * @param results result set pointer
     * @param error error information output
     * @return 1 if successful
     */
    byte first_result(ResultSetPtr results, NativeError error);

    /**
     * Moves to last result set row.
     *
     * @param results result set pointer
     * @param error error information output
     * @return 1 if successful
     */
    byte last_result(ResultSetPtr results, NativeError error);

    /**
     * Moves to absolute row position.
     *
     * @param results result set pointer
     * @param row row number (1-based)
     * @param error error information output
     * @return 1 if successful
     */
    byte absolute_result(ResultSetPtr results, int row, NativeError error);

    /**
     * Gets current row position.
     *
     * @param results result set pointer
     * @param error error information output
     * @return current row number
     */
    int get_row_position_result(ResultSetPtr results, NativeError error);

    /**
     * Gets number of affected rows.
     *
     * @param results result set pointer
     * @param error error information output
     * @return affected rows count
     */
    int affected_rows_result(ResultSetPtr results, NativeError error);

    /**
     * Gets integer value by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return integer value
     */
    int get_int_value_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets long value by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return long value
     */
    long get_long_value_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets double value by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return double value
     */
    double get_double_value_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets boolean value by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return boolean value
     */
    byte get_bool_value_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets float value by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return float value
     */
    float get_float_value_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets short value by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return short value
     */
    short get_short_value_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets string value by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return pointer to string value
     */
    Pointer get_string_value_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets date value by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return date structure
     */
    DateStruct get_date_value_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets time value by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return time structure
     */
    TimeStruct get_time_value_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets timestamp value by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return timestamp structure
     */
    TimestampStruct get_timestamp_value_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets binary array by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return binary array structure
     */
    BinaryArray get_bytes_array_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets binary stream by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return binary stream pointer
     */
    BinaryStreamPtr get_binary_stream_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Reads data from binary stream.
     *
     * @param stream binary stream pointer
     * @param buffer destination buffer
     * @param offset buffer offset
     * @param length number of bytes to read
     * @param error error information output
     * @return number of bytes read
     */
    int read_binary_stream(BinaryStreamPtr stream, byte[] buffer, int offset, int length, NativeError error);

    /**
     * Checks if last value was NULL by column index.
     *
     * @param results result set pointer
     * @param index column index (1-based)
     * @param error error information output
     * @return 1 if value was NULL
     */
    byte was_null_by_index(ResultSetPtr results, int index, NativeError error);

    /**
     * Gets integer value by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return integer value
     */
    int get_int_value_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets long value by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return long value
     */
    long get_long_value_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets double value by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return double value
     */
    double get_double_value_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets boolean value by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return boolean value
     */
    byte get_bool_value_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets float value by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return float value
     */
    float get_float_value_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets short value by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return short value
     */
    short get_short_value_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets string value by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return pointer to string value
     */
    Pointer get_string_value_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets date value by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return date structure
     */
    DateStruct get_date_value_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets time value by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return time structure
     */
    TimeStruct get_time_value_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets timestamp value by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return timestamp structure
     */
    TimestampStruct get_timestamp_value_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets binary array by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return binary array structure
     */
    BinaryArray get_bytes_array_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Gets binary stream by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return binary stream pointer
     */
    BinaryStreamPtr get_binary_stream_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Finds column index by name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return column index (1-based)
     */
    int find_column_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Checks if last value was NULL by column name.
     *
     * @param results result set pointer
     * @param name column name
     * @param error error information output
     * @return 1 if value was NULL
     */
    byte was_null_by_name(ResultSetPtr results, String name, NativeError error);

    /**
     * Frees binary array resources.
     *
     * @param array binary array to delete
     */
    void delete_binary_array(BinaryArray array);

    /**
     * Frees date structure resources.
     *
     * @param date date structure to delete
     */
    void delete_date(DateStruct date);

    /**
     * Frees time structure resources.
     *
     * @param time time structure to delete
     */
    void delete_time(TimeStruct time);

    /**
     * Frees timestamp structure resources.
     *
     * @param timestamp timestamp structure to delete
     */
    void delete_timestamp(TimestampStruct timestamp);

    /**
     * Closes result set and frees resources.
     *
     * @param results result set pointer
     * @param error error information output
     */
    void close_result(ResultSetPtr results, NativeError error);

    /**
     * Closes binary stream and frees resources.
     *
     * @param stream binary stream pointer
     */
    void close_binary_stream(BinaryStreamPtr stream);
}
