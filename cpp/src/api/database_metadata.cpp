#include "api/database_metadata.h"
#include "utils/logger.hpp"

CDatabaseMetaData* get_database_meta_data(nanodbc::connection* conn, NativeError* error) {
	LOG_DEBUG("Getting metadata from connection: {}", reinterpret_cast<uintptr_t>(conn));
	init_error(error);
	try {
		if (!conn) {
			LOG_ERROR("Connection pointer is null, cannot get metadata");
			set_error(error, 2, "DatabaseMetaData", "Result is null");
			return nullptr;
		}

		const DatabaseMetaData database_meta_data(*conn);
		auto meta_data = new CDatabaseMetaData(database_meta_data);
		LOG_DEBUG("Metadata created successfully: {}", reinterpret_cast<uintptr_t>(meta_data));
		return meta_data;
	} catch (const std::exception& e) {
		set_error(error, 2, "DatabaseMetaData", e.what());
		LOG_ERROR_W(L"Exception in get_database_meta_data: {}", utils::to_wstring(e.what()));
	} catch (...) {
		set_error(error, -1, "UnknownError", "Unknown get meta data error");
		LOG_ERROR("Unknown exception in get_database_meta_data");
	}
	return nullptr;
}

void delete_database_meta_data(CDatabaseMetaData* meta_data) {
	LOG_DEBUG("Deleting metadata: {}", reinterpret_cast<uintptr_t>(meta_data));
	if (meta_data) {
		delete meta_data;
		LOG_DEBUG("Metadata deleted successfully");
		return;
	}
	LOG_ERROR("Attempted to delete null metadata");
}