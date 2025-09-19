package io.github.michael1297.internal.handler;

import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.cstruct.DateStruct;
import io.github.michael1297.internal.cstruct.TimeStruct;
import io.github.michael1297.internal.cstruct.TimestampStruct;
import io.github.michael1297.internal.pointer.ConnectionPtr;
import io.github.michael1297.internal.pointer.ResultSetPtr;
import io.github.michael1297.internal.pointer.StatementPtr;
import io.github.michael1297.internal.cstruct.NativeError;
import lombok.experimental.UtilityClass;

import java.sql.Date;
import java.sql.Time;
import java.sql.Timestamp;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;

@UtilityClass
public final class StatementHandler {

    private static final int NANOS_PER_MICRO = 1000;

    public static ResultSetPtr execute(ConnectionPtr conn, String sql) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr = NativeDB.INSTANCE.execute_request(conn, sql + "\0", nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static int executeUpdate(ConnectionPtr conn, String sql) {
        NativeError nativeError = new NativeError();
        try {
            int result = NativeDB.INSTANCE.execute_request_update(conn, sql + "\0", nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return result;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static ResultSetPtr execute(StatementPtr statementPtr) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr = NativeDB.INSTANCE.execute(statementPtr, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return resultSetPtr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static int executeUpdate(StatementPtr statementPtr) {
        NativeError nativeError = new NativeError();
        try {
            int result = NativeDB.INSTANCE.execute_update(statementPtr, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return result;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static <T> void setValueByIndex(StatementPtr statementPtr, int index, T value, QuadConsumer<StatementPtr, Integer, T, NativeError> function) {
        NativeError nativeError = new NativeError();
        try {
            function.accept(statementPtr, index - 1, value,  nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void close(StatementPtr statement) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.close_statement(statement, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
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
        struct.fract = localDateTime.getNano() / NANOS_PER_MICRO;
        return struct;
    }

    @FunctionalInterface
    public interface QuadConsumer<T, U, V, W> {
        void accept(T t, U u, V v, W w);
    }
}
