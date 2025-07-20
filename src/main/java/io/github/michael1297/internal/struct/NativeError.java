package io.github.michael1297.internal.struct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;

import java.nio.charset.StandardCharsets;

@Structure.FieldOrder({"error_code", "error_message", "error_type"})
public final class NativeError extends Structure {
    public int error_code;
    public Pointer error_message;
    public Pointer error_type;

    public String getErrorMessage() {
        return error_message == null ? "" : error_message.getString(0, StandardCharsets.UTF_8.name());
    }

    public String getErrorType() {
        return error_type == null ? "" : error_type.getString(0, StandardCharsets.UTF_8.name());
    }
}
