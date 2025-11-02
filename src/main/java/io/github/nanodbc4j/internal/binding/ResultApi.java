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

    byte next_result(ResultSetPtr results, NativeError error);

    byte previous_result(ResultSetPtr results, NativeError error);

    byte first_result(ResultSetPtr results, NativeError error);

    byte last_result(ResultSetPtr results, NativeError error);

    byte absolute_result(ResultSetPtr results, int row, NativeError error);

    int get_row_position_result(ResultSetPtr results, NativeError error);

    int affected_rows_result(ResultSetPtr results, NativeError error);

    int get_int_value_by_index(ResultSetPtr results, int index, NativeError error);

    long get_long_value_by_index(ResultSetPtr results, int index, NativeError error);

    double get_double_value_by_index(ResultSetPtr results, int index, NativeError error);

    byte get_bool_value_by_index(ResultSetPtr results, int index, NativeError error);

    float get_float_value_by_index(ResultSetPtr results, int index, NativeError error);

    short get_short_value_by_index(ResultSetPtr results, int index, NativeError error);

    Pointer get_string_value_by_index(ResultSetPtr results, int index, NativeError error);

    DateStruct get_date_value_by_index(ResultSetPtr results, int index, NativeError error);

    TimeStruct get_time_value_by_index(ResultSetPtr results, int index, NativeError error);

    TimestampStruct get_timestamp_value_by_index(ResultSetPtr results, int index, NativeError error);

    BinaryArray get_bytes_array_by_index(ResultSetPtr results, int index, NativeError error);

    BinaryStreamPtr get_binary_stream_by_index(ResultSetPtr results, int index, NativeError error);

    int read_binary_stream(BinaryStreamPtr stream, byte[] buffer, int offset, int length, NativeError error);

    byte was_null_by_index(ResultSetPtr results, int index, NativeError error);

    int get_int_value_by_name(ResultSetPtr results, String name, NativeError error);

    long get_long_value_by_name(ResultSetPtr results, String name, NativeError error);

    double get_double_value_by_name(ResultSetPtr results, String name, NativeError error);

    byte get_bool_value_by_name(ResultSetPtr results, String name, NativeError error);

    float get_float_value_by_name(ResultSetPtr results, String name, NativeError error);

    short get_short_value_by_name(ResultSetPtr results, String name, NativeError error);

    Pointer get_string_value_by_name(ResultSetPtr results, String name, NativeError error);

    DateStruct get_date_value_by_name(ResultSetPtr results, String name, NativeError error);

    TimeStruct get_time_value_by_name(ResultSetPtr results, String name, NativeError error);

    TimestampStruct get_timestamp_value_by_name(ResultSetPtr results, String name, NativeError error);

    BinaryArray get_bytes_array_by_name(ResultSetPtr results, String name, NativeError error);

    BinaryStreamPtr get_binary_stream_by_name(ResultSetPtr results, String name, NativeError error);

    int find_column_by_name(ResultSetPtr results, String name, NativeError error);

    byte was_null_by_name(ResultSetPtr results, String name, NativeError error);

    void delete_binary_array(BinaryArray array);

    void delete_date(DateStruct date);

    void delete_time(TimeStruct time);

    void delete_timestamp(TimestampStruct timestamp);

    void close_result(ResultSetPtr results, NativeError error);

    void close_binary_stream(BinaryStreamPtr stream);
}
