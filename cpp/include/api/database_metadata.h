#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/database_metadata_c.h"
#include "struct/error_info.h"
#include "api/export.h"

#ifdef __cplusplus
extern "C" {
#endif

	ODBC_API CDatabaseMetaData* get_database_meta_data(nanodbc::connection* conn, NativeError* error);

	ODBC_API bool database_meta_data_support_convert(nanodbc::connection* conn, int from_type, int to_type, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_schemas(nanodbc::connection* conn, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_catalogs(nanodbc::connection* conn, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_table_types(nanodbc::connection* conn, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_tables(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
		const char16_t* table, const char16_t* type, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_columns(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
		const char16_t* table, const char16_t* column, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_primary_keys(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
		const char16_t* table, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_imported_keys(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
		const char16_t* table, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_exported_keys(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
		const char16_t* table, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_type_info(nanodbc::connection* conn, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_procedures(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
		const char16_t* procedure, NativeError* error);

	ODBC_API nanodbc::result* get_database_meta_data_procedure_columns(nanodbc::connection* conn, const char16_t* catalog, const char16_t* schema,
		const char16_t* procedure, const char16_t* column, NativeError* error);

	ODBC_API void delete_database_meta_data(CDatabaseMetaData* meta_data);

#ifdef __cplusplus
} // extern "C"
#endif
