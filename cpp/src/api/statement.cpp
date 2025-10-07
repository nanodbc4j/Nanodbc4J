#include "api/statement.h"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

using namespace utils;

template<typename T>
static void set_value_with_error_handling(nanodbc::statement* stmt, int index, const T& value, NativeError* error) {
    init_error(error);
    try {        
        stmt->bind(index, &value);
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR_W(L"Index range error in set_value: {}", to_wstring(e.what()));
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR_W(L"Type incompatible error in set_value: {}", to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR_W(L"Standard exception in set_value: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in set_value");
    }
}

static void set_value_with_error_handling(nanodbc::statement* stmt, int index, const nanodbc::string& value, NativeError* error) {
    init_error(error);
    try {
        // Оборачиваем одну строку в вектор
        std::vector<nanodbc::string> vec{ value };
        stmt->bind_strings(index, vec);
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR_W(L"Index range error (String): {}", to_wstring(e.what()));
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR_W(L"Type incompatible error (String): {}", to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR_W(L"Standard exception (String {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error setting string");
        LOG_ERROR("Unknown exception (String)");
    }
}

static void set_value_with_error_handling(nanodbc::statement* stmt, int index, nullptr_t, NativeError* error) {
    init_error(error);
    try {
        stmt->bind_null(index);
    } catch (const nanodbc::index_range_error& e) {
        set_error(error, ErrorCode::Database, "IndexError", e.what());
        LOG_ERROR_W(L"Index range error (NULL): {}", to_wstring(e.what()));
    } catch (const nanodbc::type_incompatible_error& e) {
        set_error(error, ErrorCode::Database, "TypeError", e.what());
        LOG_ERROR_W(L"Type incompatible error (NULL): {}", to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "DatabaseError", e.what());
        LOG_ERROR_W(L"Standard exception (NULL): {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error setting NULL");
        LOG_ERROR("Unknown exception (NULL)");
    }
}

void prepare_statement(nanodbc::statement* stmt, const ApiChar* sql, long timeout, NativeError* error) {
    auto wide_sql = to_wstring(sql);
    LOG_DEBUG_W(L"Preparing statement: {}", wide_sql);
    LOG_DEBUG("Timeout: {}", timeout);
    LOG_DEBUG("Statement object: {}", reinterpret_cast<uintptr_t>(stmt));

    init_error(error);
    try {
        if (!stmt) {
            LOG_ERROR("Statement is null, cannot prepare");
            set_error(error, ErrorCode::Database, "StatementError", "Statement is null");
            return;
        }
        nanodbc::prepare(*stmt, sql, timeout);
    } catch (const nanodbc::database_error& e) {
        set_error(error, ErrorCode::Database, "StatementError", e.what());
        LOG_ERROR_W(L"Database error during prepare: {}", to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "StatementError", e.what());
        LOG_ERROR_W(L"Standard exception during prepare: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown create statement error");
        LOG_ERROR("Unknown exception during prepare");
    }
}

void set_int_value(nanodbc::statement* stmt, int index, int value, NativeError* error) {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_long_value(nanodbc::statement* stmt, int index, long value, NativeError* error) {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_double_value(nanodbc::statement* stmt, int index, double value, NativeError* error) {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_bool_value(nanodbc::statement* stmt, int index, bool value, NativeError* error) {
    // тип bool не поддерживается nanodbc, используем short
    set_value_with_error_handling<short>(stmt, index, value, error);
}

void set_float_value(nanodbc::statement* stmt, int index, float value, NativeError* error) {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_short_value(nanodbc::statement* stmt, int index, short value, NativeError* error) {
    set_value_with_error_handling(stmt, index, value, error);
}

void set_string_value(nanodbc::statement* stmt, int index, const ApiChar* value, NativeError* error) {
    if (!value) {
        return set_value_with_error_handling(stmt, index, nullptr, error);
    }
    
    set_value_with_error_handling(stmt, index, nanodbc::string(value), error);
}

void set_date_value(nanodbc::statement* stmt, int index, CDate* value, NativeError* error) {
    if (!value) {
        set_value_with_error_handling(stmt, index, nullptr, error);
        return;
    }

    nanodbc::date d{};
    d.year = static_cast<short>(value->year);
    d.month = static_cast<short>(value->month);
    d.day = static_cast<short>(value->day);

    set_value_with_error_handling(stmt, index, d, error);
}

void set_time_value(nanodbc::statement* stmt, int index, CTime* value, NativeError* error) {
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

void set_timestamp_value(nanodbc::statement* stmt, int index, CTimestamp* value, NativeError* error) {
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

nanodbc::result* execute(nanodbc::statement* stmt, NativeError* error) {
    LOG_DEBUG("Executing statement: {}", reinterpret_cast<uintptr_t>(stmt));
    init_error(error);
    try {
        if (!stmt) {
            LOG_ERROR("Statement is null, cannot execute");
            set_error(error, ErrorCode::Database, "ExecuteError", "Statement is null");
            return nullptr;
        }
        auto results = stmt->execute();
        auto result_ptr = new nanodbc::result(std::move(results));
        LOG_DEBUG("Execute succeeded, result: {}", reinterpret_cast<uintptr_t>(result_ptr));
        return result_ptr;
    } catch (const nanodbc::database_error& e) {
        set_error(error, ErrorCode::Database, "ExecuteError", e.what());
        LOG_ERROR_W(L"Database error during execute: {}", to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "ExecuteError", e.what());
        LOG_ERROR_W(L"Database error during execute: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown execute statement error");
        LOG_ERROR("Unknown exception during execute");
    }
    return nullptr;
}

int execute_update(nanodbc::statement* stmt, NativeError* error) {
    LOG_DEBUG("Executing update on statement: {}", reinterpret_cast<uintptr_t>(stmt));
    init_error(error);
    try {
        if (!stmt) {
            LOG_ERROR("Statement is null, cannot execute update");
            set_error(error, ErrorCode::Database, "ExecuteError", "Statement is null");
            return 0;
        }
        auto results = stmt->execute();
        int affected_rows = static_cast<int>(results.rowset_size());
        LOG_DEBUG("Update executed successfully, affected rows: {}", affected_rows);
        return affected_rows;
    } catch (const nanodbc::database_error& e) {
        set_error(error, ErrorCode::Database, "ExecuteError", e.what());
        LOG_ERROR_W(L"Database error during execute_update: {}", to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "ExecuteError", e.what());
        LOG_ERROR_W(L"Standard exception during execute_update: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown execute statement error");
        LOG_ERROR("Unknown exception during execute_update");
    }
    return 0;
}


void close_statement(nanodbc::statement* stmt, NativeError* error) {
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
        LOG_ERROR_W(L"Database error during close_statement: {}", to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "StatementError", e.what());
        LOG_ERROR_W(L"Standard exception during close_statement: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown close statement error");
        LOG_ERROR("Unknown exception during close_statement");
    }
}