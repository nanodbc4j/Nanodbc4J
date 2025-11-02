package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Library;
import com.sun.jna.Native;
import io.github.nanodbc4j.internal.cstruct.BinaryArray;
import io.github.nanodbc4j.internal.cstruct.DateStruct;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.cstruct.TimeStruct;
import io.github.nanodbc4j.internal.cstruct.TimestampStruct;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import io.github.nanodbc4j.internal.pointer.StatementPtr;

public interface StatementApi extends Library {
    StatementApi INSTANCE = initialize();

    private static StatementApi initialize() {
        try {
            LibraryLoader.load();
            return Native.load(StatementApi.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    void prepare_statement(StatementPtr stmt, String sql, NativeError error);

    void set_int_value(StatementPtr stmt, int index, int value, NativeError error);

    void set_long_value(StatementPtr stmt, int index, long value, NativeError error);

    void set_double_value(StatementPtr stmt, int index, double value, NativeError error);

    void set_bool_value(StatementPtr stmt, int index, byte value, NativeError error);

    void set_float_value(StatementPtr stmt, int index, float value, NativeError error);

    void set_short_value(StatementPtr stmt, int index, short value, NativeError error);

    void set_string_value(StatementPtr stmt, int index, String value, NativeError error);

    void set_date_value(StatementPtr stmt, int index, DateStruct value, NativeError error);

    void set_time_value(StatementPtr stmt, int index, TimeStruct value, NativeError error);

    void set_timestamp_value(StatementPtr stmt, int index, TimestampStruct value, NativeError error);

    void set_binary_array_value(StatementPtr stmt, int index, BinaryArray value, NativeError error);

    ResultSetPtr execute(StatementPtr stmt, int timeout, NativeError error);

    void cancel_statement(StatementPtr stmt, NativeError error);

    void close_statement(StatementPtr stmt, NativeError error);
}
