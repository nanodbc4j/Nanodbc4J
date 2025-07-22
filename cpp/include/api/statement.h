#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"

extern "C" {

    void prepare_statement(nanodbc::statement* stmt, const char16_t* sql, long timeout, NativeError* error);

    nanodbc::result* execute(nanodbc::statement* stmt, NativeError* error);

    int execute_update(nanodbc::statement* stmt, NativeError* error);

    void close_statement(nanodbc::statement* stmt, NativeError* error);

}