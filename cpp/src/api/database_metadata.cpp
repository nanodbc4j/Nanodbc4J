#include "api/database_metadata.h"
#include "utils/logger.hpp"

using namespace utils;

template<typename Func>
static nanodbc::result* execute_metadata_query(nanodbc::connection* conn, Func&& func, NativeError* error, const char* operation_name) {
    LOG_DEBUG("Executing metadata query '{}' on connection: {}", operation_name, reinterpret_cast<uintptr_t>(conn));
    init_error(error);

    try {
        if (!conn) {
            LOG_ERROR("Connection pointer is null, cannot execute '{}'", operation_name);
            set_error(error, 2, "ResultSetError", "Result is null");
            return nullptr;
        }

        const DatabaseMetaData database_meta_data(*conn);
        auto result = func(database_meta_data); // Вызываем нужный метод
        auto result_ptr = new nanodbc::result(result);

        LOG_DEBUG("ResultSet for '{}' created successfully: {}", operation_name, reinterpret_cast<uintptr_t>(result_ptr));
        return result_ptr;

    } catch (const nanodbc::database_error& e) {
        set_error(error, 2, "MetaDataError", e.what());
        LOG_ERROR_W(L"Database error in '{}': {}", to_wstring(operation_name), to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, 2, "MetaDataError", e.what());
        LOG_ERROR_W(L"Exception in '{}': {}", to_wstring(operation_name), to_wstring(e.what()));
    } catch (...) {
        set_error(error, -1, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in '{}'", operation_name);
    }

    return nullptr;
}

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

ODBC_API nanodbc::result* get_database_meta_data_schemas(nanodbc::connection* conn, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta){
		return meta.getSchemas();
	}, error, "getSchemas");
}

ODBC_API nanodbc::result* get_database_meta_data_catalogs(nanodbc::connection* conn, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getCatalogs();
	}, error, "getCatalogs");
	
}

ODBC_API nanodbc::result* get_database_meta_data_table_types(nanodbc::connection* conn, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTableTypes();
	}, error, "getTableTypes");
}

nanodbc::result* get_database_meta_data_tables(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, const char16_t* type, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTables(to_wstring(catalog), to_wstring(schema), to_wstring(table), to_wstring(type));
	}, error, "getTables");
}

nanodbc::result* get_database_meta_data_columns(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, const char16_t* column, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getColumns(to_wstring(catalog), to_wstring(schema), to_wstring(table), to_wstring(column));
	}, error, "getColumns");
}

nanodbc::result* get_database_meta_data_primary_keys(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getPrimaryKeys(to_wstring(catalog), to_wstring(schema), to_wstring(table));
	}, error, "getPrimaryKeys");
}

nanodbc::result* get_database_meta_data_imported_keys(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getImportedKeys(to_wstring(catalog), to_wstring(schema), to_wstring(table));
	}, error, "getImportedKeys");
}

nanodbc::result* get_database_meta_data_exported_keys(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getExportedKeys(to_wstring(catalog), to_wstring(schema), to_wstring(table));
	}, error, "getExportedKeys");
}

nanodbc::result* get_database_meta_data_type_info(nanodbc::connection* conn, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTypeInfo();
	}, error, "getTypeInfo");
}

nanodbc::result* get_database_meta_data_procedures(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* procedure, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getProcedures(to_wstring(catalog), to_wstring(schema), to_wstring(procedure));
	}, error, "getProcedures");
}

nanodbc::result* get_database_meta_data_procedure_columns(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* procedure, const char16_t* column, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getProcedureColumns(to_wstring(catalog), to_wstring(schema), to_wstring(procedure), to_wstring(column));
	}, error, "getProcedureColumns");
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