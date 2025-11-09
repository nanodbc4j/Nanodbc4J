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

/**
 * JNA interface for ODBC statement operations.
 * Maps to native ODBC prepared statement and parameter binding functions.
 */
public interface StatementApi extends Library {
    StatementApi INSTANCE = initialize();

    private static StatementApi initialize() {
        try {
            String lib = LibraryLoader.load();
            return Native.load(lib, StatementApi.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    /**
     * Prepares SQL statement for execution.
     *
     * @param stmt statement pointer
     * @param sql SQL statement to prepare
     * @param error error information output
     */
    void prepare_statement(StatementPtr stmt, String sql, NativeError error);

    /**
     * Sets integer parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value integer value
     * @param error error information output
     */
    void set_int_value(StatementPtr stmt, int index, int value, NativeError error);

    /**
     * Sets long parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value long value
     * @param error error information output
     */
    void set_long_value(StatementPtr stmt, int index, long value, NativeError error);

    /**
     * Sets double parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value double value
     * @param error error information output
     */
    void set_double_value(StatementPtr stmt, int index, double value, NativeError error);

    /**
     * Sets boolean parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value boolean value
     * @param error error information output
     */
    void set_bool_value(StatementPtr stmt, int index, byte value, NativeError error);

    /**
     * Sets float parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value float value
     * @param error error information output
     */
    void set_float_value(StatementPtr stmt, int index, float value, NativeError error);

    /**
     * Sets short parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value short value
     * @param error error information output
     */
    void set_short_value(StatementPtr stmt, int index, short value, NativeError error);

    /**
     * Sets string parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value string value
     * @param error error information output
     */
    void set_string_value(StatementPtr stmt, int index, String value, NativeError error);

    /**
     * Sets date parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value date structure
     * @param error error information output
     */
    void set_date_value(StatementPtr stmt, int index, DateStruct value, NativeError error);

    /**
     * Sets time parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value time structure
     * @param error error information output
     */
    void set_time_value(StatementPtr stmt, int index, TimeStruct value, NativeError error);

    /**
     * Sets timestamp parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value timestamp structure
     * @param error error information output
     */
    void set_timestamp_value(StatementPtr stmt, int index, TimestampStruct value, NativeError error);

    /**
     * Sets binary array parameter value.
     *
     * @param stmt statement pointer
     * @param index parameter index (1-based)
     * @param value binary array structure
     * @param error error information output
     */
    void set_binary_array_value(StatementPtr stmt, int index, BinaryArray value, NativeError error);

    /**
     * Executes prepared statement.
     *
     * @param stmt statement pointer
     * @param timeout execution timeout in seconds
     * @param error error information output
     * @return result set pointer
     */
    ResultSetPtr execute(StatementPtr stmt, int timeout, NativeError error);

    /**
     * Cancels statement execution.
     *
     * @param stmt statement pointer
     * @param error error information output
     */
    void cancel_statement(StatementPtr stmt, NativeError error);

    /**
     * Closes statement and frees resources.
     *
     * @param stmt statement pointer
     * @param error error information output
     */
    void close_statement(StatementPtr stmt, NativeError error);
}
