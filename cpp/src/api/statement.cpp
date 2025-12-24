#include "api/statement.h"
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

using namespace utils;

#define BATCH_OPERATIONS 1

template<typename T>
static void set_value_with_error_handling(nanodbc::statement* stmt, int index, const T& value, NativeError* error) noexcept {
    init_error(error);
    try {        
        stmt->bind(index, &value);
    } catch (const std::exception& e) {
        set_error(error, e.what());
        LOG_ERROR("Standard exception in set_value: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown error");
        LOG_ERROR("Unknown exception in set_value");
    }
}

static void set_value_with_error_handling(nanodbc::statement* stmt, int index, const StringProxy<ApiChar> & value, NativeError* error) noexcept {
    init_error(error);
    try {
        // Wrap a single string in a vector
        std::vector<nanodbc::string> vec;
        vec.emplace_back(value);
        stmt->bind_strings(static_cast<short>(index), vec);
    } catch (const std::exception& e) {
        set_error(error, e.what());
        LOG_ERROR("Standard exception (String {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown error setting string");
        LOG_ERROR("Unknown exception (String)");
    }
}

static void set_value_with_error_handling(nanodbc::statement* stmt, int index, nullptr_t, NativeError* error) noexcept {
    init_error(error);
    try {
        stmt->bind_null(static_cast<short>(index));
    } catch (const std::exception& e) {
        set_error(error, e.what());
        LOG_ERROR("Standard exception (NULL): {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown error setting NULL");
        LOG_ERROR("Unknown exception (NULL)");
    }
}

void prepare_statement(nanodbc::statement* stmt, const ApiChar* sql, NativeError* error) noexcept {
    const StringProxy str_sql (sql);
    init_error(error);

    try {
        LOG_DEBUG("Preparing statement: {}", str_sql);
        LOG_DEBUG("Statement object: {}", reinterpret_cast<uintptr_t>(stmt));

        if (!stmt) {
            LOG_ERROR("Statement is null, cannot prepare");
            set_error(error, "Statement is null");
            return;
        }
        nanodbc::prepare(*stmt, static_cast<nanodbc::string>(str_sql));
    } catch (const std::exception& e) {
        set_error(error, e.what());
        LOG_ERROR("Standard exception during prepare: {}", StringProxy( e.what()));
    } catch (...) {
        set_error(error, "Unknown create statement error");
        LOG_ERROR("Unknown exception during prepare");
    }
}

