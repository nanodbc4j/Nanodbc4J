#include "api/meta_data.h"
#include "utils/logger.hpp"

MetaData* get_meta_data(nanodbc::result* results, NativeError* error) {
	LOG_DEBUG_W(L"Getting metadata from result: {}", reinterpret_cast<uintptr_t>(results));
	init_error(error);
	try {
		if (!results) {
			LOG_DEBUG_W(L"Result pointer is null, cannot get metadata");
			set_error(error, 2, "MetaDataError", "Result is null");
			return nullptr;
		}

		const ResultSetMetaData result_set_meta_data(*results);
		auto meta_data = new MetaData(result_set_meta_data);
		LOG_DEBUG_W(L"Metadata created successfully: columns count = {}", meta_data->columnCount);
		return meta_data;
	} catch (const std::exception& e) {
		set_error(error, 2, "MetaDataError", e.what());
		LOG_DEBUG_W(L"Exception in get_meta_data: {}", utils::to_wstring(e.what()));
	} catch (...) {
		set_error(error, -1, "UnknownError", "Unknown get meta data error");
		LOG_DEBUG_W(L"Unknown exception in get_meta_data");
	}
	return nullptr;
}

void delete_meta_data(MetaData* meta_data) {
	LOG_DEBUG_W(L"Deleting metadata: {}", reinterpret_cast<uintptr_t>(meta_data));
	if (meta_data) {
		delete meta_data;
		LOG_DEBUG_W(L"Metadata deleted successfully");
		return;
	}	
	LOG_DEBUG_W(L"Attempted to delete null metadata");	
}