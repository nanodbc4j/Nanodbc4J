#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/database_metadata_c.h"
#include "struct/error_info.h"
#include "api/export.h"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif

	ODBC_API CDatabaseMetaData* get_database_meta_data(nanodbc::connection* conn, NativeError* error);

	ODBC_API bool database_meta_data_support_convert(nanodbc::connection* conn, int from_type, int to_type, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_schemas(nanodbc::connection* conn, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_catalogs(nanodbc::connection* conn, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_table_types(nanodbc::connection* conn, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_tables(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, const ApiChar* type, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_columns(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, const ApiChar* column, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_primary_keys(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_imported_keys(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_exported_keys(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_type_info(nanodbc::connection* conn, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_procedures(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* procedure, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_procedure_columns(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* procedure, const ApiChar* column, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_column_privileges(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
		const ApiChar* table, const ApiChar* column_name_pattern, NativeError* error);
	
	ODBC_API nanodbc::result* get_database_meta_data_table_privileges(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema_pattern,
		const ApiChar* table_name_pattern, NativeError* error);
	
	ODBC_API nanodbc::result* get_database_meta_data_best_row_identifier(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema, 
		const ApiChar* table, int scope, bool nullable, NativeError* error);
	
	ODBC_API nanodbc::result* get_database_meta_data_version_columns(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema, 
		const ApiChar* table, NativeError* error);
	
	ODBC_API nanodbc::result* get_database_meta_data_cross_reference(nanodbc::connection* conn, const ApiChar* parent_catalog, const ApiChar* parent_schema,
		const ApiChar* parent_table, const ApiChar* foreign_catalog, const ApiChar* foreign_schema, const ApiChar* foreign_table, NativeError* error);
	
	ODBC_API nanodbc::result* get_database_meta_data_index_info(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema, 
		const ApiChar* table, bool unique, bool approximate, NativeError* error);

	ODBC_API void delete_database_meta_data(CDatabaseMetaData* meta_data);

#ifdef __cplusplus
} // extern "C"
#endif
