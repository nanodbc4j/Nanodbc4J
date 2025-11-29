#include "api/result.h"
#include <functional>
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"
#include "core/string_proxy.hpp"

#ifdef _WIN32
// needs to be included above sql.h for windows
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <sqlext.h>

using namespace std;
using namespace utils;

template<typename T>
static T get_value_by_index(ResultSet* results, int index, NativeError* error, T fallback = T{}) noexcept {
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Result is null");
            set_error(error, ErrorCode::Database, "ResultError", "Result is null");
            return fallback;
        }

        auto value = results->get<T>(index, fallback);

        // for unbound columns, null indicator is determined by SQLGetData call
        if (results->is_null(static_cast<short>(index))) {
            LOG_DEBUG("Column is NULL, returning fallback");
            return fallback;
        }
        return value;
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error in get_value: {}", StringProxy(e.what()));
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error in get_value: {}", StringProxy(e.what()));
    } catch (const exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Standard exception in get_value: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in get_value");
    }
    return fallback;
}

template<typename T>
static T get_value_by_name(ResultSet* results, const StringProxy<ApiChar>& column_name, NativeError* error, T fallback = T{}) noexcept {
    int index = find_column_by_name(results, column_name.c_str(), error);
    if (error && error->error_code) {
        return fallback;
    }
    return get_value_by_index(results, index, error, fallback);
}

template<typename T>
static T execute_result_set_query(ResultSet* results, const function<T(ResultSet*)>& func, NativeError* error) noexcept {
    LOG_DEBUG("Executing result set query: {}", reinterpret_cast<uintptr_t>(results));
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Result is null, returns false");
            return false;
        }
        T result = func(results);
        LOG_DEBUG("Result: {}", result);
        return result;
    } catch (const exception& e) {
        set_error(error, ErrorCode::Standard, "ResultError", e.what());
        LOG_ERROR("Exception: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in next_result");
    }
    return false;
}

bool next_result(ResultSet* results, NativeError* error) noexcept {
    LOG_DEBUG("Calling next() on result: {}", reinterpret_cast<uintptr_t>(results));
    return execute_result_set_query<bool>(results, [](ResultSet* results) {
        return results->next();
    },
    error);
}

bool previous_result(ResultSet* results, NativeError* error) noexcept {
    LOG_DEBUG("Calling previous_result() on result: {}", reinterpret_cast<uintptr_t>(results));
    return execute_result_set_query<bool>(results, [](ResultSet* results) {
        return results->prior();
    },
    error);
}

bool first_result(ResultSet* results, NativeError* error) noexcept {
    LOG_DEBUG("Calling first_result() on result: {}", reinterpret_cast<uintptr_t>(results));
    return execute_result_set_query<bool>(results, [](ResultSet* results) {
        return results->first();
    },
    error);
}

bool last_result(ResultSet* results, NativeError* error) noexcept {
    LOG_DEBUG("Calling last_result() on result: {}", reinterpret_cast<uintptr_t>(results));
    return execute_result_set_query<bool>(results, [](ResultSet* results) {
        return results->last();
    },
    error);
}

bool absolute_result(ResultSet* results, int row, NativeError* error) noexcept {
    LOG_DEBUG("Calling absolute_result() on result: {}", reinterpret_cast<uintptr_t>(results));
    return execute_result_set_query<bool>(results, [row](ResultSet* results) {
        return results->move(row);
    },
    error);
}

int get_row_position_result(ResultSet* results, NativeError* error) noexcept {
    LOG_DEBUG("Calling get_row_result() on result: {}", reinterpret_cast<uintptr_t>(results));
    return execute_result_set_query<int>(results, [](const ResultSet* results) {
        return results->position();
    },
    error);
}

int affected_rows_result(ResultSet* results, NativeError* error) noexcept {
    LOG_DEBUG("Calling get_row_position_result() on result: {}", reinterpret_cast<uintptr_t>(results));
    return execute_result_set_query<int>(results, [](const ResultSet* results) {
        return results->affected_rows();
    },
    error);
}

int get_int_value_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    return get_value_by_index<int>(results, index, error, 0);
}

long get_long_value_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    return get_value_by_index<long>(results, index, error, 0L);
}

double get_double_value_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    return get_value_by_index<double>(results, index, error, 0.0);
}

