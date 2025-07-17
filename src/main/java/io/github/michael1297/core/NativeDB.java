package io.github.michael1297.core;

import com.sun.jna.Function;
import com.sun.jna.LastErrorException;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;

import java.util.HashMap;
import java.util.Map;

public interface NativeDB extends Library {
    NativeDB INSTANCE = Native.load(getLibraryName(), NativeDB.class, getOptions());

    int POINTER_SIZE = (System.getProperty("os.arch").endsWith("64") ? 8 : 4);

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
    Pointer connection(char[] connection_string, long timeout, NativeError error) throws LastErrorException;

    /**
     * Закрывает существующее подключение
     *
     * @param connection Указатель на соединение
     * @param error      Возможная ошибка закрытия
     */
    void disconnect(Pointer connection, NativeError error) throws LastErrorException;

    /**
     * Проверяет состояние соединения
     *
     * @param conn  Указатель на соединение
     * @param error Информация об ошибке
     * @return True, если соединение активно
     */
    boolean is_connected(Pointer conn, NativeError error);

    Pointer drivers_list(IntByReference count);

    Pointer datasources_list(IntByReference count);

    void std_free(Pointer ptr);
}
