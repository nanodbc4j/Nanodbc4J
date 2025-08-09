package io.github.michael1297.internal.handler;

import com.sun.jna.Pointer;
import io.github.michael1297.core.NanodbcResultSetMetaData;
import io.github.michael1297.core.metadata.MetaData;
import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.pointer.MetaDataPtr;
import io.github.michael1297.internal.pointer.ResultSetPtr;
import io.github.michael1297.internal.struct.NativeError;

import java.sql.ResultSetMetaData;

public final class ResultSetHandler {
    // Static methods only
    private ResultSetHandler() {
    }

    public static boolean next(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        try {
            boolean hasNext = NativeDB.INSTANCE.next_result(resultSet, nativeError);
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
            T value = function.apply(resultSet, name, nativeError);
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
            strPtr = NativeDB.INSTANCE.get_string_value_by_index(resultSet, index - 1, nativeError); // В ODBC расчет начинается с 0
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

    public static String getStringValueByName(ResultSetPtr resultSet, String name) {
        NativeError nativeError = new NativeError();
        Pointer strPtr = null;
        try {
            strPtr = NativeDB.INSTANCE.get_string_value_by_name(resultSet, name, nativeError);
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

    public static ResultSetMetaData getResultSetMetaData(ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        MetaDataPtr metaDataPtr = null;
        try {
            metaDataPtr = NativeDB.INSTANCE.get_meta_data(resultSet, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }

            if (metaDataPtr == null || metaDataPtr.getPointer().equals(Pointer.NULL)) {
                return null;
            }

            MetaData metaData = MetaDataHandler.processerMetaData(metaDataPtr);
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

    @FunctionalInterface
    public interface TriFunction<T, U, V, R> {
        R apply(T t, U u, V v);
    }
}
