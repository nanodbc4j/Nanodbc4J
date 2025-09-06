#include "api/database_metadata.h"
#include "utils/logger.hpp"

using namespace utils;

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
		LOG_ERROR_W(L"Exception in get_database_meta_data: {}", to_wstring(e.what()));
	} catch (...) {
		set_error(error, -1, "UnknownError", "Unknown get meta data error");
		LOG_ERROR("Unknown exception in get_database_meta_data");
	}
	return nullptr;
}

nanodbc::result* get_database_meta_data_tables(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, const char16_t* type, NativeError* error) {
	LOG_DEBUG("Getting database tables from connection: {}", reinterpret_cast<uintptr_t>(conn));
	init_error(error);
	try {
		if (!conn) {
			LOG_ERROR("Connection pointer is null, cannot get tables");
			set_error(error, 2, "ResultSetError", "Result is null");
			return nullptr;
		}
		const DatabaseMetaData database_meta_data(*conn);
		auto result = database_meta_data.getTables(
			to_wstring(catalog),
			to_wstring(schema),
			to_wstring(table),
			to_wstring(type)
		);
		auto result_ptr = new nanodbc::result(result);
		LOG_DEBUG("ResultSet created successfully: {}", reinterpret_cast<uintptr_t>(result_ptr));
		return result_ptr;
	} catch (const nanodbc::database_error& e) {
        set_error(error, 2, "MetaDataError", e.what());
        LOG_ERROR_W(L"Database error: {}", to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, 2, "MetaDataError", e.what());
        LOG_ERROR_W(L"Database error: {}", to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception");
    }
    return nullptr;
}

nanodbc::result* get_database_meta_data_columns(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, const char16_t* column, NativeError* error) {
	LOG_DEBUG("Getting database columns from connection: {}", reinterpret_cast<uintptr_t>(conn));
	init_error(error);
	try {
		if (!conn) {
			LOG_ERROR("Connection pointer is null, cannot get columns");
			set_error(error, 2, "ResultSetError", "Result is null");
			return nullptr;
		}
		const DatabaseMetaData database_meta_data(*conn);
		auto result = database_meta_data.getColumns(
			to_wstring(catalog),
			to_wstring(schema),
			to_wstring(table),
			to_wstring(column)
		);
		auto result_ptr = new nanodbc::result(result);
		LOG_DEBUG("ResultSet created successfully: {}", reinterpret_cast<uintptr_t>(result_ptr));
		return result_ptr;
	} catch (const nanodbc::database_error& e) {
		set_error(error, 2, "MetaDataError", e.what());
		LOG_ERROR_W(L"Database error: {}", to_wstring(e.what()));
	} catch (const std::exception& e) {
		set_error(error, 2, "MetaDataError", e.what());
		LOG_ERROR_W(L"Database error: {}", to_wstring(e.what()));
	} catch (...) {
		set_error(error, -1, "UnknownError", "Unknown error");
		LOG_ERROR("Unknown exception");
	}
	return nullptr;
}

nanodbc::result* get_database_meta_data_primary_keys(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, NativeError* error) {
	LOG_DEBUG("Getting database columns from connection: {}", reinterpret_cast<uintptr_t>(conn));
	init_error(error);
	try {
		if (!conn) {
			LOG_ERROR("Connection pointer is null, cannot get columns");
			set_error(error, 2, "ResultSetError", "Result is null");
			return nullptr;
		}
		const DatabaseMetaData database_meta_data(*conn);
		auto result = database_meta_data.getPrimaryKeys(
			to_wstring(catalog),
			to_wstring(schema),
			to_wstring(table)
		);
		auto result_ptr = new nanodbc::result(result);
		LOG_DEBUG("ResultSet created successfully: {}", reinterpret_cast<uintptr_t>(result_ptr));
		return result_ptr;
	} catch (const nanodbc::database_error& e) {
		set_error(error, 2, "MetaDataError", e.what());
		LOG_ERROR_W(L"Database error: {}", to_wstring(e.what()));
	} catch (const std::exception& e) {
		set_error(error, 2, "MetaDataError", e.what());
		LOG_ERROR_W(L"Database error: {}", to_wstring(e.what()));
	} catch (...) {
		set_error(error, -1, "UnknownError", "Unknown error");
		LOG_ERROR("Unknown exception");
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