void set_int_value(nanodbc::statement* stmt, int index, int value, NativeError* error) noexcept {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_long_value(nanodbc::statement* stmt, int index, long value, NativeError* error) noexcept {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_double_value(nanodbc::statement* stmt, int index, double value, NativeError* error) noexcept {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_bool_value(nanodbc::statement* stmt, int index, bool value, NativeError* error) noexcept {
    // bool type is not supported by nanodbc
    set_value_with_error_handling<BOOL>(stmt, index, value, error);
}

void set_float_value(nanodbc::statement* stmt, int index, float value, NativeError* error) noexcept {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_short_value(nanodbc::statement* stmt, int index, short value, NativeError* error) noexcept {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_string_value(nanodbc::statement* stmt, int index, const ApiChar* value, NativeError* error) noexcept {
    if (!value) {
        set_value_with_error_handling(stmt, index, nullptr, error);
        return;
    }
    const StringProxy str_value (value);
    set_value_with_error_handling(stmt, index, str_value, error);
}

void set_date_value(nanodbc::statement* stmt, int index, CDate* value, NativeError* error) noexcept {
    if (!value) {
        set_value_with_error_handling(stmt, index, nullptr, error);
        return;
    }

    nanodbc::date d{};
    d.year = value->year;
    d.month = value->month;
    d.day = value->day;

    set_value_with_error_handling(stmt, index, d, error);
}

void set_time_value(nanodbc::statement* stmt, int index, CTime* value, NativeError* error) noexcept {
    if (!value) {
        set_value_with_error_handling(stmt, index, nullptr, error);
        return;
    }

    nanodbc::time t{};
    t.hour = value->hour;
    t.min = value->minute;
    t.sec = value->second;

    set_value_with_error_handling(stmt, index, t, error);
}

void set_timestamp_value(nanodbc::statement* stmt, int index, CTimestamp* value, NativeError* error) noexcept {
    if (!value) {
        set_value_with_error_handling(stmt, index, nullptr, error);
        return;
    }

    nanodbc::timestamp ts{};
    ts.year = value->year;
    ts.month = value->month;
    ts.day = value->day;
    ts.hour = value->hour;
    ts.min = value->minute;
    ts.sec = value->second;
    ts.fract = value->fract;

    set_value_with_error_handling(stmt, index, ts, error);
}

void set_binary_array_value(nanodbc::statement* stmt, int index, BinaryArray* value, NativeError* error) noexcept {
    if (!value) {
        set_value_with_error_handling(stmt, index, nullptr, error);
        return;
    }

    init_error(error);
    try {
        std::vector<uint8_t> data = value->to_vector();
        if (data.empty()) {
            const std::vector<std::vector<uint8_t>> empty_batch;
            stmt->bind(static_cast<short>(index), empty_batch);
        } else {
            std::vector<std::vector<uint8_t>> batch;
            batch.push_back(data);
            stmt->bind(static_cast<short>(index), batch);
        }
    } catch (const std::exception& e) {
        set_error(error, e.what());
        LOG_ERROR("Exception in set_binary_array_value: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown error");
        LOG_ERROR("Unknown exception in set_binary_array_value");
    }
}

ResultSet* execute(nanodbc::statement* stmt, int timeout, NativeError* error) noexcept {
    LOG_DEBUG("Executing statement: {}", reinterpret_cast<uintptr_t>(stmt));
    init_error(error);
    try {
        if (!stmt) {
            LOG_ERROR("Statement is null, cannot execute");
            set_error(error, "Statement is null");
            return nullptr;
        }
        auto results = stmt->execute(BATCH_OPERATIONS, timeout);
        results.unbind();
        auto result_ptr = new ResultSet(std::move(results));
        LOG_DEBUG("Execute succeeded, result: {}", reinterpret_cast<uintptr_t>(result_ptr));
        return result_ptr;
    } catch (const std::exception& e) {
        set_error(error, e.what());
        LOG_ERROR("Database error during execute: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown execute statement error");
        LOG_ERROR("Unknown exception during execute");
    }
    return nullptr;
}

void cancel_statement(nanodbc::statement* stmt, NativeError* error) noexcept {
    LOG_DEBUG("Cancel statement: {}", reinterpret_cast<uintptr_t>(stmt));
    init_error(error);
    try {
        if (!stmt) {
            LOG_ERROR("Attempted to close null statement");
            return;
        }
        stmt->cancel();
        LOG_DEBUG("Statement cancel");
    } catch (const std::exception& e) {
        set_error(error, e.what());
        LOG_ERROR("Standard exception during cancel_statement: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown cancel statement error");
        LOG_ERROR("Unknown exception during cancel_statement");
    }
}

void close_statement(nanodbc::statement* stmt, NativeError* error) noexcept {
    LOG_DEBUG("Closing statement: {}", reinterpret_cast<uintptr_t>(stmt));
    init_error(error);
    try {
        if (!stmt) {
            LOG_ERROR("Attempted to close null statement");
            return;
        }
        stmt->close();
        LOG_DEBUG("Statement closed, now deleting object");
        delete stmt;
        LOG_DEBUG("Statement successfully closed and deleted");
    } catch (const std::exception& e) {
        set_error(error, e.what());
        LOG_ERROR("Standard exception during close_statement: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown close statement error");
        LOG_ERROR("Unknown exception during close_statement");
    }
}