bool get_bool_value_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    // result->get<bool>() does not work
    return get_value_by_index<BOOL>(results, index, error, 0);
}

float get_float_value_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    return get_value_by_index<float>(results, index, error, 0.0f);
}

short get_short_value_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    return get_value_by_index<short>(results, index, error, 0);
}

const ApiChar* get_string_value_by_index(const ResultSet* results, int index, NativeError* error) noexcept {
    LOG_DEBUG("Getting string value by index: {}", index);
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Result is null");
            set_error(error, ErrorCode::Database, "ResultError", "Result is null");
            return nullptr;
        }

        const StringProxy result (results->get<nanodbc::string>(static_cast<short>(index), {}));

        // for unbound columns, null indicator is determined by SQLGetData call
        if (results->is_null(static_cast<short>(index))) {
            LOG_DEBUG("Column '{}' is NULL", index);
            return nullptr;
        }
        LOG_DEBUG("String value retrieved from index {}: '{}'", index, result);
        const auto str_result = static_cast<ApiString> (result);
        return duplicate_string(str_result.c_str(), str_result.length());
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error at index {}: {}", index, StringProxy(e.what()));
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error at index {}: {}", index, StringProxy(e.what()));
    } catch (const exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception in get_string_value_by_index {}: {}", index, StringProxy(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in get_string_value_by_index {}", index);
    }
    return nullptr;
}

CDate* get_date_value_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    const auto date = get_value_by_index<nanodbc::date>(results, index, error);

    if (was_null_by_index(results, index, error) || error && error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", index);
        return nullptr;
    }

    if (date.year <= 0 || date.month <= 0 || date.month > 12 || date.day <= 0 || date.day > 31) {
        LOG_DEBUG("Treating zero date as NULL for column {}", index);
        return nullptr;
    }

    return new CDate(date);
}

CTime* get_time_value_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    const auto time = get_value_by_index<nanodbc::time>(results, index, error);

    if (was_null_by_index(results, index, error) || error && error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", index);
        return nullptr;
    }

    if (time.hour < 0 || time.hour > 23 || time.min < 0 || time.min > 59 || time.sec < 0 || time.sec > 60) {
        LOG_DEBUG("Invalid time treated as NULL for column {}", index);
        return nullptr;
    }

    return new CTime(time);
}

CTimestamp* get_timestamp_value_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    const auto ts = get_value_by_index<nanodbc::timestamp>(results, index, error);

    if (was_null_by_index(results, index, error) || error && error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", index);
        return nullptr;
    }

    if (ts.year <= 0 || ts.month < 1 || ts.month > 12 || ts.day < 1 || ts.day > 31 || ts.hour < 0 || ts.hour > 23 || ts.min < 0 || ts.min > 59 || ts.sec < 0 || ts.sec > 60) {
        LOG_DEBUG("Invalid timestamp treated as NULL for column {}", index);
        return nullptr;
    }

    return new CTimestamp(ts);
}

ChunkedBinaryStream* get_binary_stream_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    LOG_DEBUG("Getting binary stream by index: {}", index);
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Result is null");
            set_error(error, ErrorCode::Database, "ResultError", "Result is null");
            return nullptr;
        }

        if (results->is_null(static_cast<short>(index))) {
            LOG_DEBUG("Column '{}' is NULL", index);
            return nullptr;
        }

        auto result = new ChunkedBinaryStream(results, index);
        LOG_DEBUG("Binary stream ptr retrieved from index {}: '{}'", index, reinterpret_cast<uintptr_t>(results));
        return result;
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error at index {}: {}", index, StringProxy(e.what()));
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error at index {}: {}", index, StringProxy(e.what()));
    } catch (const exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception in get_binary_stream_by_index {}: {}", index, StringProxy(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in get_binary_stream_by_index {}", index);
    }
    return nullptr;
}

int read_binary_stream(ChunkedBinaryStream* stream, uint8_t* buffer, int offset, int length, NativeError* error) noexcept {
    LOG_DEBUG("Reading binary stream: {}", reinterpret_cast<uintptr_t>(stream));
    init_error(error);
    try {
        if (!stream) {
            set_error(error, ErrorCode::Standard, "StreamError", "ChunkedBinaryStream is null");
            return -1;
        }
        if (buffer == nullptr) {
            set_error(error, ErrorCode::Standard, "StreamError", "buffer is null");
            return -1;
        }
        if (offset < 0 || length < 0) {
            set_error(error, ErrorCode::Standard, "StreamError", "Invalid offset or length");
            return -1;
        }
        int result = stream->read(buffer, offset, length);
        LOG_DEBUG("Reading {} byte", result);
        return result;
    } catch (const exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception in read_binary_stream: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception");
    }
    return -1;
}

