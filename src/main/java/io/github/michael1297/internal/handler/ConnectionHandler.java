package io.github.michael1297.internal.handler;

import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.pointer.ConnectionPtr;
import io.github.michael1297.internal.pointer.StatementPtr;
import io.github.michael1297.internal.struct.NativeError;

/**
 * Native ODBC connection operations: connect, disconnect, create statement.
 */
public final class ConnectionHandler {
    // Static methods only
    private ConnectionHandler() {
    }

    public static ConnectionPtr connect(String connection_string) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionPtr ptr = NativeDB.INSTANCE.connection(connection_string + '\0', nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return ptr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static ConnectionPtr connect(String connection_string, long timeout) {
        NativeError nativeError = new NativeError();
        ConnectionPtr ptr = NativeDB.INSTANCE.connection_with_timeout(connection_string + '\0', timeout, nativeError);
        if (nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
        return ptr;
    }

    public static ConnectionPtr connect(String dsn, String user, String pass, long timeout) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionPtr ptr = NativeDB.INSTANCE.connection_with_user_pass_timeout(dsn + '\0', user + '\0', pass + '\0', timeout, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return ptr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static StatementPtr create(ConnectionPtr connectionPtr) {
        NativeError nativeError = new NativeError();
        try {
            StatementPtr ptr = NativeDB.INSTANCE.create_statement(connectionPtr, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return ptr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void prepared(StatementPtr statementPtr, String sql, long timeout) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.prepare_statement(statementPtr, sql + '\0', timeout, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void disconnect(ConnectionPtr ptr) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.disconnect(ptr, nativeError);
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static boolean isConnected(ConnectionPtr ptr) {
        NativeError nativeError = new NativeError();
        try {
            boolean result = NativeDB.INSTANCE.is_connected(ptr, nativeError) != 0;
            if (nativeError.error_code != 0) {
                throw new NativeException(nativeError);
            }
            return result;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

}
