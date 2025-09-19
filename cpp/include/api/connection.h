#pragma once
#include "core/connection.hpp"
#include "struct/error_info.h"
#include "api/export.h"

#ifdef __cplusplus
extern "C" {
#endif

    ODBC_API Connection* connection(const char16_t* connection_string, NativeError* error);

    ODBC_API Connection* connection_with_timeout(const char16_t* connection_string, long timeout, NativeError* error);

    ODBC_API Connection* connection_with_user_pass_timeout(const char16_t* dsn, const char16_t* user, const char16_t* pass, long timeout, NativeError* error);

    ODBC_API bool is_connected(Connection* conn, NativeError* error);

    ODBC_API const char16_t* get_catalog_name(Connection* conn, NativeError* error);

    ODBC_API void set_catalog_name(Connection* conn, const char16_t* catalog, NativeError* error);

    ODBC_API void set_transaction_isolation_level(Connection* conn, int level, NativeError* error);

    ODBC_API int get_transaction_isolation_level(Connection* conn, NativeError* error);

    ODBC_API void set_auto_commit_transaction(Connection* conn, bool autoCommit, NativeError* error);

    ODBC_API void commit_transaction(Connection* conn, NativeError* error);

    ODBC_API void rollback_transaction(Connection* conn, NativeError* error);

    ODBC_API bool get_auto_commit_transaction(Connection* conn, NativeError* error);

    ODBC_API nanodbc::result* execute_request(Connection* conn, const char16_t* sql, NativeError* error);

    ODBC_API int execute_request_update(Connection* conn, const char16_t* sql, NativeError* error);

    ODBC_API nanodbc::statement* create_statement(Connection* conn, NativeError* error);

    ODBC_API void disconnect(Connection* conn, NativeError* error);

#ifdef __cplusplus
} // extern "C"
#endif