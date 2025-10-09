#include "api/result.h"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

using namespace std;
using namespace utils;

template<typename T>
static T get_value_by_name(nanodbc::result* results, const nanodbc::string& column_name, NativeError* error, T fallback = T{}) {
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Result is null");
            set_error(error, ErrorCode::Database, "ResultError", "Result is null");
            return fallback;
        }

        if (results->is_null(column_name)) {
            LOG_DEBUG("Column is NULL, returning fallback");
            return fallback;
        }

        return results->get<T>(column_name, fallback);
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error in get_value: {}", e.what());
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error in get_value: {}", e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Standard exception in get_value: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in get_value");
    }
    return fallback;
}

template<typename T>
static T get_value_by_index(nanodbc::result* results, int index, NativeError* error, T fallback = T{}) {
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Result is null");
            set_error(error, ErrorCode::Database, "ResultError", "Result is null");
            return fallback;
        }

        if (results->is_null(index)) {
            LOG_DEBUG("Column is NULL, returning fallback");
            return fallback;
        }

        return results->get<T>(index, fallback);
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error in get_value: {}", e.what());
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error in get_value: {}", e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Standard exception in get_value: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in get_value");
    }
    return fallback;
}


bool next_result(nanodbc::result* results, NativeError* error) {
    LOG_DEBUG("Calling next() on result: {}", reinterpret_cast<uintptr_t>(results));
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Result is null, next() returns false");
            return false;
        }
        bool has_next = results->next();
        LOG_DEBUG("next() result: {}", has_next ? "true" : "false");
        return has_next;
    } catch (const exception& e) {
        set_error(error, ErrorCode::Standard, "ResultError", e.what());
        LOG_ERROR("Exception in next_result: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown next result error");
        LOG_ERROR("Unknown exception in next_result");
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

const ApiChar* get_string_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    LOG_DEBUG("Getting string value by index: {}", index);
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Result is null");
            set_error(error, ErrorCode::Database, "ResultError", "Result is null");
            return nullptr;
        }

        if (results->is_null(index)) {
            LOG_DEBUG("Column '{}' is NULL", index);
            return nullptr;
        }

        auto value = results->get<nanodbc::string>(index);
        LOG_DEBUG("String value retrieved from index {}: '{}'", index, to_string(value));
        return duplicate_string(value.c_str(), value.length());
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error at index {}: {}", index, e.what());
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error at index {}: {}", index, e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception in get_string_value_by_index {}: {}", index, e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in get_string_value_by_index {}", index);
    }
    return nullptr;
}

CDate* get_date_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    if (was_null_by_index(results, index, error) || error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", index);
        return nullptr;
    }
    auto date = get_value_by_index<nanodbc::date>(results, index, error, {});
    return new CDate(date);
}

CTime* get_time_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    if (was_null_by_index(results, index, error) || error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", index);
        return nullptr;
    }
    auto time = get_value_by_index<nanodbc::time>(results, index, error, {});
    return new CTime(time);
}

CTimestamp* get_timestamp_value_by_index(nanodbc::result* results, int index, NativeError* error) {
    if (was_null_by_index(results, index, error) || error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", index);
        return nullptr;
    }
    auto timestamp = get_value_by_index<nanodbc::timestamp>(results, index, error, {});
    return new CTimestamp(timestamp);
}

ChunkedBinaryStream* get_binary_stream_by_index(nanodbc::result* results, int index, NativeError* error) {
    LOG_DEBUG("Getting binary stream by index: {}", index);
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Result is null");
            set_error(error, ErrorCode::Database, "ResultError", "Result is null");
            return nullptr;
        }

        if (results->is_null(index)) {
            LOG_DEBUG("Column '{}' is NULL", index);
            return nullptr;
        }

        auto result = new ChunkedBinaryStream(results, index);
        LOG_DEBUG("Binary stream ptr retrieved from index {}: '{}'", index, reinterpret_cast<uintptr_t>(results));
        return result;
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error at index {}: {}", index, e.what());
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error at index {}: {}", index, e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception in get_string_value_by_index {}: {}", index, e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in get_string_value_by_index {}", index);
    }
    return nullptr;
}

int read_binary_stream(ChunkedBinaryStream* stream, uint8_t* buffer, int offset, int length, NativeError* error) {
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
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception in read_binary_stream: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception");
    }
    return -1;
}

BinaryArray* get_bytes_array_by_index(nanodbc::result* results, int index, NativeError* error) {
    if (was_null_by_index(results, index, error) || error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", index);
        return nullptr;
    }
    try {
        // Пробуем получить как binary данные
        std::vector<uint8_t> binary_data = results->get<std::vector<uint8_t>>(index);
        return new BinaryArray(binary_data);

    } catch (const nanodbc::database_error& e) {
        // Если не binary, пробуем как строку (для CLOB)
        try {
            std::string string_data = results->get<std::string>(index);
            std::vector<uint8_t> binary_data(string_data.begin(), string_data.end());
            return new BinaryArray(binary_data);
        } catch (const nanodbc::index_range_error& e) {
            set_error(error, ErrorCode::Database, "IndexError", e.what());
            LOG_ERROR("Index range error at index {}: {}", index, e.what());
        } catch (const nanodbc::type_incompatible_error& e) {
            set_error(error, ErrorCode::Database, "TypeError", e.what());
            LOG_ERROR("Type incompatible error at index {}: {}", index, e.what());
        } catch (const std::exception& e) {
            set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
            LOG_ERROR("Exception in get_string_value_by_index {}: {}", index, e.what());
        } catch (...) {
            set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
            LOG_ERROR("Unknown exception in get_string_value_by_index {}", index);
        }
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error at index {}: {}", index, e.what());
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error at index {}: {}", index, e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception in get_string_value_by_index {}: {}", index, e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in get_string_value_by_index {}", index);
    } 
    return nullptr;
}

