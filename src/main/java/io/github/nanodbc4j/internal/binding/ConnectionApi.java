package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.pointer.ConnectionPtr;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import io.github.nanodbc4j.internal.pointer.StatementPtr;

/**
 * JNA interface for ODBC connection operations.
 * Maps to native ODBC API functions.
 */
public interface ConnectionApi extends Library {
    ConnectionApi INSTANCE = initialize();

    private static ConnectionApi initialize() {
        try {
            LibraryLoader.load();
            return Native.load(ConnectionApi.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    /**
     * Creates database connection with timeout.
     *
     * @param connection_string connection string (UTF-16LE encoded)
     * @param timeout connection timeout in seconds
     * @param error error information output
     * @return pointer to connection object
     */
    ConnectionPtr connection_with_timeout(String connection_string, long timeout, NativeError error);

    /**
     * Creates connection with DSN, username and password.
     *
     * @param dsn data source name
     * @param user username
     * @param pass password
     * @param timeout connection timeout in seconds
     * @param error error information output
     * @return pointer to connection object
     */
    ConnectionPtr connection_with_user_pass_timeout(String dsn, String user, String pass, long timeout, NativeError error);

    /**
     * Checks if connection is active.
     *
     * @param conn connection pointer
     * @param error error information output
     * @return 1 if connection is active
     */
    byte is_connected(ConnectionPtr conn, NativeError error);

    /**
     * Gets current catalog name.
     *
     * @param conn connection pointer
     * @param error error information output
     * @return pointer to catalog name string
     */
    Pointer get_catalog_name(ConnectionPtr conn, NativeError error);

    /**
     * Sets catalog name for connection.
     *
     * @param conn connection pointer
     * @param catalog catalog name
     * @param error error information output
     */
    void set_catalog_name(ConnectionPtr conn, String catalog, NativeError error);

    /**
     * Sets transaction isolation level.
     *
     * @param conn connection pointer
     * @param level isolation level
     * @param error error information output
     */
    void set_transaction_isolation_level(ConnectionPtr conn, int level, NativeError error);

    /**
     * Gets current transaction isolation level.
     *
     * @param conn connection pointer
     * @param error error information output
     * @return transaction isolation level
     */
    int get_transaction_isolation_level(ConnectionPtr conn, NativeError error);

    /**
     * Enables/disables auto-commit mode.
     *
     * @param conn connection pointer
     * @param autoCommit auto-commit flag (1-enabled, 0-disabled)
     * @param error error information output
     */
    void set_auto_commit_transaction(ConnectionPtr conn, byte autoCommit, NativeError error);

    /**
     * Commits current transaction.
     *
     * @param conn connection pointer
     * @param error error information output
     */
    void commit_transaction(ConnectionPtr conn, NativeError error);

    /**
     * Rolls back current transaction.
     *
     * @param conn connection pointer
     * @param error error information output
     */
    void rollback_transaction(ConnectionPtr conn, NativeError error);

    /**
     * Gets auto-commit mode status.
     *
     * @param conn connection pointer
     * @param error error information output
     * @return auto-commit status (1-enabled, 0-disabled)
     */
    byte get_auto_commit_transaction(ConnectionPtr conn, NativeError error);

    /**
     * Executes SQL query and returns result set.
     *
     * @param conn connection pointer
     * @param sql SQL query string
     * @param timeout query timeout in seconds
     * @param error error information output
     * @return pointer to result set
     */
    ResultSetPtr execute_request(ConnectionPtr conn, String sql, int timeout, NativeError error);

    /**
     * Creates prepared statement.
     *
     * @param conn connection pointer
     * @param error error information output
     * @return pointer to statement object
     */
    StatementPtr create_statement(ConnectionPtr conn, NativeError error);

    /**
     * Closes database connection.
     *
     * @param connection connection pointer
     * @param error error information output
     */
    void disconnect(ConnectionPtr connection, NativeError error);
}
