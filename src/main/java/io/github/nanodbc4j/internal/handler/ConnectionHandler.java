package io.github.nanodbc4j.internal.handler;

import com.sun.jna.Pointer;
import io.github.nanodbc4j.internal.binding.ConnectionApi;
import io.github.nanodbc4j.internal.binding.DatabaseMetaDataApi;
import io.github.nanodbc4j.internal.binding.OdbcApi;
import io.github.nanodbc4j.internal.binding.StatementApi;
import io.github.nanodbc4j.internal.cstruct.DatabaseMetaDataStruct;
import io.github.nanodbc4j.internal.dto.DatabaseMetaDataDto;
import io.github.nanodbc4j.internal.pointer.ConnectionPtr;
import io.github.nanodbc4j.internal.pointer.StatementPtr;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.jdbc.NanodbcConnection;
import io.github.nanodbc4j.jdbc.NanodbcDatabaseMetaData;
import lombok.NonNull;
import lombok.experimental.UtilityClass;

import java.sql.DatabaseMetaData;

import static io.github.nanodbc4j.internal.handler.Handler.*;

/**
 * Native ODBC connection operations: connect, disconnect, create statement.
 */
@UtilityClass
public final class ConnectionHandler {

    public static ConnectionPtr connect(@NonNull String connection_string, long timeout) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionPtr ptr =
                    ConnectionApi.INSTANCE.connection_with_timeout(connection_string + NUL_CHAR, timeout, nativeError);
            throwIfNativeError(nativeError);
            return ptr;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static ConnectionPtr connect(@NonNull String dsn, String user, String pass, long timeout) {
        user = user == null ? "" : user;
        pass = pass == null ? "" : pass;
        NativeError nativeError = new NativeError();
        try {
            ConnectionPtr ptr =
                    ConnectionApi.INSTANCE.connection_with_user_pass_timeout(dsn + NUL_CHAR, user + NUL_CHAR, pass + NUL_CHAR, timeout, nativeError);
            throwIfNativeError(nativeError);
            return ptr;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static StatementPtr create(ConnectionPtr connectionPtr) {
        NativeError nativeError = new NativeError();
        try {
            StatementPtr ptr = ConnectionApi.INSTANCE.create_statement(connectionPtr, nativeError);
            throwIfNativeError(nativeError);
            return ptr;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void prepared(StatementPtr statementPtr, @NonNull String sql) {
        NativeError nativeError = new NativeError();
        try {
            StatementApi.INSTANCE.prepare_statement(statementPtr, sql + NUL_CHAR, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void disconnect(ConnectionPtr ptr) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionApi.INSTANCE.disconnect(ptr, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static boolean isConnected(ConnectionPtr ptr) {
        NativeError nativeError = new NativeError();
        try {
            boolean result = ConnectionApi.INSTANCE.is_connected(ptr, nativeError) != 0;
            throwIfNativeError(nativeError);
            return result;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void setCatalog(ConnectionPtr conn, @NonNull String catalog) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionApi.INSTANCE.set_catalog_name(conn, catalog, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static String getCatalog(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        Pointer catalogPtr = Pointer.NULL;
        try {
            catalogPtr = ConnectionApi.INSTANCE.get_catalog_name(conn, nativeError);
            throwIfNativeError(nativeError);
            return getWideString(catalogPtr);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            OdbcApi.INSTANCE.std_free(catalogPtr);
        }
    }

    public static void setTransactionIsolation(ConnectionPtr conn, int level) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionApi.INSTANCE.set_transaction_isolation_level(conn, level, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static int getTransactionIsolation(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            int level = ConnectionApi.INSTANCE.get_transaction_isolation_level(conn, nativeError);
            throwIfNativeError(nativeError);
            return level;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void setAutoCommitTransaction(ConnectionPtr conn, boolean autoCommit) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionApi.INSTANCE.set_auto_commit_transaction(conn, (byte) (autoCommit ? 1 : 0), nativeError);
            throwIfNativeError(nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void commitTransaction(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionApi.INSTANCE.commit_transaction(conn, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void rollbackTransaction(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionApi.INSTANCE.rollback_transaction(conn, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static boolean getAutoCommitTransaction(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            boolean result = ConnectionApi.INSTANCE.get_auto_commit_transaction(conn, nativeError) != 0;
            throwIfNativeError(nativeError);
            return result;
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static DatabaseMetaData getDatabaseSetMetaData(NanodbcConnection connection, ConnectionPtr connectionPtr) {
        NativeError nativeError = new NativeError();
        DatabaseMetaDataStruct metaDataStruct = null;
        try {
            metaDataStruct = DatabaseMetaDataApi.INSTANCE.get_database_meta_data(connectionPtr, nativeError);
            throwIfNativeError(nativeError);

            if (metaDataStruct == null) {
                return null;
            }

            DatabaseMetaDataDto metaData = DatabaseMetaDataHandler.processerMetaData(metaDataStruct);
            return new NanodbcDatabaseMetaData(connection, metaData);
        } finally {
            OdbcApi.INSTANCE.clear_native_error(nativeError);
            DatabaseMetaDataApi.INSTANCE.delete_database_meta_data(metaDataStruct);
        }
    }
}
