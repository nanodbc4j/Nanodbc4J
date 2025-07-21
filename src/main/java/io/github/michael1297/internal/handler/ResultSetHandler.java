package io.github.michael1297.internal.handler;

import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.pointer.ResultSetPtr;
import io.github.michael1297.internal.struct.NativeError;

public final class ResultSetHandler {
    private ResultSetHandler() {
    }

    public static void close (ResultSetPtr resultSet) {
        NativeError nativeError = new NativeError();
        NativeDB.INSTANCE.close_result(resultSet, nativeError);
        if(nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
    }

    public static <T> T getValueByIndex (ResultSetPtr resultSet, int index, TriFunction<ResultSetPtr, Integer, NativeError, T> function) {
        NativeError nativeError = new NativeError();
        T value = function.apply(resultSet, index, nativeError);
        if(nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
        return value;
    }

    public static <T> T getValueByName (ResultSetPtr resultSet, String name, TriFunction<ResultSetPtr, String, NativeError, T> function) {
        NativeError nativeError = new NativeError();
        T value = function.apply(resultSet, name, nativeError);
        if(nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
        return value;
    }


    @FunctionalInterface
    public interface TriFunction<T, U, V, R> {
        R apply(T t, U u, V v);
    }
}
