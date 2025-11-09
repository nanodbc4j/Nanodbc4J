#include "api/statement.h"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"
#include "core/string_proxy.hpp"

using namespace utils;

#define BATCH_OPERATIONS 1

template<typename T>
static void set_value_with_error_handling(nanodbc::statement* stmt, int index, const T& value, NativeError* error) noexcept {
    init_error(error);
    try {        
        stmt->bind(index, &value);
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error in set_value: {}", e.what());
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error in set_value: {}", e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Standard exception in set_value: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in set_value");
    }
}

static void set_value_with_error_handling(nanodbc::statement* stmt, int index, const StringProxy<wchar_t> & value, NativeError* error) noexcept {
    init_error(error);
    try {
        // Оборачиваем одну строку в вектор
        std::vector<nanodbc::string> vec;
        vec.emplace_back(value);
        stmt->bind_strings(index, vec);
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error (String): {}", e.what());
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error (String): {}", e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Standard exception (String {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error setting string");
        LOG_ERROR("Unknown exception (String)");
    }
}

static void set_value_with_error_handling(nanodbc::statement* stmt, int index, nullptr_t, NativeError* error) noexcept {
    init_error(error);
    try {
        stmt->bind_null(index);
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR("Index range error (NULL): {}", e.what());
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR("Type incompatible error (NULL): {}", e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Standard exception (NULL): {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error setting NULL");
        LOG_ERROR("Unknown exception (NULL)");
    }
}

void prepare_statement(nanodbc::statement* stmt, const wchar_t* sql, NativeError* error) noexcept {
    const StringProxy str_sql (sql);
    init_error(error);

    try {
        LOG_DEBUG("Preparing statement: {}", str_sql);
        LOG_DEBUG("Statement object: {}", reinterpret_cast<uintptr_t>(stmt));

        if (!stmt) {
            LOG_ERROR("Statement is null, cannot prepare");
            set_error(error, ErrorCode::Database, "StatementError", "Statement is null");
            return;
        }
        nanodbc::prepare(*stmt, static_cast<nanodbc::string>(str_sql));
    } catch (const nanodbc::database_error& e) {
        set_error(error, ErrorCode::Database, "StatementError", e.what());
        LOG_ERROR("Database error during prepare: {}", e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "StatementError", e.what());
        LOG_ERROR("Standard exception during prepare: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown create statement error");
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
    // тип bool не поддерживается nanodbc, используем short
    set_value_with_error_handling<short>(stmt, index, value, error);
}

void set_float_value(nanodbc::statement* stmt, int index, float value, NativeError* error) noexcept {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_short_value(nanodbc::statement* stmt, int index, short value, NativeError* error) noexcept {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_string_value(nanodbc::statement* stmt, int index, const wchar_t* value, NativeError* error) noexcept {
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
    t.hour = static_cast<short>(value->hour);
    t.min = static_cast<short>(value->minute);
    t.sec = static_cast<short>(value->second);

    set_value_with_error_handling(stmt, index, t, error);
}

void set_timestamp_value(nanodbc::statement* stmt, int index, CTimestamp* value, NativeError* error) noexcept {
    if (!value) {
        set_value_with_error_handling(stmt, index, nullptr, error);
        return;
    }

    nanodbc::timestamp ts{};
    ts.year = static_cast<short>(value->year);
    ts.month = static_cast<short>(value->month);
    ts.day = static_cast<short>(value->day);
    ts.hour = static_cast<short>(value->hour);
    ts.min = static_cast<short>(value->minute);
    ts.sec = static_cast<short>(value->second);
    ts.fract = static_cast<long>(value->fract);

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
            std::vector<std::vector<uint8_t>> empty_batch;
            stmt->bind(index, empty_batch);
        } else {
            std::vector<std::vector<uint8_t>> batch;
            batch.push_back(data);
            stmt->bind(index, batch);
        }
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR("Exception in set_binary_array_value: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in set_binary_array_value");
    }
}

nanodbc::result* execute(nanodbc::statement* stmt, int timeout, NativeError* error) noexcept {
    LOG_DEBUG("Executing statement: {}", reinterpret_cast<uintptr_t>(stmt));
    init_error(error);
    try {
        if (!stmt) {
            LOG_ERROR("Statement is null, cannot execute");
            set_error(error, ErrorCode::Database, "ExecuteError", "Statement is null");
            return nullptr;
        }
        auto results = stmt->execute(BATCH_OPERATIONS, timeout);
        results.unbind();
        auto result_ptr = new nanodbc::result(std::move(results));
        LOG_DEBUG("Execute succeeded, result: {}", reinterpret_cast<uintptr_t>(result_ptr));
        return result_ptr;
    } catch (const nanodbc::database_error& e) {
        set_error(error, ErrorCode::Database, "ExecuteError", e.what());
        LOG_ERROR("Database error during execute: {}", e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "ExecuteError", e.what());
        LOG_ERROR("Database error during execute: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown execute statement error");
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
    } catch (const nanodbc::database_error& e) {
        set_error(error, ErrorCode::Database, "StatementError", e.what());
        LOG_ERROR("Database error during cancel_statement: {}", e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "StatementError", e.what());
        LOG_ERROR("Standard exception during cancel_statement: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown cancel statement error");
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
    } catch (const nanodbc::database_error& e) {
        set_error(error, ErrorCode::Database, "StatementError", e.what());
        LOG_ERROR("Database error during close_statement: {}", e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "StatementError", e.what());
        LOG_ERROR("Standard exception during close_statement: {}", e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown close statement error");
        LOG_ERROR("Unknown exception during close_statement");
    }
}