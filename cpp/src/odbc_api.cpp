#include "odbc_api.h"
#include <exception>
#include "utils/string_utils.h"
#include <utils/struct_converter.h>

using namespace std;
using namespace utils;

template<typename Result>
static Result get_value_with_error_handling(const function<Result()>& operation, NativeError* error) {
    init_error(error);
    try {
        return operation();
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, 1, "IndexError", e.what());
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, 2, "TypeError", e.what());
    } catch (const std::exception& e) {
        set_error(error, 3, "DatabaseError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown error");
    }
    return Result{}; // Возвращаем значение по умолчанию
}

template<typename T>
static T get_value_by_name(nanodbc::result* results, const wide_string_t& column_name, NativeError* error, T fallback = T{}) {
    return get_value_with_error_handling<T>(
        [=]() {
            return results->get<T>(column_name, fallback);
        },
        error
    );
}

template<typename T>
static T get_value_by_index(nanodbc::result* results, int index, NativeError* error, T fallback = T{}) {
    return get_value_with_error_handling<T>(
        [=]() {
            return results->get<T>(index, fallback);
        },
        error
    );
}

static nanodbc::connection* connection_with_error_handling(const function<nanodbc::connection*()>& operation, NativeError* error) {
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

void disconnect(nanodbc::connection* connection,  NativeError* error) {
    init_error(error);
    try {
        if (connection) {
            connection->disconnect();
            delete connection;
        }        
    } catch (const exception& e) {
        set_error(error, 2, "DisconnectError", e.what());
    }
    catch (...) {
        set_error(error, -1, "UnknownError", "Unknown disconnect error");
    }
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

nanodbc::statement* create_statement(nanodbc::connection* conn, NativeError* error) {
    init_error(error);
    try {
        return new nanodbc::statement (*conn);
    } catch (const exception& e) {
        set_error(error, 2, "StatementError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown create statement error");
    }
    return nullptr;
}

void prepare_statement(nanodbc::statement* stmt, const char16_t* sql, long timeout, NativeError* error) {
    init_error(error);
    try {
        nanodbc::prepare(*stmt, to_wide_string(sql), timeout);
    } catch (const exception& e) {
        set_error(error, 2, "StatementError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown create statement error");
    }
}

nanodbc::result* execute(nanodbc::statement* stmt, NativeError* error) {
    init_error(error);
    try {
        auto results = stmt->execute();
        return new nanodbc::result(results);
    } catch (const exception& e) {
        set_error(error, 2, "ExecuteError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown execute statement error");
    }
    return nullptr;
}

bool next_result(nanodbc::result* results, NativeError* error) {
    init_error(error);
    try {
        return results && results->next();
    }
    catch (const exception& e) {
        set_error(error, 2, "ResultError", e.what());
    }
    catch (...) {
        set_error(error, -1, "UnknownError", "Unknown next result error");
    }
    return false;
}

int get_int_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    return get_value_by_index<int>(results, index, error, 0);
}

long get_long_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    return get_value_by_index<long>(results, index, error, 0L);
}

double get_double_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    return get_value_by_index<double>(results, index, error, 0.0);
}

bool get_bool_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    // result->get<bool>() не работает
    return get_value_by_index<short>(results, index, error, 0);
}

float get_float_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    return get_value_by_index<float>(results, index, error, 0.0f);
}

short get_short_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    return get_value_by_index<short>(results, index, error, 0);
}

int get_int_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error) {
    return get_value_by_name<int>(results, to_wide_string(name), error, 0);
}

long get_long_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error) {
    return get_value_by_name<long>(results, to_wide_string(name), error, 0L);
}

double get_double_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error) {
    return get_value_by_name<double>(results, to_wide_string(name), error, 0.0);
}

bool get_bool_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error) {
    // result->get<bool>() не работает
    return get_value_by_name<short>(results, to_wide_string(name), error, 0);
}

float get_float_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error) {
    return get_value_by_name<float>(results, to_wide_string(name), error, 0.0f);
}

short get_short_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error) {
    return get_value_by_name<short>(results, to_wide_string(name), error, 0);
}

void close_result(nanodbc::result* results, NativeError* error) {
    init_error(error);
    try {
        delete results;
    } catch (const exception& e) {
        set_error(error, 2, "ResultError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown close result error");
    }
}

void close_statement(nanodbc::statement* stmt, NativeError* error) {
    init_error(error);
    try {
        stmt->close();
        delete stmt;
    } catch (const exception& e) {
        set_error(error, 2, "StatementError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown close statement error");
    }
}

const Driver** drivers_list(int* count) {
    auto drivers_list = nanodbc::list_drivers();
    *count = static_cast<int>(drivers_list.size());
    return converter::convert(drivers_list);
}

const Datasource** datasources_list(int* count) {
    auto datasources = nanodbc::list_datasources();
    *count = static_cast<int>(datasources.size());
    return converter::convert(datasources);
}
 
void std_free(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}
