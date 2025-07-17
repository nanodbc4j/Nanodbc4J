package io.github.michael1297.core.struct;

import com.sun.jna.Structure;

@Structure.FieldOrder({"error_code", "error_message", "error_type"})
public class NativeError extends Structure {
    public int error_code;
    public byte[] error_message = new byte[256];
    public byte[] error_type = new byte[64];

    public String getErrorMessage() {
        return new String(error_message).trim();
    }

    public String getErrorType() {
        return new String(error_type).trim();
    }
}
