#include "api/database_metadata.h"
#include "utils/logger.hpp"

using namespace utils;
using namespace std;

template<typename Func>
static nanodbc::result* execute_metadata_query(nanodbc::connection* conn, Func&& func, NativeError* error, const string& operation_name) noexcept {
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
        auto result_ptr = new nanodbc::result(move(result));

        LOG_DEBUG("ResultSet for '{}' created successfully: {}", operation_name, reinterpret_cast<uintptr_t>(result_ptr));
        return result_ptr;

    } catch (const nanodbc::database_error& e) {
        set_error(error, ErrorCode::Database, "MetaDataError", e.what());
        LOG_ERROR("Database error in '{}': {}", operation_name, e.what());
    } catch (const exception& e) {
        set_error(error, ErrorCode::Standard, "MetaDataError", e.what());
        LOG_ERROR("Exception in '{}': {}", operation_name, e.what());
    } catch (...) {
        set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown error");
        LOG_ERROR("Unknown exception in '{}'", operation_name);
    }

    return nullptr;
}

CDatabaseMetaData* get_database_meta_data(nanodbc::connection* conn, NativeError* error) noexcept {
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
	} catch (const exception& e) {
		set_error(error, ErrorCode::Standard, "DatabaseMetaData", e.what());
		LOG_ERROR("Exception in get_database_meta_data: {}", e.what());
	} catch (...) {
		set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown get meta data error");
		LOG_ERROR("Unknown exception in get_database_meta_data");
	}
	return nullptr;
}

bool database_meta_data_support_convert (nanodbc::connection* conn, int from_type, int to_type, NativeError* error) noexcept {
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
	} catch (const exception& e) {
		set_error(error, ErrorCode::Standard, "DatabaseMetaData", e.what());
		LOG_ERROR("Exception in database_meta_data_support_convert: {}", e.what());
	} catch (...) {
		set_error(error, ErrorCode::Unknown, "UnknownError", "Unknown get meta data error");
		LOG_ERROR("Unknown exception in database_meta_data_support_convert");
	}
	return false;
}

nanodbc::result* get_database_meta_data_schemas(nanodbc::connection* conn, NativeError* error) noexcept {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta){
		return meta.getSchemas();
	}, error, "getSchemas");
}

nanodbc::result* get_database_meta_data_catalogs(nanodbc::connection* conn, NativeError* error) noexcept {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getCatalogs();
	}, error, "getCatalogs");
	
}

nanodbc::result* get_database_meta_data_table_types(nanodbc::connection* conn, NativeError* error) noexcept {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTableTypes();
	}, error, "getTableTypes");
}

nanodbc::result* get_database_meta_data_tables(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* table, const wchar_t* type, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema ? wstring(schema) : wstring();
	auto str_table = table ? wstring(table) : wstring();
	auto str_type = type ? wstring(type) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTables(str_catalog, str_schema, str_table, str_type);
	}, error, "getTables");
}

nanodbc::result* get_database_meta_data_columns(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* table, const wchar_t* column, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema ? wstring(schema) : wstring();
	auto str_table = table ? wstring(table) : wstring();
	auto str_column = column ? wstring(column) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getColumns(str_catalog, str_schema, str_table, str_column);
	}, error, "getColumns");
}

nanodbc::result* get_database_meta_data_primary_keys(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* table, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema ? wstring(schema) : wstring();
	auto str_table = table ? wstring(table) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getPrimaryKeys(str_catalog, str_schema, str_table);
	}, error, "getPrimaryKeys");
}

nanodbc::result* get_database_meta_data_imported_keys(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* table, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema ? wstring(schema) : wstring();
	auto str_table = table ? wstring(table) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getImportedKeys(str_catalog, str_schema, str_table);
	}, error, "getImportedKeys");
}

nanodbc::result* get_database_meta_data_exported_keys(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* table, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema ? wstring(schema) : wstring();
	auto str_table = table ? wstring(table) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getExportedKeys(str_catalog, str_schema, str_table);
	}, error, "getExportedKeys");
}

nanodbc::result* get_database_meta_data_type_info(nanodbc::connection* conn, NativeError* error) noexcept {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTypeInfo();
	}, error, "getTypeInfo");
}

