#pragma once
#include <nanodbc/nanodbc.h>
#include "struct/database_metadata_c.h"
#include "struct/error_info.h"
#include "api/api.h"

#ifdef __cplusplus
extern "C" {
#endif

	/// \brief Retrieves comprehensive database metadata information.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to database metadata object on success, nullptr on failure.
    ODBC_API CDatabaseMetaData* get_database_meta_data(nanodbc::connection* conn, NativeError* error) noexcept;

    /// \brief Checks if the database supports type conversion between specified types.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param from_type The source data type for conversion.
    /// \param to_type The target data type for conversion.
    /// \param error Error information structure to populate on failure.
    /// \return true if conversion is supported, false otherwise.
    ODBC_API bool database_meta_data_support_convert(nanodbc::connection* conn, int from_type, int to_type, NativeError* error) noexcept;

    /// \brief Retrieves available schemas in the database.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing schema information.
    ODBC_API nanodbc::result* get_database_meta_data_schemas(nanodbc::connection* conn, NativeError* error) noexcept;

    /// \brief Retrieves available catalogs in the database.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing catalog information.
    ODBC_API nanodbc::result* get_database_meta_data_catalogs(nanodbc::connection* conn, NativeError* error) noexcept;

    /// \brief Retrieves available table types in the database.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing table type information.
    ODBC_API nanodbc::result* get_database_meta_data_table_types(nanodbc::connection* conn, NativeError* error) noexcept;

    /// \brief Retrieves tables matching specified criteria.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param table Table name filter (can be null for any).
    /// \param type Table type filter (can be null for any).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing table information.
    ODBC_API nanodbc::result* get_database_meta_data_tables(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
        const ApiChar* table, const ApiChar* type, NativeError* error) noexcept;

    /// \brief Retrieves columns matching specified criteria.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param table Table name filter (can be null for any).
    /// \param column Column name filter (can be null for any).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing column information.
    ODBC_API nanodbc::result* get_database_meta_data_columns(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
        const ApiChar* table, const ApiChar* column, NativeError* error) noexcept;

    /// \brief Retrieves primary key information for specified table.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param table Table name (required).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing primary key information.
    ODBC_API nanodbc::result* get_database_meta_data_primary_keys(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
        const ApiChar* table, NativeError* error) noexcept;

    /// \brief Retrieves imported foreign keys for specified table.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param table Table name (required).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing imported key information.
    ODBC_API nanodbc::result* get_database_meta_data_imported_keys(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
        const ApiChar* table, NativeError* error) noexcept;

    /// \brief Retrieves exported foreign keys for specified table.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param table Table name (required).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing exported key information.
    ODBC_API nanodbc::result* get_database_meta_data_exported_keys(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
        const ApiChar* table, NativeError* error) noexcept;

    /// \brief Retrieves data type information supported by the database.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing data type information.
    ODBC_API nanodbc::result* get_database_meta_data_type_info(nanodbc::connection* conn, NativeError* error) noexcept;

    /// \brief Retrieves procedures matching specified criteria.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param procedure Procedure name filter (can be null for any).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing procedure information.
    ODBC_API nanodbc::result* get_database_meta_data_procedures(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
        const ApiChar* procedure, NativeError* error) noexcept;

    /// \brief Retrieves procedure columns matching specified criteria.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param procedure Procedure name filter (can be null for any).
    /// \param column Column name filter (can be null for any).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing procedure column information.
    ODBC_API nanodbc::result* get_database_meta_data_procedure_columns(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
        const ApiChar* procedure, const ApiChar* column, NativeError* error) noexcept;

    /// \brief Retrieves column privilege information.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param table Table name (required).
    /// \param column_name_pattern Column name pattern filter (can be null for any).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing column privilege information.
    ODBC_API nanodbc::result* get_database_meta_data_column_privileges(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema,
        const ApiChar* table, const ApiChar* column_name_pattern, NativeError* error) noexcept;

    /// \brief Retrieves table privilege information.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema_pattern Schema name pattern filter (can be null for any).
    /// \param table_name_pattern Table name pattern filter (can be null for any).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing table privilege information.
    ODBC_API nanodbc::result* get_database_meta_data_table_privileges(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema_pattern,
        const ApiChar* table_name_pattern, NativeError* error) noexcept;

    /// \brief Retrieves optimal set of columns that uniquely identify a row.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param table Table name (required).
    /// \param scope The scope of interest.
    /// \param nullable Whether to include nullable columns.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing best row identifier information.
    ODBC_API nanodbc::result* get_database_meta_data_best_row_identifier(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema, 
        const ApiChar* table, int scope, bool nullable, NativeError* error) noexcept;

    /// \brief Retrieves columns that are automatically updated when any value in a row is updated.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param table Table name (required).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing version column information.
    ODBC_API nanodbc::result* get_database_meta_data_version_columns(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema, 
        const ApiChar* table, NativeError* error) noexcept;

    /// \brief Retrieves foreign key relationships between two tables.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param parent_catalog Parent table catalog name (can be null for any).
    /// \param parent_schema Parent table schema name (can be null for any).
    /// \param parent_table Parent table name (required).
    /// \param foreign_catalog Foreign table catalog name (can be null for any).
    /// \param foreign_schema Foreign table schema name (can be null for any).
    /// \param foreign_table Foreign table name (required).
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing cross-reference information.
    ODBC_API nanodbc::result* get_database_meta_data_cross_reference(nanodbc::connection* conn, const ApiChar* parent_catalog, const ApiChar* parent_schema,
        const ApiChar* parent_table, const ApiChar* foreign_catalog, const ApiChar* foreign_schema, const ApiChar* foreign_table, NativeError* error) noexcept;

    /// \brief Retrieves index information for specified table.
    /// \param conn Pointer to the nanodbc connection object.
    /// \param catalog Catalog name filter (can be null for any).
    /// \param schema Schema name filter (can be null for any).
    /// \param table Table name (required).
    /// \param unique true to return only unique-valued indexes.
    /// \param approximate true to return approximate values for some numeric columns.
    /// \param error Error information structure to populate on failure.
    /// \return Pointer to result set containing index information.
    ODBC_API nanodbc::result* get_database_meta_data_index_info(nanodbc::connection* conn, const ApiChar* catalog, const ApiChar* schema, 
        const ApiChar* table, bool unique, bool approximate, NativeError* error) noexcept;

    /// \brief Releases database metadata resources.
    /// \param meta_data Pointer to database metadata object to delete.
    ODBC_API void delete_database_meta_data(CDatabaseMetaData* meta_data) noexcept;

#ifdef __cplusplus
} // extern "C"
#endif
