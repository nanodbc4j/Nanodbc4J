#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"

extern "C" {

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

}