#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "struct/nanodbc_c.h"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    ODBC_API const Driver** drivers_list(int* count) noexcept;

    ODBC_API const Datasource** datasources_list(int* count) noexcept;

    ODBC_API void std_free(void* ptr) noexcept;

    ODBC_API void clear_native_error(NativeError* error) noexcept;

    ODBC_API int set_log_level(int level) noexcept;

    ODBC_API void delete_datasource(Datasource* datasource) noexcept;

    ODBC_API void delete_datasource_array(Datasource** datasource_array, int size) noexcept;

    ODBC_API void delete_driver(Driver* driver) noexcept;

    ODBC_API void delete_driver_array(Driver** driver_array, int size) noexcept;

#ifdef __cplusplus
} // extern "C"
#endif
