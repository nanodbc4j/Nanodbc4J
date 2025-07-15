package io.github.michael1297.core;

import com.sun.jna.Function;
import com.sun.jna.LastErrorException;
import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.ptr.PointerByReference;

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
     * @param error Ошибка, возникающая при соединении
     */
    PointerByReference connection(char[] connection_string, NativeError error) throws LastErrorException;

    /**
     * Закрывает существующее подключение
     *
     * @param connection Указатель на соединение
     * @param error Возможная ошибка закрытия
     */
    void disconnect(PointerByReference connection, NativeError error) throws LastErrorException;

    /**
     * Проверяет состояние соединения
     *
     * @param conn Указатель на соединение
     * @param error Информация об ошибке
     * @return True, если соединение активно
     */
    boolean is_connected(PointerByReference conn, NativeError error);

}
