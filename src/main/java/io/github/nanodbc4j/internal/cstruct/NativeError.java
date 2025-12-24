package io.github.nanodbc4j.internal.cstruct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;

import java.nio.charset.StandardCharsets;

@Structure.FieldOrder({"status", "error_message"})
public final class NativeError extends Structure {
    public int status;
    public Pointer error_message;

    public String getErrorMessage() {
        return error_message == null ? "" : error_message.getString(0, StandardCharsets.UTF_8.name());
    }
}
