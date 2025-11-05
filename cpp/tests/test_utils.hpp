#pragma once

void assert_no_error(const NativeError& err);

void assert_has_error(const NativeError& err);

ApiString get_connection_string();

Connection* create_in_memory_db(NativeError& error);