package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Library;
import com.sun.jna.Native;
import io.github.nanodbc4j.internal.cstruct.DatabaseMetaDataStruct;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.pointer.ConnectionPtr;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;

/**
 * JNA interface for ODBC database metadata operations.
 * Maps to native ODBC metadata API functions.
 */
public interface DatabaseMetaDataApi extends Library {
    DatabaseMetaDataApi INSTANCE = initialize();

    private static DatabaseMetaDataApi initialize() {
        try {
            String lib = LibraryLoader.load();
            return Native.load(lib, DatabaseMetaDataApi.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    /**
     * Gets database metadata information structure.
     *
     * @param conn connection pointer
     * @param error error information output
     * @return database metadata structure
     */
    DatabaseMetaDataStruct get_database_meta_data(ConnectionPtr conn, NativeError error);

    /**
     * Checks if data type conversion is supported.
     *
     * @param conn connection pointer
     * @param from_type source data type
     * @param to_type target data type
     * @param error error information output
     * @return 1 if conversion is supported
     */
    byte database_meta_data_support_convert(ConnectionPtr conn, int from_type, int to_type, NativeError error);

    /**
     * Gets available schemas.
     *
     * @param conn connection pointer
     * @param error error information output
     * @return result set with schema information
     */
    ResultSetPtr get_database_meta_data_schemas(ConnectionPtr conn, NativeError error);

    /**
     * Gets available catalogs.
     *
     * @param conn connection pointer
     * @param error error information output
     * @return result set with catalog information
     */
    ResultSetPtr get_database_meta_data_catalogs(ConnectionPtr conn, NativeError error);

    /**
     * Gets available table types.
     *
     * @param conn connection pointer
     * @param error error information output
     * @return result set with table type information
     */
    ResultSetPtr get_database_meta_data_table_types(ConnectionPtr conn, NativeError error);

    /**
     * Gets tables matching specified criteria.
     *
     * @param conn connection pointer
     * @param catalog catalog name pattern
     * @param schema schema name pattern
     * @param table table name pattern
     * @param type table type pattern
     * @param error error information output
     * @return result set with table information
     */
    ResultSetPtr get_database_meta_data_tables(ConnectionPtr conn, String catalog, String schema,
                                               String table, String type, NativeError error);

    /**
     * Gets columns matching specified criteria.
     *
     * @param conn connection pointer
     * @param catalog catalog name pattern
     * @param schema schema name pattern
     * @param table table name pattern
     * @param column column name pattern
     * @param error error information output
     * @return result set with column information
     */
    ResultSetPtr get_database_meta_data_columns(ConnectionPtr conn, String catalog, String schema,
                                                String table, String column, NativeError error);

    /**
     * Gets primary key information for table.
     *
     * @param conn connection pointer
     * @param catalog catalog name
     * @param schema schema name
     * @param table table name
     * @param error error information output
     * @return result set with primary key information
     */
    ResultSetPtr get_database_meta_data_primary_keys(ConnectionPtr conn, String catalog, String schema,
                                                     String table, NativeError error);

    /**
     * Gets imported (foreign) keys for table.
     *
     * @param conn connection pointer
     * @param catalog catalog name
     * @param schema schema name
     * @param table table name
     * @param error error information output
     * @return result set with imported key information
     */
    ResultSetPtr get_database_meta_data_imported_keys(ConnectionPtr conn, String catalog, String schema,
                                                      String table, NativeError error);

    /**
     * Gets exported keys for table.
     *
     * @param conn connection pointer
     * @param catalog catalog name
     * @param schema schema name
     * @param table table name
     * @param error error information output
     * @return result set with exported key information
     */
    ResultSetPtr get_database_meta_data_exported_keys(ConnectionPtr conn, String catalog, String schema,
                                                      String table, NativeError error);

    /**
     * Gets supported data type information.
     *
     * @param conn connection pointer
     * @param error error information output
     * @return result set with data type information
     */
    ResultSetPtr get_database_meta_data_type_info(ConnectionPtr conn, NativeError error);

    /**
     * Gets procedures matching specified criteria.
     *
     * @param conn connection pointer
     * @param catalog catalog name pattern
     * @param schema schema name pattern
     * @param procedure procedure name pattern
     * @param error error information output
     * @return result set with procedure information
     */
    ResultSetPtr get_database_meta_data_procedures(ConnectionPtr conn, String catalog, String schema,
                                                   String procedure, NativeError error);

    /**
     * Gets procedure columns matching specified criteria.
     *
     * @param conn connection pointer
     * @param catalog catalog name pattern
     * @param schema schema name pattern
     * @param procedure procedure name pattern
     * @param column column name pattern
     * @param error error information output
     * @return result set with procedure column information
     */
    ResultSetPtr get_database_meta_data_procedure_columns(ConnectionPtr conn, String catalog, String schema,
                                                          String procedure, String column, NativeError error);

    /**
     * Gets column privileges for table.
     *
     * @param conn connection pointer
     * @param catalog catalog name
     * @param schema schema name
     * @param table table name
     * @param column_name_pattern column name pattern
     * @param error error information output
     * @return result set with column privilege information
     */
    ResultSetPtr get_database_meta_data_column_privileges(ConnectionPtr conn, String catalog, String schema,
                                                          String table, String column_name_pattern, NativeError error);

    /**
     * Gets table privileges.
     *
     * @param conn connection pointer
     * @param catalog catalog name pattern
     * @param schema_pattern schema name pattern
     * @param table_name_pattern table name pattern
     * @param error error information output
     * @return result set with table privilege information
     */
    ResultSetPtr get_database_meta_data_table_privileges(ConnectionPtr conn, String catalog, String schema_pattern,
                                                         String table_name_pattern, NativeError error);

    /**
     * Gets best row identifier for table.
     *
     * @param conn connection pointer
     * @param catalog catalog name
     * @param schema schema name
     * @param table table name
     * @param scope scope of interest
     * @param nullable include nullable columns
     * @param error error information output
     * @return result set with best row identifier information
     */
    ResultSetPtr get_database_meta_data_best_row_identifier(ConnectionPtr conn, String catalog, String schema,
                                                            String table, int scope, byte nullable, NativeError error);

    /**
     * Gets version columns for table.
     *
     * @param conn connection pointer
     * @param catalog catalog name
     * @param schema schema name
     * @param table table name
     * @param error error information output
     * @return result set with version column information
     */
    ResultSetPtr get_database_meta_data_version_columns(ConnectionPtr conn, String catalog, String schema,
                                                        String table, NativeError error);

    /**
     * Gets cross-reference between parent and foreign tables.
     *
     * @param conn connection pointer
     * @param parent_catalog parent table catalog
     * @param parent_schema parent table schema
     * @param parent_table parent table name
     * @param foreign_catalog foreign table catalog
     * @param foreign_schema foreign table schema
     * @param foreign_table foreign table name
     * @param error error information output
     * @return result set with cross-reference information
     */
    ResultSetPtr get_database_meta_data_cross_reference(ConnectionPtr conn, String parent_catalog, String parent_schema,
                                                        String parent_table, String foreign_catalog, String foreign_schema, String foreign_table, NativeError error);

    /**
     * Gets index information for table.
     *
     * @param conn connection pointer
     * @param catalog catalog name
     * @param schema schema name
     * @param table table name
     * @param unique return only unique indexes
     * @param approximate allow approximate results
     * @param error error information output
     * @return result set with index information
     */
    ResultSetPtr get_database_meta_data_index_info(ConnectionPtr conn, String catalog, String schema,
                                                   String table, byte unique, byte approximate, NativeError error);

    /**
     * Delete database metadata structure.
     *
     * @param meta_data metadata structure to delete
     */
    void delete_database_meta_data(DatabaseMetaDataStruct meta_data);
}