BinaryArray* get_bytes_array_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    try {
        try {
            // Try to get as binary data
            const auto binary_data = results->get<vector<uint8_t>>(static_cast<short>(index));
            return results->is_null(static_cast<short>(index)) ? nullptr : new BinaryArray(binary_data);
        } catch (const nanodbc::type_incompatible_error&) {
            // If not binary, try as string (for CLOB)
            auto string_data = results->get<std::string>(static_cast<short>(index));
            const vector<uint8_t> binary_data(string_data.begin(), string_data.end());
            return results->is_null(static_cast<short>(index)) ? nullptr : new BinaryArray(binary_data);
        }
    } catch (const nanodbc::type_incompatible_error& incompatible_error) {
        set_error(error, ErrorCode::Database, "TypeError", incompatible_error.what());
        LOG_ERROR("Type incompatible error at index {}: {}", index, incompatible_error.what());
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error at index {}: {}", index, StringProxy(e.what()));
    } catch (const exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception in get_bytes_array_by_index {}: {}", index, StringProxy(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in get_bytes_array_by_index {}", index);
    } 
    return nullptr;
}

bool was_null_by_index(ResultSet* results, int index, NativeError* error) noexcept {
    LOG_DEBUG("Closing result: {}", reinterpret_cast<uintptr_t>(results));
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Attempted to close null result");
            return true;
        }
        bool is_null = results->is_null(static_cast<short>(index));
        LOG_DEBUG("Index was null '{}': '{}'", index, is_null);
        return is_null;
    }  catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error for column '{}': {}", index, StringProxy(e.what()));
    } catch (const exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception '{}': {}", index, StringProxy(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception '{}'", index);
    } 
    return true;
}

int get_int_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    const StringProxy str_name (name);
    return get_value_by_name<int>(results, str_name, error, 0);
}

long get_long_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    const StringProxy str_name (name);
    return get_value_by_name<long>(results, str_name, error, 0L);
}

double get_double_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    const StringProxy str_name (name);
    return get_value_by_name<double>(results, str_name, error, 0.0);
}

bool get_bool_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    const StringProxy str_name (name);
    return get_value_by_name<BOOL>(results, str_name, error, 0);
}

float get_float_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    const StringProxy str_name (name);
    return get_value_by_name<float>(results, str_name, error, 0.0f);
}

short get_short_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    const StringProxy str_name (name);
    return get_value_by_name<short>(results, str_name, error, 0);
}

const ApiChar* get_string_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    const int index = find_column_by_name(results, name, error);
    if (error->error_code) {
        return nullptr;
    }

    LOG_DEBUG("Getting string value by name: '{}'", to_string(name));
    
    return get_string_value_by_index(results, index, error);
}

CDate* get_date_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    const StringProxy str_name (name);
    auto date = get_value_by_name<nanodbc::date>(results, str_name, error, {});
    if (error && error->error_code) {
        return nullptr;
    }

    if (was_null_by_name(results, name, error) || error && error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", str_name);
        return nullptr;
    }

    if (date.year <= 0 || date.month <= 0 || date.month > 12 || date.day <= 0 || date.day > 31) {
        LOG_DEBUG("Treating zero date as NULL for column '{}'", str_name);
        return nullptr;
    }

    return new CDate(date);
}

CTime* get_time_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    const StringProxy str_name (name);
    const auto time = get_value_by_name<nanodbc::time>(results, str_name, error, {});

    if (error && error->error_code) {
        return nullptr;
    }

    if (was_null_by_name(results, name, error) || error && error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", str_name);
        return nullptr;
    }

    if (time.hour < 0 || time.hour > 23 || time.min < 0 || time.min > 59 || time.sec < 0 || time.sec > 60) {
        LOG_DEBUG("Invalid time treated as NULL for column '{}'", str_name);
        return nullptr;
    }

    return new CTime(time);
}

