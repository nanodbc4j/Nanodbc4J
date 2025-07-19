#pragma once
#include <nanodbc/nanodbc.h>
#include "error_info.h"
#include "nanodbc_c.h"

extern "C" {
    nanodbc::connection* connection(const char16_t* connection_string, long timeout, NativeError* error);

    void disconnect (nanodbc::connection* conn, NativeError* error);

    bool is_connected(const nanodbc::connection* conn, NativeError* error);

    nanodbc::statement* create_statement(nanodbc::connection* conn, NativeError* error);

    void prepare_statement(nanodbc::statement* stmt, const char16_t* sql, long timeout, NativeError* error);

    nanodbc::result* execute(nanodbc::statement* stmt, NativeError* error);

    bool next_result(nanodbc::result* results, NativeError* error);

    int get_int_value_by_index(nanodbc::result* results, int index, NativeError* error);

    long get_long_value_by_index(nanodbc::result* results, int index, NativeError* error);

    double get_double_value_by_index(nanodbc::result* results, int index, NativeError* error);

    bool get_bool_value_by_index(nanodbc::result* results, int index, NativeError* error);

    float get_float_value_by_index(nanodbc::result* results, int index, NativeError* error);

    short get_short_value_by_index(nanodbc::result* results, int index, NativeError* error);

    int get_int_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error);

    long get_long_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error);

    double get_double_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error);

    bool get_bool_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error);

    float get_float_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error);

    short get_short_value_by_name(nanodbc::result* results, const char16_t* name, NativeError* error);

    void close_result(nanodbc::result* results, NativeError* error);

    void close_statement(nanodbc::statement* stmt, NativeError* error);

    const char16_t** drivers_list(int* count);

    const datasource** datasources_list(int* count);

    void std_free(void* ptr);
}
