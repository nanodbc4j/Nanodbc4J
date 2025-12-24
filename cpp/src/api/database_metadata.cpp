#include "api/database_metadata.h"

#include "utils/string_proxy.hpp"
#include "utils/logger.hpp"

using namespace utils;
using namespace std;

template<typename Func>
static ResultSet* execute_metadata_query(nanodbc::connection *conn, Func &&func, NativeError *error,
                                               const string &operation_name) noexcept {
    LOG_DEBUG("Executing metadata query '{}' on connection: {}", operation_name, reinterpret_cast<uintptr_t>(conn));
    init_error(error);

    try {
        if (!conn) {
            LOG_ERROR("Connection pointer is null, cannot execute '{}'", operation_name);
            set_error(error, "Result is null");
            return nullptr;
        }

        const DatabaseMetaData database_meta_data(*conn);
        auto result = func(database_meta_data); // Call the required method
        auto result_ptr = new ResultSet(move(result));

        LOG_DEBUG("ResultSet for '{}' created successfully: {}", operation_name, reinterpret_cast<uintptr_t>(result_ptr));
        return result_ptr;
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Exception in '{}': {}", operation_name, StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown error");
        LOG_ERROR("Unknown exception in '{}'", operation_name);
    }

    return nullptr;
}

CDatabaseMetaData *get_database_meta_data(nanodbc::connection *conn, NativeError *error) noexcept {
    LOG_DEBUG("Getting metadata from connection: {}", reinterpret_cast<uintptr_t>(conn));
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection pointer is null, cannot get metadata");
            set_error(error, "Result is null");
            return nullptr;
        }

        const DatabaseMetaData database_meta_data(*conn);
        auto meta_data = new CDatabaseMetaData(database_meta_data);
        LOG_DEBUG("Metadata created successfully: {}", reinterpret_cast<uintptr_t>(meta_data));
        return meta_data;
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Exception in get_database_meta_data: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown get meta data error");
        LOG_ERROR("Unknown exception in get_database_meta_data");
    }
    return nullptr;
}

bool database_meta_data_support_convert(nanodbc::connection *conn, int from_type, int to_type,
                                        NativeError *error) noexcept {
    LOG_TRACE("Support convert fromType: {}, toType: {}", from_type, to_type);
    init_error(error);
    try {
        if (!conn) {
            LOG_ERROR("Connection pointer is null");
            set_error(error, "Result false");
            return false;
        }

        const DatabaseMetaData database_meta_data(*conn);
        bool result = database_meta_data.supportsConvert(from_type, to_type);
        LOG_TRACE("Result: {}", result);
        return result;
    } catch (const exception &e) {
        set_error(error, e.what());
        LOG_ERROR("Exception in database_meta_data_support_convert: {}", StringProxy(e.what()));
    } catch (...) {
        set_error(error, "Unknown get meta data error");
        LOG_ERROR("Unknown exception in database_meta_data_support_convert");
    }
    return false;
}

ResultSet* get_database_meta_data_schemas(nanodbc::connection *conn, NativeError *error) noexcept {
    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getSchemas();
    }, error, "getSchemas");
}

ResultSet* get_database_meta_data_catalogs(nanodbc::connection *conn, NativeError *error) noexcept {
    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getCatalogs();
    }, error, "getCatalogs");
}

ResultSet* get_database_meta_data_table_types(nanodbc::connection *conn, NativeError *error) noexcept {
    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getTableTypes();
    }, error, "getTableTypes");
}

ResultSet* get_database_meta_data_tables(nanodbc::connection *conn, const ApiChar *catalog, const ApiChar *schema,
                                               const ApiChar *table, const ApiChar *type, NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_table(table);
    const StringProxy str_type(type);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getTables(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_table),
            static_cast<nanodbc::string>(str_type));
    }, error, "getTables");
}

ResultSet* get_database_meta_data_columns(nanodbc::connection *conn, const ApiChar *catalog,
                                                const ApiChar *schema,
                                                const ApiChar *table, const ApiChar *column,
                                                NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_table(table);
    const StringProxy str_column(column);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getColumns(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_table),
            static_cast<nanodbc::string>(str_column));
    }, error, "getColumns");
}

ResultSet* get_database_meta_data_primary_keys(nanodbc::connection *conn, const ApiChar *catalog,
                                                     const ApiChar *schema,
                                                     const ApiChar *table, NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_table(table);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getPrimaryKeys(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_table));
    }, error, "getPrimaryKeys");
}

ResultSet* get_database_meta_data_imported_keys(nanodbc::connection *conn, const ApiChar *catalog,
                                                      const ApiChar *schema,
                                                      const ApiChar *table, NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_table(table);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getImportedKeys(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_table));
    }, error, "getImportedKeys");
}

