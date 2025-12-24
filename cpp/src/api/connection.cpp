#include "api/connection.h"
#include <exception>
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"
#include "core/string_proxy.hpp"

using namespace std;
using namespace utils;

#define BATCH_OPERATIONS 1

static Connection *
connection_with_error_handling(const function<Connection*()> &operation, NativeError *error) noexcept {
    init_error(error);
    try {
        return operation();
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Connection exception: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown connection error");
        LOG_ERROR("Unknown exception in connection_with_error_handling");
    }
    return nullptr;
}

Connection *connection_with_timeout(const ApiChar *connection_string, long timeout, NativeError *error) noexcept {
    StringProxy str_connection_string(connection_string);

    LOG_DEBUG("Connection_string={}, timeout={}", str_connection_string, timeout);
    return connection_with_error_handling(
        [&] {
            return new Connection(static_cast<nanodbc::string>(str_connection_string), timeout);
        },
        error
    );
}

Connection *connection_with_user_pass_timeout(const ApiChar *dsn, const ApiChar *user, const ApiChar *pass,
                                              long timeout, NativeError *error) noexcept {
    const StringProxy str_dsn(dsn);
    const StringProxy str_user(user);
    const StringProxy str_pass(pass);

    LOG_DEBUG("DSN={}, User={}, Pass=***, Timeout={}", str_dsn, str_user, timeout);
    return connection_with_error_handling(
        [&] {
            return new Connection(
                static_cast<nanodbc::string>(str_dsn),
                static_cast<nanodbc::string>(str_user),
                static_cast<nanodbc::string>(str_pass),
                timeout);
        },
        error
    );
}

nanodbc::statement *create_statement(Connection *conn, NativeError *error) noexcept {
    LOG_DEBUG("Creating statement for connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot set auto commit transaction");
            set_error(error, "Connection is null");
            return nullptr;
        }

        auto stmt = new nanodbc::statement(*conn);
        LOG_DEBUG("Statement created successfully: {}", reinterpret_cast<uintptr_t>(stmt));
        return stmt;
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Exception in create_statement: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown create statement error");
        LOG_ERROR("Unknown exception in create_statement");
    }
    return nullptr;
}

void set_auto_commit_transaction(Connection *conn, bool autoCommit, NativeError *error) noexcept {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot set auto commit transaction");
            set_error(error, "Connection is null");
            return;
        }
        conn->set_auto_commit(autoCommit);
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Database error during set auto commit: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error,"Unknown set auto commit error");
        LOG_ERROR("Unknown exception during set auto commit transaction");
    }
}

void commit_transaction(Connection *conn, NativeError *error) noexcept {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot commit transaction");
            set_error(error, "Connection is null");
            return;
        }
        conn->commit();
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Database error during commit transaction: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown commit transaction error");
        LOG_ERROR("Unknown exception during commit transaction");
    }
}

void rollback_transaction(Connection *conn, NativeError *error) noexcept {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot rollback transaction");
            set_error(error, "Connection is null");
            return;
        }
        conn->rollback();
    } catch (const exception &e) {
        set_error(error,e.what());
        LOG_ERROR("Database error during rollback transaction: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown rollback transaction error");
        LOG_ERROR("Unknown exception during rollback");
    }
}

bool get_auto_commit_transaction(Connection *conn, NativeError *error) noexcept {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot get auto commit transaction");
            set_error(error, "Connection is null");
            return true;
        }
        return conn->get_auto_commit();
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Database error during get auto commit transaction: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown get auto commit transaction error");
        LOG_ERROR("Unknown exception during get auto commit transaction");
    }
    return true;
}

const ApiChar *get_catalog_name(Connection *conn, NativeError *error) noexcept {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot get catalog name");
            set_error(error, "Connection is null");
            return nullptr;
        }
        StringProxy catalog(conn->catalog_name());
        const auto result = static_cast<ApiString>(catalog);
        LOG_DEBUG("Catalog name: '{}'", catalog);
        return duplicate_string(result.c_str(), result.length());
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Database error during get catalog name: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown get catalog name error");
        LOG_ERROR("Unknown exception during get catalog name");
    }
    return nullptr;
}

void set_catalog_name(Connection *conn, const ApiChar *catalog, NativeError *error) noexcept {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot set catalog name");
            set_error(error, "Connection is null");
            return;
        }

        const StringProxy str_catalog(catalog);

        conn->set_catalog(static_cast<const nanodbc::string>(str_catalog));
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Database error during set catalog name: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown set catalog name error");
        LOG_ERROR("Unknown exception during set catalog name");
    }
}

bool is_connected(Connection *conn, NativeError *error) noexcept {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        const bool connected = conn && conn->connected();
        LOG_DEBUG("Connection status: {}", connected);
        return connected;
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Exception in is_connected: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown connection check error");
        LOG_ERROR("Unknown exception in is_connected");
    }
    return false;
}

void set_transaction_isolation_level(Connection *conn, int level, NativeError *error) noexcept {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot rollback transaction");
            set_error(error,  "Connection is null");
            return;
        }

        conn->set_isolation_level(IsolationLevel::from_odbc(level));
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Database error during set transaction isolation level: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown set transaction isolation level error");
        LOG_ERROR("Unknown exception during set transaction isolation level");
    }
}

int get_transaction_isolation_level(Connection *conn, NativeError *error) noexcept {
    LOG_DEBUG("Checking connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot rollback transaction");
            set_error(error, "Connection is null");
            return 0;
        }

        return conn->get_isolation_level().to_odbc();
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Database error during get transaction isolation level: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown get transaction isolation level error");
        LOG_ERROR("Unknown exception during get transaction isolation level");
    }
    return 0;
}

ResultSet *execute_request(Connection *conn, const ApiChar *sql, int timeout, NativeError *error) noexcept {
    LOG_DEBUG("Executing request: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection is null, cannot execute");
            set_error(error, "Connection is null");
            return nullptr;
        }

        const StringProxy str_sql(sql);

        nanodbc::statement stmt(*conn);
        stmt.prepare(static_cast<const nanodbc::string>(str_sql));
        auto result = stmt.execute(BATCH_OPERATIONS, timeout);
        result.unbind();
        auto result_ptr = new ResultSet(std::move(result));
        LOG_DEBUG("Execute succeeded, result: {}", reinterpret_cast<uintptr_t>(result_ptr));
        return result_ptr;
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Database error during execute: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown execute connection error");
        LOG_ERROR("Unknown exception during execute");
    }
    return nullptr;
}

void disconnect(Connection *connection, NativeError *error) noexcept {
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
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Exception in disconnect: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown disconnect error");
        LOG_ERROR("Unknown exception in disconnect");
    }
}
