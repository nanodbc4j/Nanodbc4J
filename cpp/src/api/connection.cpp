#include "api/connection.h"
#include <exception>
#include "utils/string_utils.h"
#include <utils/struct_converter.h>

using namespace std;
using namespace utils;



static nanodbc::connection* connection_with_error_handling(const function<nanodbc::connection* ()>& operation, NativeError* error) {
    init_error(error);
    try {
        return operation();
    } catch (const nanodbc::database_error& e) {
        set_error(error, 1, "DatabaseError", e.what());
    } catch (const exception& e) {
        set_error(error, 1, "DatabaseError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown connection error");
    }
    return nullptr;
}

nanodbc::connection* connection(const char16_t* connection_string, NativeError* error) {
    return connection_with_error_handling(
        [&]() {
            return new nanodbc::connection(to_wide_string(connection_string));
        },
        error
    );
}

nanodbc::connection* connection_with_timeout(const char16_t* connection_string, long timeout, NativeError* error) {
    return connection_with_error_handling(
        [&]() {
            return new nanodbc::connection(to_wide_string(connection_string), timeout);
        },
        error
    );
}

nanodbc::connection* connection_with_user_pass_timeout(const char16_t* dsn, const char16_t* user, const char16_t* pass, long timeout, NativeError* error) {
    return connection_with_error_handling(
        [&]() {
            return new nanodbc::connection(to_wide_string(dsn), to_wide_string(user), to_wide_string(pass), timeout);
        },
        error
    );
}

nanodbc::statement* create_statement(nanodbc::connection* conn, NativeError* error) {
    init_error(error);
    try {
        return new nanodbc::statement(*conn);
    }
    catch (const exception& e) {
        set_error(error, 2, "StatementError", e.what());
    }
    catch (...) {
        set_error(error, -1, "UnknownError", "Unknown create statement error");
    }
    return nullptr;
}

bool is_connected(const nanodbc::connection* conn, NativeError* error) {
    init_error(error);
    try {
        return conn && conn->connected();
    } catch (const exception& e) {
        set_error(error, 3, "ConnectionCheckError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown connection check error");
    }
    return false;
}

void disconnect(nanodbc::connection* connection, NativeError* error) {
    init_error(error);
    try {
        if (connection) {
            connection->disconnect();
            delete connection;
        }
    } catch (const exception& e) {
        set_error(error, 2, "DisconnectError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown disconnect error");
    }
}