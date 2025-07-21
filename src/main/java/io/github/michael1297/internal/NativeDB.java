package io.github.michael1297.internal;

import com.sun.jna.Function;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import io.github.michael1297.internal.pointer.ConnectionPtr;
import io.github.michael1297.internal.pointer.ResultSetPtr;
import io.github.michael1297.internal.pointer.StatementPtr;
import io.github.michael1297.internal.struct.DatasourceStruct;
import io.github.michael1297.internal.struct.DriverStruct;
import io.github.michael1297.internal.struct.NativeError;

import java.util.HashMap;
import java.util.Map;

public interface NativeDB extends Library {
    NativeDB INSTANCE = Native.load(getLibraryName(), NativeDB.class, getOptions());

    private static String getLibraryName() {
        if (Platform.isWindows()) {
            return "odbc";
        } else if (Platform.isLinux()) {
            return "libodbc"; // или "libodbc.so" в зависимости от установки
        }
        throw new UnsupportedOperationException("Unsupported platform");
    }

    private static Map<String, Object> getOptions() {
        Map<String, Object> options = new HashMap<>();
        // Nanodbc обычно ожидает UTF-16 независимо от платформы
        options.put(Library.OPTION_STRING_ENCODING, "UTF-16LE");
        options.put(Library.OPTION_CALLING_CONVENTION, Function.C_CONVENTION);
        return options;
    }

    /**
     * Метод создаёт соединение с базой данных
     *
     * @param connection_string Массив символов (UTF-16LE) строки подключения
     * @param error             Ошибка, возникающая при соединении
     */
    ConnectionPtr connection(String connection_string, NativeError error);

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
    boolean is_connected(ConnectionPtr conn, NativeError error);

    StatementPtr create_statement(ConnectionPtr conn, NativeError error);

    void prepare_statement(StatementPtr stmt, String sql, long timeout, NativeError error);

    ResultSetPtr execute(StatementPtr stmt, NativeError error);

    boolean next_result(ResultSetPtr results, NativeError error);

    int get_int_value_by_index(ResultSetPtr results, int index, NativeError error);

    long get_long_value_by_index(ResultSetPtr results, int index, NativeError error);

    double get_double_value_by_index(ResultSetPtr results, int index, NativeError error);

    boolean get_bool_value_by_index(ResultSetPtr results, int index, NativeError error);

    float get_float_value_by_index(ResultSetPtr results, int index, NativeError error);

    short get_short_value_by_index(ResultSetPtr results, int index, NativeError error);

    int get_int_value_by_name(ResultSetPtr results, String name, NativeError error);

    long get_long_value_by_name(ResultSetPtr results, String name, NativeError error);

    double get_double_value_by_name(ResultSetPtr results, String name, NativeError error);

    boolean get_bool_value_by_name(ResultSetPtr results, String name, NativeError error);

    float get_float_value_by_name(ResultSetPtr results, String name, NativeError error);

    short get_short_value_by_name(ResultSetPtr results, String name, NativeError error);

    void close_result(ResultSetPtr results, NativeError error);

    void close_statement(StatementPtr stmt, NativeError error);

    Pointer drivers_list(IntByReference count);

    Pointer datasources_list(IntByReference count);

    void std_free(Pointer ptr);

    void clear_native_error(NativeError error);

    void delete_datasource (DatasourceStruct datasource);

    void delete_datasource_array (Pointer datasource_array, int size);

    void delete_driver (DriverStruct driver);

    void delete_driver_array (Pointer driver_array, int size);
}
