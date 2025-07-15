#include "odbc_api.h"
#include <exception>
#include "utils.h"

// Тип wide_string зависит от архитектуры и платформы
#ifdef _WIN32
typedef wchar_t wide_char_t;
#else
typedef char16_t wide_char_t;
#endif

using namespace std;


nanodbc::connection* connection(const char16_t* connection_string,  NativeError* error) {
    init_error(error);
    try {
        return new nanodbc::connection(to_wide_string(connection_string).c_str());
    } catch (const std::exception& e) {
        set_error(error, 1, "DatabaseError", e.what());
    }
    catch (...) {
        set_error(error, -1, "UnknownError", "Unknown connection error");
    }
    return nullptr;
}

void disconnect(nanodbc::connection* connection,  NativeError* error) {
    init_error(error);
    try {
        connection->disconnect();
        delete connection;
    } catch (const std::exception& e) {
        set_error(error, 2, "DisconnectError", e.what());
    }
    catch (...) {
        set_error(error, -1, "UnknownError", "Unknown disconnect error");
    }
}

int is_connected(const nanodbc::connection* conn, NativeError* error) {
    init_error(error);
    try {
        return conn && conn->connected();
    }
    catch (const std::exception& e) {
        set_error(error, 3, "ConnectionCheckError", e.what());
    }
    catch (...) {
        set_error(error, -1, "UnknownError", "Unknown connection check error");
    }
    return 0;
}


