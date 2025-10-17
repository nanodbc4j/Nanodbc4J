package io.github.nanodbc4j.internal.handler;

import io.github.nanodbc4j.internal.NativeDB;
import io.github.nanodbc4j.internal.cstruct.DateStruct;
import io.github.nanodbc4j.internal.cstruct.TimeStruct;
import io.github.nanodbc4j.internal.cstruct.TimestampStruct;
import io.github.nanodbc4j.internal.pointer.ConnectionPtr;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import io.github.nanodbc4j.internal.pointer.StatementPtr;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import lombok.NonNull;
import lombok.experimental.UtilityClass;

import java.sql.Date;
import java.sql.Time;
import java.sql.Timestamp;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;

import static io.github.nanodbc4j.internal.handler.Handler.*;

@UtilityClass
public final class StatementHandler {

    public static ResultSetPtr execute(ConnectionPtr conn, @NonNull String sql, int timeout) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr = NativeDB.INSTANCE.execute_request(conn, sql + NUL_CHAR, timeout, nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static int executeUpdate(ConnectionPtr conn, @NonNull String sql, int timeout) {
        NativeError nativeError = new NativeError();
        try {
            int result = NativeDB.INSTANCE.execute_request_update(conn, sql + NUL_CHAR, timeout, nativeError);
            throwIfNativeError(nativeError);
            return result;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static ResultSetPtr execute(StatementPtr statementPtr, int timeout) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr = NativeDB.INSTANCE.execute(statementPtr, timeout, nativeError);
            throwIfNativeError(nativeError);
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static int executeUpdate(StatementPtr statementPtr, int timeout) {
        NativeError nativeError = new NativeError();
        try {
            int result = NativeDB.INSTANCE.execute_update(statementPtr, timeout, nativeError);
            throwIfNativeError(nativeError);
            return result;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static <T> void setValueByIndex(StatementPtr statementPtr, int index, T value, Handler.QuadConsumer<StatementPtr, Integer, T, NativeError> function) {
        NativeError nativeError = new NativeError();
        try {
            function.accept(statementPtr, index - 1, value,  nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void cancel(StatementPtr statement) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.cancel_statement(statement, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void close(StatementPtr statement) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.close_statement(statement, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static DateStruct convert(Date date) {
        if (date == null) return null;
        LocalDate localDate = date.toLocalDate();
        DateStruct struct = new DateStruct();
        struct.year  = (short) localDate.getYear();
        struct.month = (short) localDate.getMonthValue();
        struct.day   = (short) localDate.getDayOfMonth();
        return struct;
    }

    public static TimeStruct convert(Time time) {
        if (time == null) return null;
        LocalTime localTime = time.toLocalTime();
        TimeStruct struct = new TimeStruct();
        struct.hour  = (short) localTime.getHour();
        struct.minute = (short) localTime.getMinute();
        struct.second = (short) localTime.getSecond();
        return struct;
    }

    public static TimestampStruct convert(Timestamp timestamp) {
        if (timestamp == null) return null;
        LocalDateTime localDateTime =  timestamp.toLocalDateTime();
        TimestampStruct struct = new TimestampStruct();
        struct.year  = (short) localDateTime.getYear();
        struct.month = (short) localDateTime.getMonthValue();
        struct.day   = (short) localDateTime.getDayOfMonth();
        struct.hour   = (short) localDateTime.getHour();
        struct.minute = (short) localDateTime.getMinute();
        struct.second = (short) localDateTime.getSecond();
        struct.fract = localDateTime.getNano();
        return struct;
    }
}
