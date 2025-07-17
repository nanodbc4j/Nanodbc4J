#pragma once
#include <nanodbc/nanodbc.h>
#include "error_info.h"
#include "datasource.h"

extern "C" {
    nanodbc::connection* connection(const char16_t* connection_string, long timeout, NativeError* error);

    void disconnect (nanodbc::connection* conn, NativeError* error);

    int is_connected(const nanodbc::connection* conn, NativeError* error);

    nanodbc::statement* create_statement(nanodbc::connection* conn, NativeError* error);

    nanodbc::statement* prepare_statement(nanodbc::connection* conn, const char16_t* sql, long timeout, NativeError* error);

    nanodbc::result* execute(nanodbc::statement* stmt, NativeError* error);

    void close_result(nanodbc::result* results, NativeError* error);

    void close_statement(nanodbc::statement* stmt, NativeError* error);

    const char16_t** drivers_list(int* count);

    const datasource** datasources_list(int* count);

    void std_free(void* ptr);
}
