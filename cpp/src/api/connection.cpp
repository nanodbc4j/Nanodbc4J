#include "api/connection.h"
#include <exception>
#include "utils/string_utils.hpp"
#include "utils/struct_converter.hpp"
#include "utils/logger.hpp"

using namespace std;
using namespace utils;


static nanodbc::connection* connection_with_error_handling(const function<nanodbc::connection* ()>& operation, NativeError* error) {
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

nanodbc::connection* connection(const char16_t* connection_string, NativeError* error) {
    LOG_DEBUG_W(L"Сonnection_string={}", utils::to_wstring(connection_string));
    return connection_with_error_handling(
        [&]() {
            return new nanodbc::connection(to_wide_string(connection_string));
        },
        error
    );
}

nanodbc::connection* connection_with_timeout(const char16_t* connection_string, long timeout, NativeError* error) {
    LOG_DEBUG_W(L"Сonnection_string={}, timeout={}", utils::to_wstring(connection_string), timeout);
    return connection_with_error_handling(
        [&]() {
            return new nanodbc::connection(to_wide_string(connection_string), timeout);
        },
        error
    );
}

nanodbc::connection* connection_with_user_pass_timeout(const char16_t* dsn, const char16_t* user, const char16_t* pass, long timeout, NativeError* error) {
    LOG_DEBUG_W(L"DSN={}, User={}, Pass=***, Timeout={}",
        utils::to_wstring(dsn),
        utils::to_wstring(user),
        timeout);
    return connection_with_error_handling(
        [&]() {
            return new nanodbc::connection(to_wide_string(dsn), to_wide_string(user), to_wide_string(pass), timeout);
        },
        error
    );
}

nanodbc::statement* create_statement(nanodbc::connection* conn, NativeError* error) {
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

bool is_connected(const nanodbc::connection* conn, NativeError* error) {
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

void disconnect(nanodbc::connection* connection, NativeError* error) {
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
            LOG_DEBUG("Attempted to disconnect null connection");
        }
    } catch (const exception& e) {
        set_error(error, 2, "DisconnectError", e.what());
        LOG_ERROR_W(L"Exception in disconnect: {}", utils::to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown disconnect error");
        LOG_ERROR("Unknown exception in disconnect");
    }
}