#include "api/connection.h"
#include <exception>
#include "utils/string_utils.hpp"
#include "utils/struct_converter.hpp"
#include "utils/logger.hpp"

using namespace std;
using namespace utils;


static Connection* connection_with_error_handling(const function<Connection* ()>& operation, NativeError* error) {
    init_error(error);
    try {
        return operation();
    } catch (const nanodbc::database_error& e) {
        set_error(error, 1, "DatabaseError", e.what());
        LOG_ERROR_W(L"Database error: {}", utils::to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 1, "DatabaseError", e.what());
        LOG_ERROR_W(L"Standard exception: {}", utils::to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown connection error");
        LOG_ERROR("Unknown exception in connection_with_error_handling");
    }
    return nullptr;
}

Connection* connection(const char16_t* connection_string, NativeError* error) {
    LOG_DEBUG_W(L"Сonnection_string={}", utils::to_wstring(connection_string));
    return connection_with_error_handling(
        [&]() {
            return new Connection(to_wide_string(connection_string));
        },
        error
    );
}

Connection* connection_with_timeout(const char16_t* connection_string, long timeout, NativeError* error) {
    LOG_DEBUG_W(L"Сonnection_string={}, timeout={}", utils::to_wstring(connection_string), timeout);
    return connection_with_error_handling(
        [&]() {
            return new Connection(to_wide_string(connection_string), timeout);
        },
        error
    );
}

Connection* connection_with_user_pass_timeout(const char16_t* dsn, const char16_t* user, const char16_t* pass, long timeout, NativeError* error) {
    LOG_DEBUG_W(L"DSN={}, User={}, Pass=***, Timeout={}",
        utils::to_wstring(dsn),
        utils::to_wstring(user),
        timeout);
    return connection_with_error_handling(
        [&]() {
            return new Connection(to_wide_string(dsn), to_wide_string(user), to_wide_string(pass), timeout);
        },
        error
    );
}

nanodbc::statement* create_statement(Connection* conn, NativeError* error) {
    LOG_DEBUG("Creating statement for connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        auto stmt = new nanodbc::statement(*conn);
        LOG_DEBUG("Statement created successfully: {}", reinterpret_cast<uintptr_t>(stmt));
        return stmt;
    } catch (const exception& e) {
        set_error(error, 2, "StatementError", e.what());
        LOG_ERROR_W(L"Exception in create_statement: {}", utils::to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown create statement error");
        LOG_ERROR("Unknown exception in create_statement");
    }
    return nullptr;
}

void set_auto_commit_transaction(Connection* conn, bool autoCommit, NativeError* error) {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot set auto commit transaction");
            set_error(error, 2, "TransactionError", "Connection is null");            
        }
        conn->set_auto_commit(autoCommit);
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during set auto commit: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during set auto commit: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown set auto commit error");
        LOG_ERROR("Unknown exception during set auto commit transaction");
    }
}

void commit_transaction(Connection* conn, NativeError* error) {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot commit transaction");
            set_error(error, 2, "TransactionError", "Connection is null");
        }
        conn->commit();
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during commit transaction: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during commit transaction: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown commit transaction error");
        LOG_ERROR("Unknown exception during commit transaction");
    }
}

void rollback_transaction(Connection* conn, NativeError* error) {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot rollback transaction");
            set_error(error, 2, "TransactionError", "Connection is null");            
        }
        conn->rollback();
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during rollback transaction: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during rollback transaction: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown rollback transaction error");
        LOG_ERROR("Unknown exception during execute");
    }
}

bool get_auto_commit_transaction(Connection* conn, NativeError* error) {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot get auto commit transaction");
            set_error(error, 2, "TransactionError", "Connection is null");
            return true;
        }
        return conn->get_auto_commit();
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during get auto commit transaction: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during get auto commit transaction: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown get auto commit transaction error");
        LOG_ERROR("Unknown exception during get auto commit transaction");
    }
    return true;
}

const char16_t* get_catalog_name(Connection* conn, NativeError* error) {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot get catalog name");
            set_error(error, 2, "ConnectionError", "Connection is null");
            return nullptr;
        }
        auto catalog = conn->catalog_name();
        auto u16_catalog = to_u16string(catalog);
        LOG_DEBUG_W(L"Catalog name: '{}'", to_wstring(catalog));
        return duplicate_string(u16_catalog.c_str(), u16_catalog.length());
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "ConnectionError", e.what());
        LOG_ERROR_W(L"Database error during get catalog name: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "ConnectionError", e.what());
        LOG_ERROR_W(L"Database error during get catalog name: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown get catalog name error");
        LOG_ERROR("Unknown exception during get catalog name");
    }
    return nullptr;
}

