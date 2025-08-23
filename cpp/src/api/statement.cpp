#include "api/statement.h"
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"

using namespace std;
using namespace utils;

void prepare_statement(nanodbc::statement* stmt, const char16_t* sql, long timeout, NativeError* error) {
    auto wide_sql = to_wstring(sql);
    LOG_DEBUG_W(L"Preparing statement: {}", wide_sql);
    LOG_DEBUG("Timeout: {}", timeout);
    LOG_DEBUG("Statement object: {}", reinterpret_cast<uintptr_t>(stmt));

    init_error(error);
    try {
        if (!stmt) {
            LOG_DEBUG("Statement is null, cannot prepare");
            set_error(error, 2, "StatementError", "Statement is null");
            return;
        }
        nanodbc::prepare(*stmt, wide_sql, timeout);
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "StatementError", e.what());
        LOG_ERROR_W(L"Database error during prepare: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "StatementError", e.what());
        LOG_ERROR_W(L"Standard exception during prepare: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown create statement error");
        LOG_ERROR("Unknown exception during prepare");
    }
}

nanodbc::result* execute(nanodbc::statement* stmt, NativeError* error) {
    LOG_DEBUG("Executing statement: {}", reinterpret_cast<uintptr_t>(stmt));
    init_error(error);
    try {
        if (!stmt) {
            LOG_DEBUG("Statement is null, cannot execute");
            set_error(error, 2, "ExecuteError", "Statement is null");
            return nullptr;
        }
        auto results = stmt->execute();
        auto result_ptr = new nanodbc::result(results);
        LOG_DEBUG("Execute succeeded, result: {}", reinterpret_cast<uintptr_t>(result_ptr));
        return result_ptr;
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "ExecuteError", e.what());
        LOG_ERROR_W(L"Database error during execute: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "ExecuteError", e.what());
        LOG_ERROR_W(L"Database error during execute: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown execute statement error");
        LOG_ERROR("Unknown exception during execute");
    }
    return nullptr;
}

int execute_update(nanodbc::statement* stmt, NativeError* error) {
    LOG_DEBUG("Executing update on statement: {}", reinterpret_cast<uintptr_t>(stmt));
    init_error(error);
    try {
        if (!stmt) {
            LOG_DEBUG("Statement is null, cannot execute update");
            set_error(error, 2, "ExecuteError", "Statement is null");
            return 0;
        }
        auto results = stmt->execute();
        int affected_rows = static_cast<int>(results.rowset_size());
        LOG_DEBUG("Update executed successfully, affected rows: {}", affected_rows);
        return affected_rows;
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "ExecuteError", e.what());
        LOG_ERROR_W(L"Database error during execute_update: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "ExecuteError", e.what());
        LOG_ERROR_W(L"Standard exception during execute_update: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown execute statement error");
        LOG_ERROR("Unknown exception during execute_update");
    }
    return 0;
}


void close_statement(nanodbc::statement* stmt, NativeError* error) {
    LOG_DEBUG("Closing statement: {}", reinterpret_cast<uintptr_t>(stmt));
    init_error(error);
    try {
        if (!stmt) {
            LOG_DEBUG("Attempted to close null statement");
            return;
        }
        stmt->close();
        LOG_DEBUG("Statement closed, now deleting object");
        delete stmt;
        LOG_DEBUG("Statement successfully closed and deleted");
    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "StatementError", e.what());
        LOG_ERROR_W(L"Database error during close_statement: {}", to_wstring(e.what()));
    } catch (const exception& e) {
        set_error(error, 2, "StatementError", e.what());
        LOG_ERROR_W(L"Standard exception during close_statement: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown close statement error");
        LOG_ERROR("Unknown exception during close_statement");
    }
}