CTimestamp* get_timestamp_value_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    const StringProxy str_name (name);
    const auto ts = get_value_by_name<nanodbc::timestamp>(results, str_name, error, {});
    if (error && error->error_code) {
        return nullptr;
    }

    if (was_null_by_name(results, name, error) || error && error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", str_name);
        return nullptr;
    }

    if (ts.year <= 0 || ts.month < 1 || ts.month > 12 || ts.day < 1 || ts.day > 31 || ts.hour < 0 || ts.hour > 23 || ts.min < 0 || ts.min > 59 || ts.sec < 0 || ts.sec > 60) {
        LOG_DEBUG("Invalid timestamp treated as NULL for column '{}'", str_name);
        return nullptr;
    }

    return new CTimestamp(ts);
}

BinaryArray* get_bytes_array_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    int index = find_column_by_name(results, name, error);
    if (error && error->error_code) {
        return nullptr;
    }
    return get_bytes_array_by_index(results, index, error);
}

ChunkedBinaryStream* get_binary_stream_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    int index = find_column_by_name(results, name, error);
    if (error && error->error_code) {
        return nullptr;
    }
    return get_binary_stream_by_index(results, index, error);
}

int find_column_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    LOG_DEBUG("Closing result: {}", reinterpret_cast<uintptr_t>(results));
    const StringProxy str_name (name);
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Attempted to close null result");
            return 0;
        }
        int index = results->column(static_cast<nanodbc::string>(str_name));
        LOG_DEBUG("Index retrieved by name '{}': '{}'", str_name, index);
        return index;
    }  catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error for column '{}': {}", str_name, StringProxy(e.what()));
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception '{}': {}", str_name, StringProxy(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception '{}'", str_name);
    } 
    return 0;
}

bool was_null_by_name(ResultSet* results, const ApiChar* name, NativeError* error) noexcept {
    LOG_DEBUG("Closing result: {}", reinterpret_cast<uintptr_t>(results));
    const StringProxy str_name (name);
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Attempted to close null result");
            return true;
        }
        bool is_null = results->is_null(static_cast<nanodbc::string>(str_name));
        LOG_DEBUG("Index was null '{}': '{}'", str_name, is_null);
        return is_null;
    }  catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error for column '{}': {}", str_name, StringProxy(e.what()));
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception '{}': {}", str_name, StringProxy(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception '{}'", str_name);
    } 
    return true;
}

void close_result(ResultSet* results, NativeError* error) noexcept {
    LOG_DEBUG("Closing result: {}", reinterpret_cast<uintptr_t>(results));
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Attempted to close null result");
            return;
        }
        delete results;
        LOG_DEBUG("Result successfully closed and deleted");
    } catch (const exception& e) {
        set_error(error, ErrorCode::Standard, "ResultError", e.what());
        LOG_ERROR("Exception in close_result: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown close result error");
        LOG_ERROR("Unknown exception in close_result");
    }
}

void delete_binary_array(BinaryArray* array) noexcept {
    LOG_DEBUG("Deleting BinaryArray object: {}", reinterpret_cast<uintptr_t>(array));
    if (array) {
        delete array;
        LOG_DEBUG("BinaryArray deleted");
    }
}

void delete_date(CDate* date) noexcept {
    LOG_DEBUG("Deleting CDate object: {}", reinterpret_cast<uintptr_t>(date));
    if (date) {
        delete date;
        LOG_DEBUG("CDate deleted");
    }
}

void delete_time(CTime* time) noexcept {
    LOG_DEBUG("Deleting CTime object: {}", reinterpret_cast<uintptr_t>(time));
    if (time) {
        delete time;
        LOG_DEBUG("CTime deleted");
    }
}

void delete_timestamp(CTimestamp* timestamp) noexcept {
    LOG_DEBUG("Deleting CTimestamp object: {}", reinterpret_cast<uintptr_t>(timestamp));
    if (timestamp) {
        delete timestamp;
        LOG_DEBUG("CTimestamp deleted");
    }
}

void close_binary_stream(ChunkedBinaryStream* stream) noexcept {
    LOG_DEBUG("Deleting ChunkedBinaryStream object: {}", reinterpret_cast<uintptr_t>(stream));
    if (stream) {
        delete stream;
        LOG_DEBUG("ChunkedBinaryStream deleted");
    }
}