nanodbc::result* get_database_meta_data_procedures(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* procedure, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema ? wstring(schema) : wstring();
	auto str_procedure = procedure ? wstring(procedure) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getProcedures(str_catalog, str_schema, str_procedure);
	}, error, "getProcedures");
}

nanodbc::result* get_database_meta_data_procedure_columns(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* procedure, const wchar_t* column, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema ? wstring(schema) : wstring();
	auto str_procedure = procedure ? wstring(procedure) : wstring();
	auto str_column = column ? wstring(column) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getProcedureColumns(str_catalog, str_schema, str_procedure, str_column);
	}, error, "getProcedureColumns");
}

nanodbc::result* get_database_meta_data_column_privileges(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* table, const wchar_t* columnNamePattern, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema ? wstring(schema) : wstring();
	auto str_table = table ? wstring(table) : wstring();
	auto str_column = columnNamePattern ? wstring(columnNamePattern) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getColumnPrivileges(str_catalog, str_schema, str_table, str_column);
	}, error, "getColumnPrivileges");
}

nanodbc::result* get_database_meta_data_table_privileges(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema_pattern,
		const wchar_t* table_name_pattern, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema_pattern ? wstring(schema_pattern) : wstring();
	auto str_table = table_name_pattern ? wstring(table_name_pattern) : wstring();
	
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getTablePrivileges(str_catalog, str_schema, str_table);
	}, error, "getTablePrivileges");
}

nanodbc::result* get_database_meta_data_best_row_identifier(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* table, int scope, bool nullable, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema ? wstring(schema) : wstring();
	auto str_table = table ? wstring(table) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getBestRowIdentifier(str_catalog, str_schema, str_table, scope, nullable);
	}, error, "getBestRowIdentifier");
}

nanodbc::result* get_database_meta_data_version_columns(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* table, NativeError* error) noexcept {
	auto str_catalog = catalog ? wstring(catalog) : wstring();
	auto str_schema = schema ? wstring(schema) : wstring();
	auto str_table = table ? wstring(table) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getVersionColumns(str_catalog, str_schema, str_table);
	}, error, "getVersionColumns");
}

nanodbc::result* get_database_meta_data_cross_reference(nanodbc::connection* conn, const wchar_t* parent_catalog, const wchar_t* parent_schema,
		const wchar_t* parent_table, const wchar_t* foreign_catalog, const wchar_t* foreign_schema, const wchar_t* foreign_table, NativeError* error) noexcept {
	auto str_parent_catalog = parent_catalog ? wstring(parent_catalog) : wstring();
	auto str_parent_schema = parent_schema ? wstring(parent_schema) : wstring();
	auto str_parent_table = parent_table ? wstring(parent_table) : wstring();
	auto str_parent_foreign_catalog = foreign_catalog ? wstring(foreign_catalog) : wstring();
	auto str_parent_foreign_schema = foreign_schema ? wstring(foreign_schema) : wstring();
	auto str_parent_foreign_table = foreign_table ? wstring(foreign_table) : wstring();

	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getCrossReference(str_parent_catalog, str_parent_schema, str_parent_table,
			str_parent_foreign_catalog, str_parent_foreign_schema, str_parent_foreign_table);
	}, error, "getCrossReference");
}

nanodbc::result* get_database_meta_data_index_info(nanodbc::connection* conn, const wchar_t* catalog, const wchar_t* schema,
		const wchar_t* table, bool unique, bool approximate, NativeError* error) noexcept {
	return execute_metadata_query(conn, [=](const DatabaseMetaData& meta) {
		return meta.getIndexInfo(catalog, schema, table, unique, approximate);
	}, error, "getIndexInfo");
}

void delete_database_meta_data(CDatabaseMetaData* meta_data) noexcept {
	LOG_DEBUG("Deleting metadata: {}", reinterpret_cast<uintptr_t>(meta_data));
	if (meta_data) {
		delete meta_data;
		LOG_DEBUG("Metadata deleted successfully");
		return;
	}
	LOG_ERROR("Attempted to delete null metadata");
}