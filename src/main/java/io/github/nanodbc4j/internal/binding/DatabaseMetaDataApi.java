package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Library;
import com.sun.jna.Native;
import io.github.nanodbc4j.internal.cstruct.DatabaseMetaDataStruct;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.pointer.ConnectionPtr;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;

public interface DatabaseMetaDataApi extends Library {
    DatabaseMetaDataApi INSTANCE = initialize();

    private static DatabaseMetaDataApi initialize() {
        try {
            LibraryLoader.load();
            return Native.load(DatabaseMetaDataApi.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    DatabaseMetaDataStruct get_database_meta_data(ConnectionPtr conn, NativeError error);

    byte database_meta_data_support_convert(ConnectionPtr conn, int from_type, int to_type, NativeError error);

    ResultSetPtr get_database_meta_data_schemas(ConnectionPtr conn, NativeError error);

    ResultSetPtr get_database_meta_data_catalogs(ConnectionPtr conn, NativeError error);

    ResultSetPtr get_database_meta_data_table_types(ConnectionPtr conn, NativeError error);

    ResultSetPtr get_database_meta_data_tables(ConnectionPtr conn, String catalog, String schema,
                                               String table, String type, NativeError error);

    ResultSetPtr get_database_meta_data_columns(ConnectionPtr conn, String catalog, String schema,
                                                String table, String column, NativeError error);

    ResultSetPtr get_database_meta_data_primary_keys(ConnectionPtr conn, String catalog, String schema,
                                                     String table, NativeError error);

    ResultSetPtr get_database_meta_data_imported_keys(ConnectionPtr conn, String catalog, String schema,
                                                      String table, NativeError error);

    ResultSetPtr get_database_meta_data_exported_keys(ConnectionPtr conn, String catalog, String schema,
                                                      String table, NativeError error);

    ResultSetPtr get_database_meta_data_type_info(ConnectionPtr conn, NativeError error);

    ResultSetPtr get_database_meta_data_procedures(ConnectionPtr conn, String catalog, String schema,
                                                   String procedure, NativeError error);

    ResultSetPtr get_database_meta_data_procedure_columns(ConnectionPtr conn, String catalog, String schema,
                                                          String procedure, String column, NativeError error);

    ResultSetPtr get_database_meta_data_column_privileges(ConnectionPtr conn, String catalog, String schema,
                                                          String table, String column_name_pattern, NativeError error);

    ResultSetPtr get_database_meta_data_table_privileges(ConnectionPtr conn, String catalog, String schema_pattern,
                                                         String table_name_pattern, NativeError error);

    ResultSetPtr get_database_meta_data_best_row_identifier(ConnectionPtr conn, String catalog, String schema,
                                                            String table, int scope, byte nullable, NativeError error);

    ResultSetPtr get_database_meta_data_version_columns(ConnectionPtr conn, String catalog, String schema,
                                                        String table, NativeError error);

    ResultSetPtr get_database_meta_data_cross_reference(ConnectionPtr conn, String parent_catalog, String parent_schema,
                                                        String parent_table, String foreign_catalog, String foreign_schema, String foreign_table, NativeError error);

    ResultSetPtr get_database_meta_data_index_info(ConnectionPtr conn, String catalog, String schema,
                                                   String table, byte unique, byte approximate, NativeError error);

    void delete_database_meta_data(DatabaseMetaDataStruct meta_data);
}
