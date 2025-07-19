#include "odbc_api.h"
#include <exception>
#include "string_utils.h"

using namespace std;

template<typename Result>
static Result execute_with_error_handling(const function<Result()>& operation, NativeError* error) {
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
    return execute_with_error_handling<T>(
        [=]() {
            return results->get<T>(column_name, fallback);
        },
        error
    );
}

template<typename T>
static T get_value_by_index(nanodbc::result* results, int index, NativeError* error, T fallback = T{}) {
    return execute_with_error_handling<T>(
        [=]() {
            return results->get<T>(index, fallback);
        },
        error
    );
}

nanodbc::connection* connection(const char16_t* connection_string, long timeout, NativeError* error) {
    init_error(error);
    try {
        return new nanodbc::connection(to_wide_string(connection_string), timeout);
    } catch (const exception& e) {
        set_error(error, 1, "DatabaseError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown connection error");
    }
    return nullptr;
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

const char16_t** drivers_list(int* count) {
    auto drivers_list = nanodbc::list_drivers();
    *count = static_cast<int>(drivers_list.size());

    vector<nanodbc::driver> drivers (drivers_list.begin(), drivers_list.end());

    // Выделяем массив указателей на char16_t
    const char16_t** result = static_cast<const char16_t**>(malloc(sizeof(char16_t*) * drivers.size()));
    if (!result) return nullptr;

    for (size_t i = 0; i < drivers.size(); ++i) {
        u16string utf16_name = to_u16string(drivers[i].name);
        size_t length = utf16_name.size();

        char16_t* strCopy = static_cast<char16_t*>(malloc(sizeof(char16_t) * (length + 1)));

        if (!strCopy) {
            for (size_t j = 0; j < i; ++j)
                free(const_cast<char16_t*>(result[j]));
            free(result);
            return nullptr;
        }

        copy(utf16_name.c_str(), utf16_name.c_str() + length + 1, strCopy);
        result[i] = strCopy;
    }

    return result;
}

const datasource** datasources_list(int* count) {
    auto datasource_list = nanodbc::list_datasources();
    *count = static_cast<int>(datasource_list.size());

    vector<nanodbc::datasource> datasources(datasource_list.begin(), datasource_list.end());

    // Выделяем массив указателей на datasource
    const datasource** result = static_cast<const datasource**>(malloc(sizeof(datasource*) * datasources.size()));
    if (!result) return nullptr;

    for (size_t i = 0; i < datasources.size(); ++i) {
        datasource* item = static_cast<datasource*>(malloc(sizeof(datasource)));

        if (!item) {
            // Освобождаем уже выделенную память в случае ошибки
            for (int j = 0; j < i; ++j) {
                free((void*)result[j]->name);
                free((void*)result[j]->driver);
                free((void*)result[j]);
            }
            free(result);
            return nullptr;
        }

        // Копируем name
        auto u16_name = to_u16string(datasources[i].name);
        item->name = static_cast<char16_t*>(malloc(sizeof(char16_t) * (u16_name.size() + 1)));
        copy(u16_name.c_str(), u16_name.c_str() + u16_name.size() + 1, const_cast<char16_t*>(item->name));

        // Копируем driver
        auto u16_driver = to_u16string(datasources[i].driver);
        item->driver = static_cast<char16_t*>(malloc(sizeof(char16_t) * (u16_driver.size() + 1)));
        copy(u16_driver.c_str(), u16_driver.c_str() + u16_driver.size() + 1, const_cast<char16_t*>(item->driver));

        result[i] = item;
    }

    return result;
}
 
void std_free(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}
