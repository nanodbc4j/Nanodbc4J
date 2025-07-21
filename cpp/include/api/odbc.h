#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/error_info.h"
#include "struct/nanodbc_c.h"

extern "C" {    
    
    const Driver** drivers_list(int* count);

    const Datasource** datasources_list(int* count);

    void std_free(void* ptr);

    void delete_datasource(Datasource* datasource);

    void delete_datasource_array(Datasource** datasource_array, int size);

    void delete_driver(Driver* driver);

    void delete_driver_array(Driver** driver_array, int size);
}
