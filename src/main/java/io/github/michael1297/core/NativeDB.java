package io.github.michael1297.core;

import com.sun.jna.Function;
import com.sun.jna.LastErrorException;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import io.github.michael1297.core.pointer.ConnectionPtr;
import io.github.michael1297.core.pointer.ResultSetPtr;
import io.github.michael1297.core.pointer.StatementPtr;
import io.github.michael1297.core.pointer.ArrayPtr;
import io.github.michael1297.core.struct.NativeError;

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
    ConnectionPtr connection(String connection_string, long timeout, NativeError error) throws LastErrorException;

    /**
     * Закрывает существующее подключение
     *
     * @param connection Указатель на соединение
     * @param error      Возможная ошибка закрытия
     */
    void disconnect(ConnectionPtr connection, NativeError error) throws LastErrorException;

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

    void close_result(ResultSetPtr results, NativeError error);

    void close_statement(StatementPtr stmt, NativeError error);

    ArrayPtr drivers_list(IntByReference count);

    ArrayPtr datasources_list(IntByReference count);

    void std_free(Pointer ptr);
}
