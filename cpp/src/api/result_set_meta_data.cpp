#include "api/result_set_meta_data.h"
#include "utils/logger.hpp"

CResultSetMetaData* get_meta_data(nanodbc::result* results, NativeError* error) {
	LOG_DEBUG("Getting metadata from result: {}", reinterpret_cast<uintptr_t>(results));
	init_error(error);
	try {
		if (!results) {
			LOG_ERROR("Result pointer is null, cannot get metadata");
			set_error(error, ErrorCode::Database, "MetaDataError", "Result is null");
			return nullptr;
		}

		const ResultSetMetaData result_set_meta_data(*results);
		auto meta_data = new CResultSetMetaData(result_set_meta_data);
		LOG_DEBUG("Metadata created successfully: columns count = {}", meta_data->columnCount);
		return meta_data;
	} catch (const std::exception& e) {
		set_error(error, ErrorCode::Standard, "MetaDataError", e.what());
		LOG_ERROR_W(L"Exception in get_meta_data: {}", utils::to_wstring(e.what()));
	} catch (...) {
		set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown get meta data error");
		LOG_ERROR("Unknown exception in get_meta_data");
	}
	return nullptr;
}

void delete_meta_data(CResultSetMetaData* meta_data) {
	LOG_DEBUG("Deleting metadata: {}", reinterpret_cast<uintptr_t>(meta_data));
	if (meta_data) {
		delete meta_data;
		LOG_DEBUG("Metadata deleted successfully");
		return;
	}	
	LOG_ERROR("Attempted to delete null metadata");
}