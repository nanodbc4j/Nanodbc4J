package io.github.michael1297.internal.handler;

import com.sun.jna.Pointer;
import io.github.michael1297.internal.cstruct.ResultSetMetaDataStruct;
import io.github.michael1297.internal.dto.ResultSetMetadataDto;
import io.github.michael1297.jdbc.NanodbcResultSetMetaData;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.pointer.ResultSetPtr;
import io.github.michael1297.internal.cstruct.DateStruct;
import io.github.michael1297.internal.cstruct.NativeError;
import io.github.michael1297.internal.cstruct.TimeStruct;
import io.github.michael1297.internal.cstruct.TimestampStruct;
import lombok.experimental.UtilityClass;

import java.io.InvalidClassException;
import java.sql.Date;
import java.sql.ResultSetMetaData;
import java.sql.Time;
import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.time.LocalTime;

import static io.github.michael1297.internal.handler.Handler.*;

@UtilityClass
public final class ResultSetHandler {
    public static boolean next(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            boolean hasNext = NativeDB.INSTANCE.next_result(resultSet, nativeError) != 0;
            throwIfNativeError(nativeError);
            return hasNext;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static <T> T getValueByIndex(ResultSetPtr resultSet, int index, Handler.TriFunction<ResultSetPtr, Integer, NativeError, T> function) {
        NativeError nativeError = new NativeError();
        try {
            T value = function.apply(resultSet, index - 1, nativeError);
            throwIfNativeError(nativeError);
            return value;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static <T> T getValueByName(ResultSetPtr resultSet, String name, Handler.TriFunction<ResultSetPtr, String, NativeError, T> function) {
        NativeError nativeError = new NativeError();
        try {
            T value = function.apply(resultSet, name + '\0', nativeError);
            throwIfNativeError(nativeError);
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
            throwIfNativeError(nativeError);

            return getWideString(strPtr);
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
            throwIfNativeError(nativeError);
            return getWideString(strPtr);
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

    public static boolean wasNull(ResultSetPtr resultSet, Object lastColumn) throws InvalidClassException {
        if (lastColumn == null) {
            return true;
        }
        NativeError nativeError = new NativeError();
        try {
            boolean isNull;
            if (lastColumn instanceof Integer column) {
                isNull = NativeDB.INSTANCE.was_null_by_index(resultSet, column - 1, nativeError) != 0;
            } else if (lastColumn instanceof String column) {
                isNull = NativeDB.INSTANCE.was_null_by_name(resultSet, column + NUL_CHAR, nativeError) != 0;
            } else {
                throw new InvalidClassException("lastColumn is not of type Integer or String");
            }

            throwIfNativeError(nativeError);
            return isNull;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static int findColumn(ResultSetPtr resultSet, String name) {
        NativeError nativeError = new NativeError();
        try {
            int col = NativeDB.INSTANCE.find_column_by_name(resultSet, name + NUL_CHAR, nativeError) + 1;
            throwIfNativeError(nativeError);
            return col;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static ResultSetMetaData getResultSetMetaData(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        ResultSetMetaDataStruct metaDataStruct = null;
        try {
            metaDataStruct = NativeDB.INSTANCE.get_meta_data(resultSet, nativeError);
            throwIfNativeError(nativeError);

            if (metaDataStruct == null) {
                return null;
            }

            ResultSetMetadataDto metaData = ResultSetMetaDataHandler.processerMetaData(metaDataStruct);
            return new NanodbcResultSetMetaData(metaData);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.delete_meta_data(metaDataStruct);
        }
    }

    public static void close(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.close_result(resultSet, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    private static Date convert(DateStruct dateStruct, NativeError nativeError) {
        throwIfNativeError(nativeError);

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
        throwIfNativeError(nativeError);

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
        throwIfNativeError(nativeError);

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

        timestamp.setNanos(timestampStruct.fract);

        return timestamp;
    }
}
