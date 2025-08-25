package io.github.michael1297.internal.handler;

import com.sun.jna.Pointer;
import io.github.michael1297.jdbc.NanodbcResultSetMetaData;
import io.github.michael1297.jdbc.metadata.OdbcResultSetMetadata;
import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.pointer.OdbcResultSetMetaDataPtr;
import io.github.michael1297.internal.pointer.ResultSetPtr;
import io.github.michael1297.internal.cstruct.DateStruct;
import io.github.michael1297.internal.cstruct.NativeError;
import io.github.michael1297.internal.cstruct.TimeStruct;
import io.github.michael1297.internal.cstruct.TimestampStruct;

import java.sql.Date;
import java.sql.ResultSetMetaData;
import java.sql.Time;
import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.time.LocalTime;

public final class ResultSetHandler {

    private static final int MICROSECONDS_TO_NANOSECONDS = 1000;

    // Static methods only
    private ResultSetHandler() {
    }

    public static boolean next(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            boolean hasNext = NativeDB.INSTANCE.next_result(resultSet, nativeError) != 0;
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return hasNext;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static <T> T getValueByIndex(ResultSetPtr resultSet, int index, TriFunction<ResultSetPtr, Integer, NativeError, T> function) {
        NativeError nativeError = new NativeError();
        try {
            T value = function.apply(resultSet, index - 1, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return value;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static <T> T getValueByName(ResultSetPtr resultSet, String name, TriFunction<ResultSetPtr, String, NativeError, T> function) {
        NativeError nativeError = new NativeError();
        try {
            T value = function.apply(resultSet, name + '\0', nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return value;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static String getStringValueByIndex(ResultSetPtr resultSet, int index) {
        NativeError nativeError = new NativeError();
        Pointer strPtr = null;
        try {
            strPtr = NativeDB.INSTANCE.get_string_value_by_index(resultSet, index - 1, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }

            if (strPtr == null || strPtr.equals(Pointer.NULL)) {
                return null;
            }

            return strPtr.getWideString(0);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.std_free(strPtr);
        }
    }

    public static Date getDateValueByIndex(ResultSetPtr resultSet, int index) {
        NativeError nativeError = new NativeError();
        DateStruct dateStruct = null;
        try {
            dateStruct = NativeDB.INSTANCE.get_date_value_by_index(resultSet, index - 1, nativeError);
            return convert(dateStruct, nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.delete_date(dateStruct);
        }
    }

    public static Time getTimeValueByIndex(ResultSetPtr resultSet, int index) {
        NativeError nativeError = new NativeError();
        TimeStruct timeStruct = null;
        try {
            timeStruct = NativeDB.INSTANCE.get_time_value_by_index(resultSet, index - 1, nativeError);
            return convert(timeStruct, nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.delete_time(timeStruct);
        }
    }

    public static Timestamp getTimestampValueByIndex(ResultSetPtr resultSet, int index) {
        NativeError nativeError = new NativeError();
        TimestampStruct timestampStruct = null;
        try {
            timestampStruct = NativeDB.INSTANCE.get_timestamp_value_by_index(resultSet, index - 1, nativeError);
            return convert(timestampStruct, nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.delete_timestamp(timestampStruct);
        }
    }

    public static String getStringValueByName(ResultSetPtr resultSet, String name) {
        NativeError nativeError = new NativeError();
        Pointer strPtr = null;
        try {
            strPtr = NativeDB.INSTANCE.get_string_value_by_name(resultSet, name + '\0', nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }

            if (strPtr == null || strPtr.equals(Pointer.NULL)) {
                return null;
            }

            return strPtr.getWideString(0);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.std_free(strPtr);
        }
    }


    public static Date getDateValueByName(ResultSetPtr resultSet, String name) {
        NativeError nativeError = new NativeError();
        DateStruct dateStruct = null;
        try {
            dateStruct = NativeDB.INSTANCE.get_date_value_by_name(resultSet, name + '\0', nativeError);
            return convert(dateStruct, nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.delete_date(dateStruct);
        }
    }

    public static Time getTimeValueByName(ResultSetPtr resultSet, String name) {
        NativeError nativeError = new NativeError();
        TimeStruct timeStruct = null;
        try {
            timeStruct = NativeDB.INSTANCE.get_time_value_by_name(resultSet, name + '\0', nativeError);
            return convert(timeStruct, nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.delete_time(timeStruct);
        }
    }

    public static Timestamp getTimestampValueByName(ResultSetPtr resultSet, String name) {
        NativeError nativeError = new NativeError();
        TimestampStruct timestampStruct = null;
        try {
            timestampStruct = NativeDB.INSTANCE.get_timestamp_value_by_name(resultSet, name + '\0', nativeError);
            return convert(timestampStruct, nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.delete_timestamp(timestampStruct);
        }
    }

    public static ResultSetMetaData getResultSetMetaData(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        OdbcResultSetMetaDataPtr metaDataPtr = null;
        try {
            metaDataPtr = NativeDB.INSTANCE.get_meta_data(resultSet, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }

            if (metaDataPtr == null || metaDataPtr.getPointer().equals(Pointer.NULL)) {
                return null;
            }

            OdbcResultSetMetadata metaData = OdbcResultSetMetaDataHandler.processerMetaData(metaDataPtr);
            return new NanodbcResultSetMetaData(metaData);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.delete_meta_data(metaDataPtr);
        }
    }

    public static void close(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.close_result(resultSet, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    private static Date convert(DateStruct dateStruct, NativeError nativeError) {
        if (nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }

        if (dateStruct == null || dateStruct.getPointer().equals(Pointer.NULL)) {
            return null;
        }

        var localDate = java.time.LocalDate.of(
                dateStruct.year,
                dateStruct.month,
                dateStruct.day
        );
        return Date.valueOf(localDate);
    }

    private static Time convert(TimeStruct timeStruct, NativeError nativeError) {
        if (nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }

        if (timeStruct == null || timeStruct.getPointer().equals(Pointer.NULL)) {
            return null;
        }

        LocalTime localTime = LocalTime.of(
                timeStruct.hour,
                timeStruct.minute,
                timeStruct.second
        );
        return Time.valueOf(localTime);
    }

    private static Timestamp convert(TimestampStruct timestampStruct, NativeError nativeError) {
        if (nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }

        if (timestampStruct == null || timestampStruct.getPointer().equals(Pointer.NULL)) {
            return null;
        }

        // Собираем LocalDateTime из целых частей
        LocalDateTime localDateTime = LocalDateTime.of(
                timestampStruct.year,
                timestampStruct.month,
                timestampStruct.day,
                timestampStruct.hour,
                timestampStruct.minute,
                timestampStruct.second
        );

        // Создаём Timestamp
        Timestamp timestamp = Timestamp.valueOf(localDateTime);

        // Устанавливаем наносекунды: микросекунды × 1000
        timestamp.setNanos(timestampStruct.fract * MICROSECONDS_TO_NANOSECONDS); // микросекунды → наносекунды

        return timestamp;
    }

    @FunctionalInterface
    public interface TriFunction<T, U, V, R> {
        R apply(T t, U u, V v);
    }
}
