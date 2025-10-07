#include "api/odbc.h"
#include <exception>
#include "utils/string_utils.hpp"
#include "utils/struct_converter.hpp"
#include "utils/logger.hpp"


const Driver** drivers_list(int* count) {
	LOG_DEBUG("Fetching list of ODBC drivers...");
    auto drivers_list = nanodbc::list_drivers();
    *count = static_cast<int>(drivers_list.size());
	LOG_DEBUG("Found {} ODBC drivers", *count);
    return converter::convert(drivers_list);
}

const Datasource** datasources_list(int* count) {
	LOG_DEBUG("Fetching list of ODBC data sources...");
    auto datasources = nanodbc::list_datasources();
    *count = static_cast<int>(datasources.size());
	LOG_DEBUG("Found {} ODBC data sources", *count);
    return converter::convert(datasources);
}

int set_log_level(int level) {
	try {
		auto result_level_name = set_spdlog_level(level);
		LOG_DEBUG("Set log level: {}", result_level_name);
		return ErrorCode::Success;
	} catch (const std::exception& e) {
		LOG_ERROR("Exception in set_log_level: {}", e.what());
	} catch (...) {
		LOG_ERROR("Unknown exception in get_value");
	}
	return 1;
}
 
void std_free(void* ptr) {
	LOG_DEBUG("Freeing memory at address: {}", reinterpret_cast<uintptr_t>(ptr));
    if (ptr) {
        free(ptr);
		LOG_DEBUG("Memory freed successfully");
		return;
    }
}

void delete_datasource(Datasource* datasource) {
	LOG_DEBUG("Deleting Datasource object: {}", reinterpret_cast<uintptr_t>(datasource));
	if (datasource) {
		delete datasource;
		LOG_DEBUG("Datasource deleted");
	}
}

void delete_datasource_array(Datasource** datasource_array, int size) {
	LOG_DEBUG("Deleting array of Datasource objects: {} elements, address: {}", size, reinterpret_cast<uintptr_t>(datasource_array));
	if (!datasource_array) {
		LOG_ERROR("Datasource array is null, nothing to delete");
		return;
	}

	for (int i = 0; i < size; ++i) {
		LOG_DEBUG("Deleting Datasource at index {}: {}", i, reinterpret_cast<uintptr_t>(datasource_array[i]));
		delete_datasource(datasource_array[i]);
	}
	delete[] datasource_array;
	LOG_DEBUG("Datasource array deleted");
}

void delete_driver(Driver* driver) {
	LOG_DEBUG("Deleting Driver object: {}", reinterpret_cast<uintptr_t>(driver));
	if (driver) {
		delete driver;
		LOG_DEBUG("Driver deleted");
	}
}

void delete_driver_array(Driver** driver_array, int size) {
	LOG_DEBUG("Deleting array of Driver objects: {} elements, address: {}", size, reinterpret_cast<uintptr_t>(driver_array));
	if (!driver_array) {
		LOG_ERROR("Driver array is null, nothing to delete");
		return;
	}

	for (int i = 0; i < size; ++i) {
		LOG_DEBUG("Deleting Driver at index {}: {}", i, reinterpret_cast<uintptr_t>(driver_array[i]));
		delete_driver(driver_array[i]);
	}
	delete[] driver_array;
	LOG_DEBUG("Driver array deleted");
}