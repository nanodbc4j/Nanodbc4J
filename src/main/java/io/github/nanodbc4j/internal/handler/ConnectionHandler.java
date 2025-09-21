package io.github.nanodbc4j.internal.handler;

import com.sun.jna.Pointer;
import io.github.nanodbc4j.internal.NativeDB;
import io.github.nanodbc4j.internal.cstruct.DatabaseMetaDataStruct;
import io.github.nanodbc4j.internal.dto.DatabaseMetaDataDto;
import io.github.nanodbc4j.internal.pointer.ConnectionPtr;
import io.github.nanodbc4j.internal.pointer.StatementPtr;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.jdbc.NanodbcConnection;
import io.github.nanodbc4j.jdbc.NanodbcDatabaseMetaData;
import lombok.experimental.UtilityClass;

import java.sql.DatabaseMetaData;

import static io.github.nanodbc4j.internal.handler.Handler.*;

/**
 * Native ODBC connection operations: connect, disconnect, create statement.
 */
@UtilityClass
public final class ConnectionHandler {

    public static ConnectionPtr connect(String connection_string) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionPtr ptr = NativeDB.INSTANCE.connection(connection_string + NUL_CHAR, nativeError);
            throwIfNativeError(nativeError);
            return ptr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static ConnectionPtr connect(String connection_string, long timeout) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionPtr ptr =
                    NativeDB.INSTANCE.connection_with_timeout(connection_string + NUL_CHAR, timeout, nativeError);
            throwIfNativeError(nativeError);
            return ptr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static ConnectionPtr connect(String dsn, String user, String pass, long timeout) {
        NativeError nativeError = new NativeError();
        try {
            ConnectionPtr ptr =
                    NativeDB.INSTANCE.connection_with_user_pass_timeout(dsn + NUL_CHAR, user + NUL_CHAR, pass + NUL_CHAR, timeout, nativeError);
            throwIfNativeError(nativeError);
            return ptr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static StatementPtr create(ConnectionPtr connectionPtr) {
        NativeError nativeError = new NativeError();
        try {
            StatementPtr ptr = NativeDB.INSTANCE.create_statement(connectionPtr, nativeError);
            throwIfNativeError(nativeError);
            return ptr;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void prepared(StatementPtr statementPtr, String sql, long timeout) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.prepare_statement(statementPtr, sql + NUL_CHAR, timeout, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void disconnect(ConnectionPtr ptr) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.disconnect(ptr, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static boolean isConnected(ConnectionPtr ptr) {
        NativeError nativeError = new NativeError();
        try {
            boolean result = NativeDB.INSTANCE.is_connected(ptr, nativeError) != 0;
            throwIfNativeError(nativeError);
            return result;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void setCatalog(ConnectionPtr conn, String catalog) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.set_catalog_name(conn, catalog, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static String getCatalog(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        Pointer catalogPtr = Pointer.NULL;
        try {
            catalogPtr = NativeDB.INSTANCE.get_catalog_name(conn, nativeError);
            throwIfNativeError(nativeError);
            return getWideString(catalogPtr);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.std_free(catalogPtr);
        }
    }

    public static void setTransactionIsolation(ConnectionPtr conn, int level) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.set_transaction_isolation_level(conn, level, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static int getTransactionIsolation(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            int level = NativeDB.INSTANCE.get_transaction_isolation_level(conn, nativeError);
            throwIfNativeError(nativeError);
            return level;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void setAutoCommitTransaction(ConnectionPtr conn, boolean autoCommit) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.set_auto_commit_transaction(conn, (byte) (autoCommit ? 1 : 0), nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void commitTransaction(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.commit_transaction(conn, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static void rollbackTransaction(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            NativeDB.INSTANCE.rollback_transaction(conn, nativeError);
            throwIfNativeError(nativeError);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static boolean getAutoCommitTransaction(ConnectionPtr conn) {
        NativeError nativeError = new NativeError();
        try {
            boolean result = NativeDB.INSTANCE.get_auto_commit_transaction(conn, nativeError) != 0;
            throwIfNativeError(nativeError);
            return result;
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
        }
    }

    public static DatabaseMetaData getDatabaseSetMetaData(NanodbcConnection connection, ConnectionPtr connectionPtr) {
        NativeError nativeError = new NativeError();
        DatabaseMetaDataStruct metaDataStruct = null;
        try {
            metaDataStruct = NativeDB.INSTANCE.get_database_meta_data(connectionPtr, nativeError);
            throwIfNativeError(nativeError);

            if (metaDataStruct == null) {
                return null;
            }

            DatabaseMetaDataDto metaData = DatabaseMetaDataHandler.processerMetaData(metaDataStruct);
            return new NanodbcDatabaseMetaData(connection, metaData);
        } finally {
            NativeDB.INSTANCE.clear_native_error(nativeError);
            NativeDB.INSTANCE.delete_database_meta_data(metaDataStruct);
        }
    }
}
