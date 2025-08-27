#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "struct/nanodbc_c.h"
#include "api/export.h"

extern "C" {    
    
    ODBC_API const Driver** drivers_list(int* count);

    ODBC_API const Datasource** datasources_list(int* count);

    ODBC_API void std_free(void* ptr);

    ODBC_API int set_log_level(int level);

    ODBC_API void delete_datasource(Datasource* datasource);

    ODBC_API void delete_datasource_array(Datasource** datasource_array, int size);

    ODBC_API void delete_driver(Driver* driver);

    ODBC_API void delete_driver_array(Driver** driver_array, int size);
}
