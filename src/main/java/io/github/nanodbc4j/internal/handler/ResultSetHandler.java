package io.github.nanodbc4j.internal.handler;

import com.sun.jna.Pointer;
import io.github.nanodbc4j.internal.binding.OdbcApi;
import io.github.nanodbc4j.internal.binding.ResultApi;
import io.github.nanodbc4j.internal.binding.ResultSetMetaDataApi;
import io.github.nanodbc4j.internal.cstruct.BinaryArray;
import io.github.nanodbc4j.internal.cstruct.ResultSetMetaDataStruct;
import io.github.nanodbc4j.internal.dto.ResultSetMetadataDto;
import io.github.nanodbc4j.jdbc.NanodbcResultSetMetaData;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import io.github.nanodbc4j.internal.cstruct.DateStruct;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.cstruct.TimeStruct;
import io.github.nanodbc4j.internal.cstruct.TimestampStruct;
import lombok.NonNull;
import lombok.experimental.UtilityClass;

import java.io.InvalidClassException;
import java.sql.Date;
import java.sql.ResultSetMetaData;
import java.sql.Time;
import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.time.LocalTime;

import static io.github.nanodbc4j.internal.handler.Handler.*;

@UtilityClass
public final class ResultSetHandler {
    public static boolean next(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            boolean result = ResultApi.INSTANCE.next_result(resultSet, nativeError) != 0;
            throwIfNativeError(nativeError);
            return result;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static boolean previous(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            boolean result = ResultApi.INSTANCE.previous_result(resultSet, nativeError) != 0;
            throwIfNativeError(nativeError);
            return result;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static boolean first(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            boolean result = ResultApi.INSTANCE.first_result(resultSet, nativeError) != 0;
            throwIfNativeError(nativeError);
            return result;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static boolean last(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            boolean result = ResultApi.INSTANCE.last_result(resultSet, nativeError) != 0;
            throwIfNativeError(nativeError);
            return result;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static boolean absolute(ResultSetPtr resultSet, int row) {
        NativeError nativeError = new NativeError();
        try {
            boolean result = ResultApi.INSTANCE.absolute_result(resultSet, row, nativeError) != 0;
            throwIfNativeError(nativeError);
            return result;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static int getRow(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            int result = ResultApi.INSTANCE.get_row_position_result(resultSet, nativeError);
            throwIfNativeError(nativeError);
            return result;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static int getUpdateCount(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            int result = ResultApi.INSTANCE.affected_rows_result(resultSet, nativeError);
            throwIfNativeError(nativeError);
            return result;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static <T> T getValueByIndex(ResultSetPtr resultSet, int index, Handler.TriFunction<ResultSetPtr, Integer, NativeError, T> function) {
        NativeError nativeError = new NativeError();
        try {
            T value = function.apply(resultSet, index - 1, nativeError);
            throwIfNativeError(nativeError);
            return value;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static <T> T getValueByName(ResultSetPtr resultSet, @NonNull String name, Handler.TriFunction<ResultSetPtr, String, NativeError, T> function) {
        NativeError nativeError = new NativeError();
        try {
            T value = function.apply(resultSet, name + NUL_CHAR, nativeError);
            throwIfNativeError(nativeError);
            return value;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static String getStringValueByIndex(ResultSetPtr resultSet, int index) {
        NativeError nativeError = new NativeError();
        Pointer strPtr = null;
        try {
            strPtr = ResultApi.INSTANCE.get_string_value_by_index(resultSet, index - 1, nativeError);
            throwIfNativeError(nativeError);

            return getWideString(strPtr);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            OdbcApi.INSTANCE.std_free(strPtr);
        }
    }

    public static Date getDateValueByIndex(ResultSetPtr resultSet, int index) {
        NativeError nativeError = new NativeError();
        DateStruct dateStruct = null;
        try {
            dateStruct = ResultApi.INSTANCE.get_date_value_by_index(resultSet, index - 1, nativeError);
            return convert(dateStruct, nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            ResultApi.INSTANCE.delete_date(dateStruct);
        }
    }

    public static Time getTimeValueByIndex(ResultSetPtr resultSet, int index) {
        NativeError nativeError = new NativeError();
        TimeStruct timeStruct = null;
        try {
            timeStruct = ResultApi.INSTANCE.get_time_value_by_index(resultSet, index - 1, nativeError);
            return convert(timeStruct, nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            ResultApi.INSTANCE.delete_time(timeStruct);
        }
    }

    public static Timestamp getTimestampValueByIndex(ResultSetPtr resultSet, int index) {
        NativeError nativeError = new NativeError();
        TimestampStruct timestampStruct = null;
        try {
            timestampStruct = ResultApi.INSTANCE.get_timestamp_value_by_index(resultSet, index - 1, nativeError);
            return convert(timestampStruct, nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            ResultApi.INSTANCE.delete_timestamp(timestampStruct);
        }
    }

    public static byte[] getBytesByIndex(ResultSetPtr resultSet, int index) {
        NativeError nativeError = new NativeError();
        BinaryArray array = null;
        try {
            array = ResultApi.INSTANCE.get_bytes_array_by_index(resultSet, index - 1, nativeError);
            throwIfNativeError(nativeError);
            if (array == null) {
                return new byte[0];
            }
            return array.getBytes();
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            ResultApi.INSTANCE.delete_binary_array(array);
        }
    }

    public static byte[] getBytesByName(ResultSetPtr resultSet, @NonNull String name) {
        NativeError nativeError = new NativeError();
        BinaryArray array = null;
        try {
            array = ResultApi.INSTANCE.get_bytes_array_by_name(resultSet, name + NUL_CHAR, nativeError);
            throwIfNativeError(nativeError);
            if (array == null) {
                return new byte[0];
            }
            return array.getBytes();
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            if (array != null) {
                ResultApi.INSTANCE.delete_binary_array(array);
            }
        }
    }

    public static String getStringValueByName(ResultSetPtr resultSet, @NonNull String name) {
        NativeError nativeError = new NativeError();
        Pointer strPtr = null;
        try {
            strPtr = ResultApi.INSTANCE.get_string_value_by_name(resultSet, name + NUL_CHAR, nativeError);
            throwIfNativeError(nativeError);
            return getWideString(strPtr);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            if (strPtr != null) {
                OdbcApi.INSTANCE.std_free(strPtr);
            }
        }
    }


    public static Date getDateValueByName(ResultSetPtr resultSet, @NonNull String name) {
        NativeError nativeError = new NativeError();
        DateStruct dateStruct = null;
        try {
            dateStruct = ResultApi.INSTANCE.get_date_value_by_name(resultSet, name + NUL_CHAR, nativeError);
            return convert(dateStruct, nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            if (dateStruct != null) {
                ResultApi.INSTANCE.delete_date(dateStruct);
            }
        }
    }

    public static Time getTimeValueByName(ResultSetPtr resultSet, @NonNull String name) {
        NativeError nativeError = new NativeError();
        TimeStruct timeStruct = null;
        try {
            timeStruct = ResultApi.INSTANCE.get_time_value_by_name(resultSet, name + NUL_CHAR, nativeError);
            return convert(timeStruct, nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            if (timeStruct != null) {
                ResultApi.INSTANCE.delete_time(timeStruct);
            }
        }
    }

    public static Timestamp getTimestampValueByName(ResultSetPtr resultSet, @NonNull String name) {
        NativeError nativeError = new NativeError();
        TimestampStruct timestampStruct = null;
        try {
            timestampStruct = ResultApi.INSTANCE.get_timestamp_value_by_name(resultSet, name + NUL_CHAR, nativeError);
            return convert(timestampStruct, nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            if (timestampStruct != null) {
                ResultApi.INSTANCE.delete_timestamp(timestampStruct);
            }
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
                isNull = ResultApi.INSTANCE.was_null_by_index(resultSet, column - 1, nativeError) != 0;
            } else if (lastColumn instanceof String column) {
                isNull = ResultApi.INSTANCE.was_null_by_name(resultSet, column + NUL_CHAR, nativeError) != 0;
            } else {
                throw new InvalidClassException("lastColumn is not of type Integer or String");
            }

            throwIfNativeError(nativeError);
            return isNull;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static int findColumn(ResultSetPtr resultSet, @NonNull String name) {
        NativeError nativeError = new NativeError();
        try {
            int col = ResultApi.INSTANCE.find_column_by_name(resultSet, name + NUL_CHAR, nativeError) + 1;
            throwIfNativeError(nativeError);
            return col;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static ResultSetMetaData getResultSetMetaData(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        ResultSetMetaDataStruct metaDataStruct = null;
        try {
            metaDataStruct = ResultSetMetaDataApi.INSTANCE.get_meta_data(resultSet, nativeError);
            throwIfNativeError(nativeError);

            if (metaDataStruct == null) {
                return null;
            }

            ResultSetMetadataDto metaData = ResultSetMetaDataHandler.processerMetaData(metaDataStruct);
            return new NanodbcResultSetMetaData(metaData);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            ResultSetMetaDataApi.INSTANCE.delete_meta_data(metaDataStruct);
        }
    }

    public static void close(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            ResultApi.INSTANCE.close_result(resultSet, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
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
