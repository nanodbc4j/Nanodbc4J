#pragma once
#include "core/connection.hpp"
#include "core/result_set.hpp"
#include "struct/error_info.h"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif

    /// \brief Creates a connection using connection string with specified timeout.
    /// \param connection_string The connection string for establishing a connection.
    /// \param timeout Seconds before connection timeout.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to Connection object on success, nullptr on failure.
    ODBC_API Connection* connection_with_timeout(const ApiChar* connection_string, long timeout, NativeError* error) noexcept;

    /// \brief Creates a connection using DSN, username, password and timeout.
    /// \param dsn The name of the data source (DSN).
    /// \param user The username for authenticating to the data source.
    /// \param pass The password for authenticating to the data source.
    /// \param timeout Seconds before connection timeout.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to Connection object on success, nullptr on failure.
    ODBC_API Connection* connection_with_user_pass_timeout(const ApiChar* dsn, const ApiChar* user, const ApiChar* pass, long timeout, NativeError* error) noexcept;

    /// \brief Checks if the connection is currently active and valid.
    /// \param conn Pointer to the Connection object.
    /// \param error Error information structure to populate on failure.
    /// \return true if connected, false otherwise.
    ODBC_API bool is_connected(Connection* conn, NativeError* error) noexcept;

    /// \brief Returns the name of the current catalog.
    /// \param conn Pointer to the Connection object.
    /// \param error Error information structure to populate on failure.
    /// \return Current catalog name as string.
    ODBC_API const ApiChar* get_catalog_name(Connection* conn, NativeError* error) noexcept;

    /// \brief Sets the current catalog name.
    /// \param conn Pointer to the Connection object.
    /// \param catalog The catalog name to set as current.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_catalog_name(Connection* conn, const ApiChar* catalog, NativeError* error) noexcept;

    /// \brief Sets the transaction isolation level.
    /// \param conn Pointer to the Connection object.
    /// \param level The transaction isolation level to set.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_transaction_isolation_level(Connection* conn, int level, NativeError* error) noexcept;

    /// \brief Returns the current transaction isolation level.
    /// \param conn Pointer to the Connection object.
    /// \param error Error information structure to populate on failure.
    /// \return Current transaction isolation level.
    ODBC_API int get_transaction_isolation_level(Connection* conn, NativeError* error) noexcept;

    /// \brief Sets the auto-commit mode for transactions.
    /// \param conn Pointer to the Connection object.
    /// \param autoCommit true to enable auto-commit, false to disable.
    /// \param error Error information structure to populate on failure.
    ODBC_API void set_auto_commit_transaction(Connection* conn, bool autoCommit, NativeError* error) noexcept;

    /// \brief Commits the current transaction.
    /// \param conn Pointer to the Connection object.
    /// \param error Error information structure to populate on failure.
    ODBC_API void commit_transaction(Connection* conn, NativeError* error) noexcept;

    /// \brief Rolls back the current transaction.
    /// \param conn Pointer to the Connection object.
    /// \param error Error information structure to populate on failure.
    ODBC_API void rollback_transaction(Connection* conn, NativeError* error) noexcept;

    /// \brief Returns the current auto-commit mode status.
    /// \param conn Pointer to the Connection object.
    /// \param error Error information structure to populate on failure.
    /// \return true if auto-commit is enabled, false otherwise.
    ODBC_API bool get_auto_commit_transaction(Connection* conn, NativeError* error) noexcept;

    /// \brief Executes a SQL query with specified timeout.
    /// \param conn Pointer to the Connection object.
    /// \param sql The SQL statement to execute.
    /// \param timeout Seconds before query timeout.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result object on success, nullptr on failure.
    ODBC_API ResultSet* execute_request(Connection* conn, const ApiChar* sql, int timeout, NativeError* error) noexcept;

    /// \brief Creates a prepared statement for parameterized queries.
    /// \param conn Pointer to the Connection object.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to Statement object on success, nullptr on failure.
    ODBC_API nanodbc::statement* create_statement(Connection* conn, NativeError* error) noexcept;

    /// \brief Closes the connection and releases associated resources.
    /// \param conn Pointer to the Connection object.
    /// \param error Error information structure to populate on failure.
    ODBC_API void disconnect(Connection* conn, NativeError* error) noexcept;

#ifdef __cplusplus
} // extern "C"
#endif