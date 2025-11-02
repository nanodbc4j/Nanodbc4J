package io.github.nanodbc4j.internal;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import io.github.nanodbc4j.internal.pointer.*;
import io.github.nanodbc4j.internal.cstruct.*;

public interface NativeDB extends Library {
    NativeDB INSTANCE = initialize();

    private static NativeDB initialize() {
        try {
            LibraryLoader.load();
            return Native.load(NativeDB.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    /**
     * Метод создаёт соединение с базой данных
     *
     * @param connection_string Массив символов (UTF-16LE) строки подключения
     * @param timeout           Время ожидания соединения
     * @param error             Ошибка, возникающая при соединении
     */
    ConnectionPtr connection_with_timeout(String connection_string, long timeout, NativeError error);

    ConnectionPtr connection_with_user_pass_timeout(String dsn, String user, String pass, long timeout, NativeError error);

    /**
     * Закрывает существующее подключение
     *
     * @param connection Указатель на соединение
     * @param error      Возможная ошибка закрытия
     */
    void disconnect(ConnectionPtr connection, NativeError error);

    /**
     * Проверяет состояние соединения
     *
     * @param conn  Указатель на соединение
     * @param error Информация об ошибке
     * @return True, если соединение активно
     */
    byte is_connected(ConnectionPtr conn, NativeError error);

    Pointer get_catalog_name(ConnectionPtr conn, NativeError error);

    void set_catalog_name(ConnectionPtr conn, String catalog, NativeError error);

    void set_transaction_isolation_level(ConnectionPtr conn, int level, NativeError error);

    int get_transaction_isolation_level(ConnectionPtr conn, NativeError error);

    void set_auto_commit_transaction(ConnectionPtr conn, byte autoCommit, NativeError error);

    void commit_transaction(ConnectionPtr conn, NativeError error);

    void rollback_transaction(ConnectionPtr conn, NativeError error);

    byte get_auto_commit_transaction(ConnectionPtr conn, NativeError error);

    ResultSetPtr execute_request(ConnectionPtr conn, String sql, int timeout, NativeError error);

    StatementPtr create_statement(ConnectionPtr conn, NativeError error);

    void prepare_statement(StatementPtr stmt, String sql, NativeError error);

    void set_int_value(StatementPtr stmt, int index, int value, NativeError error);

    void set_long_value(StatementPtr stmt, int index, long value, NativeError error);

    void set_double_value(StatementPtr stmt, int index, double value, NativeError error);

    void set_bool_value(StatementPtr stmt, int index, byte value, NativeError error);

    void set_float_value(StatementPtr stmt, int index, float value, NativeError error);

    void set_short_value(StatementPtr stmt, int index, short value, NativeError error);

    void set_string_value(StatementPtr stmt, int index, String value, NativeError error);

    void set_date_value(StatementPtr stmt, int index, DateStruct value, NativeError error);

    void set_time_value(StatementPtr stmt, int index, TimeStruct value, NativeError error);

    void set_timestamp_value(StatementPtr stmt, int index, TimestampStruct value, NativeError error);

    void set_binary_array_value(StatementPtr stmt, int index, BinaryArray value, NativeError error);

    ResultSetPtr execute(StatementPtr stmt, int timeout, NativeError error);

    void cancel_statement(StatementPtr stmt, NativeError error);

    byte next_result(ResultSetPtr results, NativeError error);

    byte previous_result(ResultSetPtr results, NativeError error);

    byte first_result(ResultSetPtr results, NativeError error);

    byte last_result(ResultSetPtr results, NativeError error);

    byte absolute_result(ResultSetPtr results, int row, NativeError error);

    int get_row_position_result(ResultSetPtr results, NativeError error);

    int affected_rows_result(ResultSetPtr results, NativeError error);

    int get_int_value_by_index(ResultSetPtr results, int index, NativeError error);

    long get_long_value_by_index(ResultSetPtr results, int index, NativeError error);

    double get_double_value_by_index(ResultSetPtr results, int index, NativeError error);

    byte get_bool_value_by_index(ResultSetPtr results, int index, NativeError error);

    float get_float_value_by_index(ResultSetPtr results, int index, NativeError error);

    short get_short_value_by_index(ResultSetPtr results, int index, NativeError error);

    Pointer get_string_value_by_index(ResultSetPtr results, int index, NativeError error);

    DateStruct get_date_value_by_index(ResultSetPtr results, int index, NativeError error);

    TimeStruct get_time_value_by_index(ResultSetPtr results, int index, NativeError error);

    TimestampStruct get_timestamp_value_by_index(ResultSetPtr results, int index, NativeError error);

    BinaryArray get_bytes_array_by_index(ResultSetPtr results, int index, NativeError error);

    BinaryStreamPtr get_binary_stream_by_index(ResultSetPtr results, int index, NativeError error);

    int read_binary_stream(BinaryStreamPtr stream, byte[] buffer, int offset, int length, NativeError error);

    byte was_null_by_index(ResultSetPtr results, int index, NativeError error);

    int get_int_value_by_name(ResultSetPtr results, String name, NativeError error);

    long get_long_value_by_name(ResultSetPtr results, String name, NativeError error);

    double get_double_value_by_name(ResultSetPtr results, String name, NativeError error);

    byte get_bool_value_by_name(ResultSetPtr results, String name, NativeError error);

    float get_float_value_by_name(ResultSetPtr results, String name, NativeError error);

    short get_short_value_by_name(ResultSetPtr results, String name, NativeError error);

    Pointer get_string_value_by_name(ResultSetPtr results, String name, NativeError error);

    DateStruct get_date_value_by_name(ResultSetPtr results, String name, NativeError error);

    TimeStruct get_time_value_by_name(ResultSetPtr results, String name, NativeError error);

    TimestampStruct get_timestamp_value_by_name(ResultSetPtr results, String name, NativeError error);

    BinaryArray get_bytes_array_by_name(ResultSetPtr results, String name, NativeError error);

    BinaryStreamPtr get_binary_stream_by_name(ResultSetPtr results, String name, NativeError error);

    int find_column_by_name(ResultSetPtr results, String name, NativeError error);

    byte was_null_by_name(ResultSetPtr results, String name, NativeError error);

    void delete_binary_array(BinaryArray array);

    void delete_date(DateStruct date);

    void delete_time(TimeStruct time);

    void delete_timestamp(TimestampStruct timestamp);

    void close_result(ResultSetPtr results, NativeError error);

    void close_statement(StatementPtr stmt, NativeError error);

    Pointer drivers_list(IntByReference count);

    Pointer datasources_list(IntByReference count);

    void std_free(Pointer ptr);

    void clear_native_error(NativeError error);

    int set_log_level(int level);

    void delete_datasource(DatasourceStruct datasource);

    void delete_datasource_array(Pointer datasource_array, int size);

    void delete_driver(DriverStruct driver);

    void delete_driver_array(Pointer driver_array, int size);

    ResultSetMetaDataStruct get_meta_data(ResultSetPtr results, NativeError error);

    void delete_meta_data(ResultSetMetaDataStruct meta_data);

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

    void close_binary_stream(BinaryStreamPtr stream);
}
