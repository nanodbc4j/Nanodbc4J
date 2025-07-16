#pragma once
#include <nanodbc/nanodbc.h>
#include "error_info.h"

extern "C" {
    nanodbc::connection* connection(const char16_t* connection_string, NativeError* error);

    void disconnect (nanodbc::connection*, NativeError* error);

    int is_connected(const nanodbc::connection* conn, NativeError* error);

    void std_free(void* ptr);
}
