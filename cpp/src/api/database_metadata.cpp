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
            set_error(error, ErrorCode::Database, "ResultSetError", "Result is null");
            return nullptr;
        }

        const DatabaseMetaData database_meta_data(*conn);
        auto result = func(database_meta_data); // Вызываем нужный метод
        auto result_ptr = new nanodbc::result(std::move(result));

        LOG_DEBUG("ResultSet for '{}' created successfully: {}", operation_name, reinterpret_cast<uintptr_t>(result_ptr));
        return result_ptr;

    } catch (const nanodbc::database_error& e) {
        set_error(error, ErrorCode::Database, "MetaDataError", e.what());
        LOG_ERROR_W(L"Database error in '{}': {}", to_wstring(operation_name), to_wstring(e.what()));
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "MetaDataError", e.what());
        LOG_ERROR_W(L"Exception in '{}': {}", to_wstring(operation_name), to_wstring(e.what()));
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
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
			set_error(error, ErrorCode::Database, "DatabaseMetaData", "Result is null");
			return nullptr;
		}

		const DatabaseMetaData database_meta_data(*conn);
		auto meta_data = new CDatabaseMetaData(database_meta_data);
		LOG_DEBUG("Metadata created successfully: {}", reinterpret_cast<uintptr_t>(meta_data));
		return meta_data;
	} catch (const std::exception& e) {
		set_error(error, ErrorCode::Standard, "DatabaseMetaData", e.what());
		LOG_ERROR_W(L"Exception in get_database_meta_data: {}", to_wstring(e.what()));
	} catch (...) {
		set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown get meta data error");
		LOG_ERROR("Unknown exception in get_database_meta_data");
	}
	return nullptr;
}

bool database_meta_data_support_convert (nanodbc::connection* conn, int from_type, int to_type, NativeError* error) {
	LOG_TRACE("Support convert fromType: {}, toType: {}", from_type, to_type);
	init_error(error);
	try {
		if (!conn) {
			LOG_ERROR("Connection pointer is null");
			set_error(error, ErrorCode::Database, "DatabaseMetaData", "Result false");
			return false;
		}

		const DatabaseMetaData database_meta_data(*conn);
		bool result = database_meta_data.supportsConvert(from_type, to_type);
		LOG_TRACE("Result: {}", result);
		return result;
	} catch (const std::exception& e) {
		set_error(error, ErrorCode::Standard, "DatabaseMetaData", e.what());
		LOG_ERROR_W(L"Exception in database_meta_data_support_convert: {}", to_wstring(e.what()));
	} catch (...) {
		set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown get meta data error");
		LOG_ERROR("Unknown exception in database_meta_data_support_convert");
	}
	return false;
}

nanodbc::result* get_database_meta_data_schemas(nanodbc::connection* conn, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta){
		return meta.getSchemas();
	}, error, "getSchemas");
}

nanodbc::result* get_database_meta_data_catalogs(nanodbc::connection* conn, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getCatalogs();
	}, error, "getCatalogs");
	
}

nanodbc::result* get_database_meta_data_table_types(nanodbc::connection* conn, NativeError* error) {
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

nanodbc::result* get_database_meta_data_column_privileges(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, const char16_t* columnNamePattern, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getColumnPrivileges(to_wstring(catalog), to_wstring(schema), to_wstring(table), to_wstring(columnNamePattern));
	}, error, "getColumnPrivileges");
}

nanodbc::result* get_database_meta_data_table_privileges(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema_pattern,
	const char16_t* table_name_pattern, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTablePrivileges(to_wstring(catalog), to_wstring(schema_pattern), to_wstring(table_name_pattern));
	}, error, "getTablePrivileges");
}

nanodbc::result* get_database_meta_data_best_row_identifier(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, int scope, bool nullable, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getBestRowIdentifier(to_wstring(catalog), to_wstring(schema), to_wstring(table), scope, nullable);
	}, error, "getBestRowIdentifier");
}

nanodbc::result* get_database_meta_data_version_columns(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getVersionColumns(to_wstring(catalog), to_wstring(schema), to_wstring(table));
	}, error, "getVersionColumns");
}

nanodbc::result* get_database_meta_data_cross_reference(nanodbc::connection* conn, const char16_t* parent_catalog, const char16_t* parent_schema,
	const char16_t* parent_table, const char16_t* foreign_catalog, const char16_t* foreign_schema, const char16_t* foreign_table, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getCrossReference(to_wstring(parent_catalog), to_wstring(parent_schema), to_wstring(parent_table), 
			to_wstring(foreign_catalog), to_wstring(foreign_schema), to_wstring(foreign_table));
	}, error, "getCrossReference");
}

nanodbc::result* get_database_meta_data_index_info(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
	const char16_t* table, bool unique, bool approximate, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getIndexInfo(to_wstring(catalog), to_wstring(schema), to_wstring(table), unique, approximate);
	}, error, "getIndexInfo");
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