bool was_null_by_index(nanodbc::result* results, int index, NativeError* error) {
    LOG_DEBUG("Closing result: {}", reinterpret_cast<uintptr_t>(results));
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Attempted to close null result");
            return 0;
        }
        bool is_null = results->is_null(index);
        LOG_DEBUG("Index was null '{}': '{}'", index, is_null);
        return is_null;
    }  catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error for column '{}': {}", index, e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception '{}': {}", index, e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception '{}'", index);
    } 
    return true;
}

int get_int_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    return get_value_by_name<int>(results, name, error, 0);
}

long get_long_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    return get_value_by_name<long>(results, name, error, 0L);
}

double get_double_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    return get_value_by_name<double>(results, name, error, 0.0);
}

bool get_bool_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    // result->get<bool>() не работает
    return get_value_by_name<short>(results, name, error, 0);
}

float get_float_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    return get_value_by_name<float>(results, name, error, 0.0f);
}

short get_short_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    return get_value_by_name<short>(results, name, error, 0);
}

const ApiChar* get_string_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    int index = find_column_by_name(results, name, error);
    if (error->error_code) {
        return nullptr;
    }

    LOG_DEBUG("Getting string value by name: '{}'", to_string(name));
    
    return get_string_value_by_index(results, index, error);
}

CDate* get_date_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    if (was_null_by_name(results, name, error) || error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", to_string(name));
        return nullptr;
    }
    auto date = get_value_by_name<nanodbc::date>(results, name, error, {});
    return new CDate(date);
}

CTime* get_time_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    if (was_null_by_name(results, name, error) || error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", to_string(name));
        return nullptr;
    }
    auto time = get_value_by_name<nanodbc::time>(results, name, error, {});
    return new CTime(time);
}

CTimestamp* get_timestamp_value_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    if (was_null_by_name(results, name, error) || error->error_code) {
        LOG_DEBUG("Column '{}' is NULL", to_string(name));
        return nullptr;
    }
    auto timestamp = get_value_by_name<nanodbc::timestamp>(results, name, error, {});
    return new CTimestamp(timestamp);
}

BinaryArray* get_bytes_array_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    int index = find_column_by_name(results, name, error);
    if (error->error_code) {
        return nullptr;
    }
    return get_bytes_array_by_index(results, index, error);
}

ChunkedBinaryStream* get_binary_stream_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    int index = find_column_by_name(results, name, error);
    if (error->error_code) {
        return nullptr;
    }
    return  get_binary_stream_by_index(results, index, error);
}

int find_column_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    LOG_DEBUG("Closing result: {}", reinterpret_cast<uintptr_t>(results));
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Attempted to close null result");
            return 0;
        }
        int index = results->column(name);
        LOG_DEBUG("Index retrieved by name '{}': '{}'", to_string(name), index);
        return index;
    }  catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error for column '{}': {}", to_string(name), e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception '{}': {}", to_string(name), e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception '{}'", to_string(name));
    } 
    return 0;
}

bool was_null_by_name(nanodbc::result* results, const ApiChar* name, NativeError* error) {
    LOG_DEBUG("Closing result: {}", reinterpret_cast<uintptr_t>(results));
    init_error(error);
    try {
        if (!results) {
            LOG_ERROR("Attempted to close null result");
            return 0;
        }
        bool is_null = results->is_null(name);
        LOG_DEBUG("Index was null '{}': '{}'", to_string(name), is_null);
        return is_null;
    }  catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error for column '{}': {}", to_string(name), e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception '{}': {}", to_string(name), e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception '{}'", to_string(name));
    } 
    return true;
}

void close_result(nanodbc::result* results, NativeError* error) {
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
        LOG_ERROR("Exception in close_result: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown close result error");
        LOG_ERROR("Unknown exception in close_result");
    }
}

void delete_binary_array(BinaryArray* array) {
    LOG_DEBUG("Deleting BinaryArray object: {}", reinterpret_cast<uintptr_t>(array));
    if (array) {
        delete array;
        LOG_DEBUG("BinaryArray deleted");
    }
}

void delete_date(CDate* date) {
    LOG_DEBUG("Deleting CDate object: {}", reinterpret_cast<uintptr_t>(date));
    if (date) {
        delete date;
        LOG_DEBUG("CDate deleted");
    }
}

void delete_time(CTime* time) {
    LOG_DEBUG("Deleting CTime object: {}", reinterpret_cast<uintptr_t>(time));
    if (time) {
        delete time;
        LOG_DEBUG("CTime deleted");
    }
}

void delete_timestamp(CTimestamp* timestamp) {
    LOG_DEBUG("Deleting CTimestamp object: {}", reinterpret_cast<uintptr_t>(timestamp));
    if (timestamp) {
        delete timestamp;
        LOG_DEBUG("CTimestamp deleted");
    }
}

void close_binary_stream(ChunkedBinaryStream* stream) {
    LOG_DEBUG("Deleting ChunkedBinaryStream object: {}", reinterpret_cast<uintptr_t>(stream));
    if (stream) {
        delete stream;
        LOG_DEBUG("ChunkedBinaryStream deleted");
    }
}