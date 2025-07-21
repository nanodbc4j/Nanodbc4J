#include "api/result.h"
#include "utils/string_utils.h"

using namespace std;
using namespace utils;

template<typename Result>
static Result get_value_with_error_handling(const function<Result()>& operation, NativeError* error) {
    init_error(error);
    try {
        return operation();
    }
    catch (const nanodbc::index_range_error& e) {
        set_error(error, 1, "IndexError", e.what());
    }
    catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, 2, "TypeError", e.what());
    }
    catch (const std::exception& e) {
        set_error(error, 3, "DatabaseError", e.what());
    }
    catch (...) {
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
    }
    catch (const exception& e) {
        set_error(error, 2, "ResultError", e.what());
    }
    catch (...) {
        set_error(error, -1, "UnknownError", "Unknown close result error");
    }
}
