#pragma once
#include "core/connection.hpp"
#include "struct/error_info.h"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif

    ODBC_API Connection* connection_with_timeout(const ApiChar* connection_string, long timeout, NativeError* error) noexcept;

    ODBC_API Connection* connection_with_user_pass_timeout(const ApiChar* dsn, const ApiChar* user, const ApiChar* pass, long timeout, NativeError* error) noexcept;

    ODBC_API bool is_connected(Connection* conn, NativeError* error) noexcept;

    ODBC_API const ApiChar* get_catalog_name(Connection* conn, NativeError* error) noexcept;

    ODBC_API void set_catalog_name(Connection* conn, const ApiChar* catalog, NativeError* error) noexcept;

    ODBC_API void set_transaction_isolation_level(Connection* conn, int level, NativeError* error) noexcept;

    ODBC_API int get_transaction_isolation_level(Connection* conn, NativeError* error) noexcept;

    ODBC_API void set_auto_commit_transaction(Connection* conn, bool autoCommit, NativeError* error) noexcept;

    ODBC_API void commit_transaction(Connection* conn, NativeError* error) noexcept;

    ODBC_API void rollback_transaction(Connection* conn, NativeError* error) noexcept;

    ODBC_API bool get_auto_commit_transaction(Connection* conn, NativeError* error) noexcept;

    ODBC_API nanodbc::result* execute_request(Connection* conn, const ApiChar* sql, int timeout, NativeError* error) noexcept;

    ODBC_API nanodbc::statement* create_statement(Connection* conn, NativeError* error) noexcept;

    ODBC_API void disconnect(Connection* conn, NativeError* error) noexcept;

#ifdef __cplusplus
} // extern "C"
#endif