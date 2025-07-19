package io.github.michael1297.internal.struct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import io.github.michael1297.internal.NativeDB;

import java.nio.charset.StandardCharsets;

@Structure.FieldOrder({"error_code", "error_message", "error_type"})
public final class NativeError extends Structure implements AutoCloseable {
    public int error_code;
    public Pointer error_message;
    public Pointer error_type;

    private volatile boolean closed = false;

    public String getErrorMessage() {
        return error_message == null ? "" : error_message.getString(0, StandardCharsets.UTF_8.name());
    }

    public String getErrorType() {
        return error_type == null ? "" : error_type.getString(0, StandardCharsets.UTF_8.name());
    }

    @Override
    public void close() {
        if (closed) return;

        if(error_type != null) {
            NativeDB.INSTANCE.std_free(error_type);
        }
        if (error_message != null) {
            NativeDB.INSTANCE.std_free(error_message);
        }

        closed = true;
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            close();
        } finally {
            super.finalize();
        }
    }
}