ResultSet* get_database_meta_data_exported_keys(nanodbc::connection *conn, const ApiChar *catalog,
                                                      const ApiChar *schema,
                                                      const ApiChar *table, NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_table(table);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getExportedKeys(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_table));
    }, error, "getExportedKeys");
}

ResultSet* get_database_meta_data_type_info(nanodbc::connection *conn, NativeError *error) noexcept {
    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getTypeInfo();
    }, error, "getTypeInfo");
}

ResultSet* get_database_meta_data_procedures(nanodbc::connection *conn, const ApiChar *catalog,
                                                   const ApiChar *schema,
                                                   const ApiChar *procedure, NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_procedure(procedure);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getProcedures(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_procedure));
    }, error, "getProcedures");
}

ResultSet* get_database_meta_data_procedure_columns(nanodbc::connection *conn, const ApiChar *catalog,
                                                          const ApiChar *schema,
                                                          const ApiChar *procedure, const ApiChar *column,
                                                          NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_procedure(procedure);
    const StringProxy str_column(column);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getProcedureColumns(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_procedure),
            static_cast<nanodbc::string>(str_column));
    }, error, "getProcedureColumns");
}

ResultSet* get_database_meta_data_column_privileges(nanodbc::connection *conn, const ApiChar *catalog,
                                                          const ApiChar *schema,
                                                          const ApiChar *table, const ApiChar *columnNamePattern,
                                                          NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_table(table);
    const StringProxy str_column(columnNamePattern);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getColumnPrivileges(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_table),
            static_cast<nanodbc::string>(str_column));
    }, error, "getColumnPrivileges");
}

ResultSet* get_database_meta_data_table_privileges(nanodbc::connection *conn, const ApiChar *catalog,
                                                         const ApiChar *schema_pattern,
                                                         const ApiChar *table_name_pattern,
                                                         NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema_pattern);
    const StringProxy str_table(table_name_pattern);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getTablePrivileges(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_table));
    }, error, "getTablePrivileges");
}

ResultSet* get_database_meta_data_best_row_identifier(nanodbc::connection *conn, const ApiChar *catalog,
                                                            const ApiChar *schema,
                                                            const ApiChar *table, int scope, bool nullable,
                                                            NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_table(table);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getBestRowIdentifier(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_table), scope, nullable);
    }, error, "getBestRowIdentifier");
}

ResultSet* get_database_meta_data_version_columns(nanodbc::connection *conn, const ApiChar *catalog,
                                                        const ApiChar *schema,
                                                        const ApiChar *table, NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_table(table);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getVersionColumns(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_table));
    }, error, "getVersionColumns");
}

ResultSet* get_database_meta_data_cross_reference(nanodbc::connection *conn, const ApiChar *parent_catalog,
                                                        const ApiChar *parent_schema,
                                                        const ApiChar *parent_table, const ApiChar *foreign_catalog,
                                                        const ApiChar *foreign_schema, const ApiChar *foreign_table,
                                                        NativeError *error) noexcept {
    const StringProxy str_parent_catalog(parent_catalog);
    const StringProxy str_parent_schema(parent_schema);
    const StringProxy str_parent_table(parent_table);
    const StringProxy str_parent_foreign_catalog(foreign_catalog);
    const StringProxy str_parent_foreign_schema(foreign_schema);
    const StringProxy str_parent_foreign_table(foreign_table);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getCrossReference(
            static_cast<nanodbc::string>(str_parent_catalog),
            static_cast<nanodbc::string>(str_parent_schema),
            static_cast<nanodbc::string>(str_parent_table),
            static_cast<nanodbc::string>(str_parent_foreign_catalog),
            static_cast<nanodbc::string>(str_parent_foreign_schema),
            static_cast<nanodbc::string>(str_parent_foreign_table));
    }, error, "getCrossReference");
}

ResultSet* get_database_meta_data_index_info(nanodbc::connection *conn, const ApiChar *catalog,
                                                   const ApiChar *schema,
                                                   const ApiChar *table, bool unique, bool approximate,
                                                   NativeError *error) noexcept {
    const StringProxy str_catalog(catalog);
    const StringProxy str_schema(schema);
    const StringProxy str_table(table);

    return execute_metadata_query(conn, [=](const DatabaseMetaData &meta) {
        return meta.getIndexInfo(
            static_cast<nanodbc::string>(str_catalog),
            static_cast<nanodbc::string>(str_schema),
            static_cast<nanodbc::string>(str_table),
            unique,
            approximate);
    }, error, "getIndexInfo");
}

void delete_database_meta_data(CDatabaseMetaData *meta_data) noexcept {
    LOG_DEBUG("Deleting metadata: {}", reinterpret_cast<uintptr_t>(meta_data));
    if (meta_data) {
        delete meta_data;
        LOG_DEBUG("Metadata deleted successfully");
        return;
    }
    LOG_ERROR("Attempted to delete null metadata");
}
