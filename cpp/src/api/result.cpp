#include "api/result.h"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

using namespace std;
using namespace utils;

template<typename Result>
static Result get_value_with_error_handling(const function<Result()>& operation, NativeError* error) {
    init_error(error);
    try {
        return operation();
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, 1, "IndexError", e.what());
        LOG_DEBUG_W(L"Index range error in get_value: {}", to_wstring(e.what()));
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, 2, "TypeError", e.what());
        LOG_DEBUG_W(L"Type incompatible error in get_value: {}", to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, 3, "DatabaseError", e.what());
        LOG_DEBUG_W(L"Standard exception in get_value: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown error");
        LOG_DEBUG_W(L"Unknown exception in get_value");
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


bool next_result(nanodbc::result* results, NativeError* error) {
    LOG_DEBUG_W(L"Calling next() on result: {}", reinterpret_cast<uintptr_t>(results));
    init_error(error);
    try {
        if (!results) {
            LOG_DEBUG_W(L"Result is null, next() returns false");
            return false;
        }
        bool has_next = results->next();
        LOG_DEBUG_W(L"next() result: {}", has_next ? L"true" : L"false");
        return has_next;
    } catch (const exception& e) {
        set_error(error, 2, "ResultError", e.what());
        LOG_DEBUG_W(L"Exception in next_result: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown next result error");
        LOG_DEBUG_W(L"Unknown exception in next_result");
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
    short value = get_value_by_index<short>(results, index, error, 0);
    return value != 0;
}

float get_float_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    return get_value_by_index<float>(results, index, error, 0.0f);
}

short get_short_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    return get_value_by_index<short>(results, index, error, 0);
}

const char16_t* get_string_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    LOG_DEBUG_W(L"Getting string value by index: {}", index);
    init_error(error);
    try {
        if (!results) {
            LOG_DEBUG_W(L"Result is null");
            set_error(error, 3, "ResultError", "Result is null");
            return nullptr;
        }
        auto value = results->get<nanodbc::string>(index);
        auto u16_value = to_u16string(value);
        LOG_DEBUG_W(L"String value retrieved from index {}: '{}'", index, to_wstring(u16_value));
        return duplicate_string(u16_value.c_str());
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, 1, "IndexError", e.what());
        LOG_DEBUG_W(L"Index range error at index {}: {}", index, to_wstring(e.what()));
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, 2, "TypeError", e.what());
        LOG_DEBUG_W(L"Type incompatible error at index {}: {}", index, to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, 3, "DatabaseError", e.what());
        LOG_DEBUG_W(L"Exception in get_string_value_by_index {}: {}", index, to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown error");
        LOG_DEBUG_W(L"Unknown exception in get_string_value_by_index {}", index);
    }
    return nullptr;    
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

const char16_t* get_string_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error) {
    const auto w_name = to_wstring(name);
    LOG_DEBUG_W(L"Getting string value by name: '{}'", w_name);
    init_error(error);

    try {
        if (!results) {
            LOG_DEBUG_W(L"Result is null");
            set_error(error, 3, "ResultError", "Result is null");
            return nullptr;
        }
        auto value = results->get<nanodbc::string>(to_wide_string(name));
        auto u16_value = to_u16string(value);
        LOG_DEBUG_W(L"String value retrieved by name '{}': '{}'", w_name, to_wstring(u16_value));
        return duplicate_string(u16_value.c_str());
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, 1, "IndexError", e.what());
        LOG_DEBUG_W(L"Index range error for column '{}': {}", w_name, to_wstring(e.what()));
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, 2, "TypeError", e.what());
        LOG_DEBUG_W(L"Type incompatible error for column '{}': {}", w_name, to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, 3, "DatabaseError", e.what());
        LOG_DEBUG_W(L"Exception in get_string_value_by_name '{}': {}", w_name, to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown error");
        LOG_DEBUG_W(L"Unknown exception in get_string_value_by_name '{}'", w_name);
    }
    return nullptr;
}

void close_result(nanodbc::result* results, NativeError* error) {
    LOG_DEBUG_W(L"Closing result: {}", reinterpret_cast<uintptr_t>(results));
    init_error(error);
    try {
        if (!results) {
            LOG_DEBUG_W(L"Attempted to close null result");
            return;
        }
        delete results;
        LOG_DEBUG_W(L"Result successfully closed and deleted");
    } catch (const exception& e) {
        set_error(error, 2, "ResultError", e.what());
        LOG_DEBUG_W(L"Exception in close_result: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown close result error");
        LOG_DEBUG_W(L"Unknown exception in close_result");
    }
}
