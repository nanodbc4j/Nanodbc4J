#include "api/database_metadata.h"
#include "utils/logger.hpp"

using namespace utils;

template<typename Func>
static nanodbc::result* execute_metadata_query(nanodbc::connection* conn, Func&& func, NativeError* error, const std::string& operation_name) {
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
        LOG_ERROR("Database error in '{}': {}", operation_name, e.what());
    } catch (const std::exception& e) {
        set_error(error, ErrorCode::Standard, "MetaDataError", e.what());
        LOG_ERROR("Exception in '{}': {}", operation_name, e.what());
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
		LOG_ERROR("Exception in get_database_meta_data: {}", e.what());
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
		LOG_ERROR("Exception in database_meta_data_support_convert: {}", e.what());
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

nanodbc::result* get_database_meta_data_tables(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, const ApiChar* type, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema ? nanodbc::string(schema) : nanodbc::string();
	auto str_table = table ? nanodbc::string(table) : nanodbc::string();
	auto str_type = type ? nanodbc::string(type) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTables(str_catalog, str_schema, str_table, str_type);
	}, error, "getTables");
}

nanodbc::result* get_database_meta_data_columns(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, const ApiChar* column, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema ? nanodbc::string(schema) : nanodbc::string();
	auto str_table = table ? nanodbc::string(table) : nanodbc::string();
	auto str_column = column ? nanodbc::string(column) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getColumns(str_catalog, str_schema, str_table, str_column);
	}, error, "getColumns");
}

nanodbc::result* get_database_meta_data_primary_keys(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema ? nanodbc::string(schema) : nanodbc::string();
	auto str_table = table ? nanodbc::string(table) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getPrimaryKeys(str_catalog, str_schema, str_table);
	}, error, "getPrimaryKeys");
}

nanodbc::result* get_database_meta_data_imported_keys(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema ? nanodbc::string(schema) : nanodbc::string();
	auto str_table = table ? nanodbc::string(table) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getImportedKeys(str_catalog, str_schema, str_table);
	}, error, "getImportedKeys");
}

nanodbc::result* get_database_meta_data_exported_keys(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema ? nanodbc::string(schema) : nanodbc::string();
	auto str_table = table ? nanodbc::string(table) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getExportedKeys(str_catalog, str_schema, str_table);
	}, error, "getExportedKeys");
}

nanodbc::result* get_database_meta_data_type_info(nanodbc::connection* conn, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTypeInfo();
	}, error, "getTypeInfo");
}

nanodbc::result* get_database_meta_data_procedures(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* procedure, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema ? nanodbc::string(schema) : nanodbc::string();
	auto str_procedure = procedure ? nanodbc::string(procedure) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getProcedures(str_catalog, str_schema, str_procedure);
	}, error, "getProcedures");
}

nanodbc::result* get_database_meta_data_procedure_columns(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* procedure, const ApiChar* column, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema ? nanodbc::string(schema) : nanodbc::string();
	auto str_procedure = procedure ? nanodbc::string(procedure) : nanodbc::string();
	auto str_column = column ? nanodbc::string(column) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getProcedureColumns(str_catalog, str_schema, str_procedure, str_column);
	}, error, "getProcedureColumns");
}

nanodbc::result* get_database_meta_data_column_privileges(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, const ApiChar* columnNamePattern, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema ? nanodbc::string(schema) : nanodbc::string();
	auto str_table = table ? nanodbc::string(table) : nanodbc::string();
	auto str_column = columnNamePattern ? nanodbc::string(columnNamePattern) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getColumnPrivileges(str_catalog, str_schema, str_table, str_column);
	}, error, "getColumnPrivileges");
}

nanodbc::result* get_database_meta_data_table_privileges(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema_pattern,
		const ApiChar* table_name_pattern, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema_pattern ? nanodbc::string(schema_pattern) : nanodbc::string();
	auto str_table = table_name_pattern ? nanodbc::string(table_name_pattern) : nanodbc::string();
	
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTablePrivileges(str_catalog, str_schema, str_table);
	}, error, "getTablePrivileges");
}

nanodbc::result* get_database_meta_data_best_row_identifier(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, int scope, bool nullable, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema ? nanodbc::string(schema) : nanodbc::string();
	auto str_table = table ? nanodbc::string(table) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getBestRowIdentifier(str_catalog, str_schema, str_table, scope, nullable);
	}, error, "getBestRowIdentifier");
}

nanodbc::result* get_database_meta_data_version_columns(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, NativeError* error) {
	auto str_catalog = catalog ? nanodbc::string(catalog) : nanodbc::string();
	auto str_schema = schema ? nanodbc::string(schema) : nanodbc::string();
	auto str_table = table ? nanodbc::string(table) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getVersionColumns(str_catalog, str_schema, str_table);
	}, error, "getVersionColumns");
}

nanodbc::result* get_database_meta_data_cross_reference(nanodbc::connection* conn, const ApiChar* parent_catalog, const ApiChar* parent_schema,
		const ApiChar* parent_table, const ApiChar* foreign_catalog, const ApiChar* foreign_schema, const ApiChar* foreign_table, NativeError* error) {
	auto str_parent_catalog = parent_catalog ? nanodbc::string(parent_catalog) : nanodbc::string();
	auto str_parent_schema = parent_schema ? nanodbc::string(parent_schema) : nanodbc::string();
	auto str_parent_table = parent_table ? nanodbc::string(parent_table) : nanodbc::string();
	auto str_parent_foreign_catalog = foreign_catalog ? nanodbc::string(foreign_catalog) : nanodbc::string();
	auto str_parent_foreign_schema = foreign_schema ? nanodbc::string(foreign_schema) : nanodbc::string();
	auto str_parent_foreign_table = foreign_table ? nanodbc::string(foreign_table) : nanodbc::string();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getCrossReference(str_parent_catalog, str_parent_schema, str_parent_table,
			str_parent_foreign_catalog, str_parent_foreign_schema, str_parent_foreign_table);
	}, error, "getCrossReference");
}

nanodbc::result* get_database_meta_data_index_info(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, bool unique, bool approximate, NativeError* error) {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getIndexInfo(catalog, schema, table, unique, approximate);
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