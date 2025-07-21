package io.github.michael1297.internal.handler;

import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.pointer.StatementPtr;
import io.github.michael1297.internal.struct.NativeError;

public final class StatementHandler {
    private  StatementHandler(){
    }

    public static void close (StatementPtr statement) {
        NativeError nativeError = new NativeError();
        NativeDB.INSTANCE.close_statement(statement, nativeError);
        if(nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
    }
}
