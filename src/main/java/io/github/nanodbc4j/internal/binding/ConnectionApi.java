package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.pointer.ConnectionPtr;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import io.github.nanodbc4j.internal.pointer.StatementPtr;

public interface ConnectionApi extends Library {
    ConnectionApi INSTANCE = initialize();

    private static ConnectionApi initialize() {
        try {
            LibraryLoader.load();
            return Native.load(ConnectionApi.class, LibraryLoader.getNativeLibraryOptions());
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

    /**
     * Закрывает существующее подключение
     *
     * @param connection Указатель на соединение
     * @param error      Возможная ошибка закрытия
     */
    void disconnect(ConnectionPtr connection, NativeError error);
}
