#include "api/statement.h"
#include "utils/string_utils.h"

using namespace std;
using namespace utils;

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

int execute_update(nanodbc::statement* stmt, NativeError* error) {
    init_error(error);
    try {
        auto results = stmt->execute();
        return results.rowset_size();
    } catch (const exception& e) {
        set_error(error, 2, "ExecuteError", e.what());
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown execute statement error");
    }
    return 0;
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