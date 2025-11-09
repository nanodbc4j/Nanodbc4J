#pragma once
#include "struct/error_info.h"
#include "api/connection.h"

void assert_no_error(const NativeError& err);

void assert_has_error(const NativeError& err);

std::wstring get_connection_string();

Connection* create_in_memory_db(NativeError& error);