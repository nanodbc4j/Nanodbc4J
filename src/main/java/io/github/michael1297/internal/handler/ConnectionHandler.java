package io.github.michael1297.internal.handler;

import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.pointer.ConnectionPtr;
import io.github.michael1297.internal.struct.NativeError;

public final class ConnectionHandler {
    private ConnectionHandler () {
    }

    public static ConnectionPtr connect(String connection_string) {
        NativeError nativeError = new NativeError();
        ConnectionPtr ptr = NativeDB.INSTANCE.connection(connection_string, nativeError);
        if(nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
        return ptr;
    }

    public static ConnectionPtr connect(String connection_string, long timeout) {
        NativeError nativeError = new NativeError();
        ConnectionPtr ptr = NativeDB.INSTANCE.connection_with_timeout(connection_string, timeout, nativeError);
        if(nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
        return ptr;
    }

    public static ConnectionPtr connect(String dsn, String user, String pass, long timeout) {
        NativeError nativeError = new NativeError();
        ConnectionPtr ptr = NativeDB.INSTANCE.connection_with_user_pass_timeout(dsn, user, pass, timeout, nativeError);
        if(nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
        return ptr;
    }

    public static void disconnect(ConnectionPtr ptr) {
        NativeError nativeError = new NativeError();
        NativeDB.INSTANCE.disconnect(ptr, nativeError);
        if(nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
    }

    public static boolean isConnected(ConnectionPtr ptr) {
        NativeError nativeError = new NativeError();
        boolean result = NativeDB.INSTANCE.is_connected(ptr, nativeError);
        if(nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
        return result;
    }

}
