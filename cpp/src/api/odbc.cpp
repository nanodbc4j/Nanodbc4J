#include "api/odbc.h"
#include <exception>
#include "utils/string_utils.h"
#include <utils/struct_converter.h>


const Driver** drivers_list(int* count) {
    auto drivers_list = nanodbc::list_drivers();
    *count = static_cast<int>(drivers_list.size());
    return converter::convert(drivers_list);
}

const Datasource** datasources_list(int* count) {
    auto datasources = nanodbc::list_datasources();
    *count = static_cast<int>(datasources.size());
    return converter::convert(datasources);
}
 
void std_free(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}

inline void delete_datasource(Datasource* datasource) {
	if (datasource) {
		delete datasource;
	}
}

void delete_datasource_array(Datasource** datasource_array, int size) {
	if (!datasource_array) {
		return;
	}

	for (int i = 0; i < size; ++i) {
		delete_datasource(datasource_array[i]);
		datasource_array[i] = nullptr;
	}
	delete[] datasource_array;
}

inline void delete_driver(Driver* driver) {
	if (driver) {
		delete driver;
	}
}

void delete_driver_array(Driver** driver_array, int size) {
	if (!driver_array) {
		return;
	}

	for (int i = 0; i < size; ++i) {
		delete_driver(driver_array[i]);
		driver_array[i] = nullptr;
	}
	delete[] driver_array;
}