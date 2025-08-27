package io.github.michael1297.internal.handler;

import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.pointer.ConnectionPtr;
import io.github.michael1297.internal.pointer.ResultSetPtr;
import io.github.michael1297.internal.pointer.StatementPtr;
import io.github.michael1297.internal.cstruct.NativeError;

public final class StatementHandler {
    // Static methods only
    private StatementHandler() {
    }

    public static ResultSetPtr execute(ConnectionPtr conn, String sql) {
        NativeError nativeError = new NativeError();
        try {
            ResultSetPtr resultSetPtr = NativeDB.INSTANCE.execute_request(conn, sql, nativeError);
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
            int result = NativeDB.INSTANCE.execute_request_update(conn, sql, nativeError);
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
}
