#include "api/odbc.h"
#include <exception>
#include "utils/string_utils.hpp"
#include "utils/logger.hpp"
#include "utils/string_proxy.hpp"

Driver** drivers_list(int* count) noexcept {
	LOG_DEBUG("Fetching list of ODBC drivers...");
    auto drivers_list = nanodbc::list_drivers();
    *count = static_cast<int>(drivers_list.size());
	LOG_DEBUG("Found {} ODBC drivers", *count);
    return Driver::convert(drivers_list);
}

Datasource** datasources_list(int* count) noexcept {
	LOG_DEBUG("Fetching list of ODBC data sources...");
    auto datasources = nanodbc::list_datasources();
    *count = static_cast<int>(datasources.size());
	LOG_DEBUG("Found {} ODBC data sources", *count);
    return Datasource::convert(datasources);
}

int set_log_level(int level) noexcept {
	try {
		auto result_level = set_spdlog_level(level);
		LOG_DEBUG("Set log level: {}", result_level);
		return result_level;
	} catch (const std::exception& e) {
		LOG_ERROR("Exception in set_log_level: {}", StringProxy(e.what()));
	} catch (...) {
		LOG_ERROR("Unknown exception in get_value");
	}
	return 1;
}
 
void std_free(void* ptr) noexcept {
	LOG_DEBUG("Freeing memory at address: {}", reinterpret_cast<uintptr_t>(ptr));
    if (ptr) {
        free(ptr);
		LOG_DEBUG("Memory freed successfully");
    }
}

void clear_native_error(NativeError* error) noexcept {
	LOG_TRACE("clear_native_error: error={}", reinterpret_cast<uintptr_t>(error));
	NativeError::clear(error);
}

void delete_datasource(Datasource* datasource) noexcept {
	LOG_DEBUG("Deleting Datasource object: {}", reinterpret_cast<uintptr_t>(datasource));
	if (datasource) {
		delete datasource;
		LOG_DEBUG("Datasource deleted");
	}
}

void delete_datasource_array(Datasource** datasource_array, int size) noexcept {
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

void delete_driver(Driver* driver) noexcept {
	LOG_DEBUG("Deleting Driver object: {}", reinterpret_cast<uintptr_t>(driver));
	if (driver) {
		delete driver;
		LOG_DEBUG("Driver deleted");
	}
}

void delete_driver_array(Driver** driver_array, int size) noexcept {
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