void set_catalog_name(Connection* conn, const char16_t* catalog, NativeError* error) {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot set catalog name");
            set_error(error, 2, "ConnectionError", "Connection is null");            
        }
        auto w_catalog = to_wide_string(catalog);
        conn->set_catalog(w_catalog);
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "ConnectionError", e.what());
        LOG_ERROR_W(L"Database error during set catalog name: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "ConnectionError", e.what());
        LOG_ERROR_W(L"Database error during set catalog name: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown set catalog name error");
        LOG_ERROR("Unknown exception during set catalog name");
    }
}

bool is_connected(Connection* conn, NativeError* error) {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        bool connected = conn && conn->connected();
        LOG_DEBUG("Connection status: {}", connected ? "true" : "false");
        return connected;
    } catch (const exception& e) {
        set_error(error, 3, "ConnectionCheckError", e.what());
        LOG_ERROR_W(L"Exception in is_connected: {}", utils::to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown connection check error");
        LOG_ERROR("Unknown exception in is_connected");
    }
    return false;
}

void set_transaction_isolation_level(Connection* conn, int level, NativeError* error) {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot rollback transaction");
            set_error(error, 2, "TransactionError", "Connection is null");
            return;
        }

        conn->set_isolation_level(IsolationLevel::from_odbc(level));
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during set transaction isolation level: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during set transaction isolation level: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown set transaction isolation level error");
        LOG_ERROR("Unknown exception during execute");
    }
}

int get_transaction_isolation_level(Connection* conn, NativeError* error) {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot rollback transaction");
            set_error(error, 2, "TransactionError", "Connection is null");
            return 0;
        }

        return conn->get_isolation_level().to_odbc();
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during get transaction isolation level: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "TransactionError", e.what());
        LOG_ERROR_W(L"Database error during get transaction isolation level: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown get transaction isolation level error");
        LOG_ERROR("Unknown exception during execute");
    }
    return 0;
}

nanodbc::result* execute_request(Connection* conn, const char16_t* sql, NativeError* error) {
    LOG_DEBUG("Executing request: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot execute");
            set_error(error, 2, "ExecuteError", "Connection is null");
            return nullptr;
        }
        auto results = nanodbc::execute(*conn, to_wide_string(sql));
        auto result_ptr = new nanodbc::result(results);
        LOG_DEBUG("Execute succeeded, result: {}", reinterpret_cast<uintptr_t>(result_ptr));
        return result_ptr;
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "ExecuteError", e.what());
        LOG_ERROR_W(L"Database error during execute: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "ExecuteError", e.what());
        LOG_ERROR_W(L"Database error during execute: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown execute connection error");
        LOG_ERROR("Unknown exception during execute");
    }
    return nullptr;
}

int execute_request_update(Connection* conn, const char16_t* sql, NativeError* error) {
    LOG_DEBUG("Executing request: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot execute_update");
            set_error(error, 2, "ExecuteError", "Connection is null");
            return 0;
        }
        auto results = nanodbc::execute(*conn, to_wide_string(sql));
        int affected_rows = static_cast<int>(results.rowset_size());
        LOG_DEBUG("Update executed successfully, affected rows: {}", affected_rows);
        return affected_rows;
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "ExecuteError", e.what());
        LOG_ERROR_W(L"Database error during execute_update: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "ExecuteError", e.what());
        LOG_ERROR_W(L"Database error during execute_update: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown execute_update connection error");
        LOG_ERROR("Unknown exception during execute_update");
    }
    return 0;
}

void disconnect(Connection* connection, NativeError* error) {
    LOG_DEBUG("Disconnecting connection: {}", reinterpret_cast<uintptr_t>(connection));
    init_error(error);
    try {
        if (connection) {
            if (connection->connected()) {
                connection->disconnect();
                LOG_DEBUG("Disconnected successfully");
            } else {
                LOG_DEBUG("Connection was already disconnected");
            }
            delete connection;
            LOG_DEBUG("Connection object deleted");
        } else {
            LOG_ERROR("Attempted to disconnect null connection");
        }
    } catch (const exception& e) {
        set_error(error, 2, "DisconnectError", e.what());
        LOG_ERROR_W(L"Exception in disconnect: {}", utils::to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown disconnect error");
        LOG_ERROR("Unknown exception in disconnect");
    }
}