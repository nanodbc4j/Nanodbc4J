package io.github.michael1297.core;

import io.github.michael1297.core.struct.NativeError;

public class NativeException extends RuntimeException {
    private final int errorCode;
    private final String errorType;

    public NativeException(NativeError error) {
        super(error.getErrorMessage());
        this.errorCode = error.error_code;
        this.errorType = error.getErrorType();
    }

    public NativeException(int errorCode, String errorType, String message) {
        super(message);
        this.errorCode = errorCode;
        this.errorType = errorType;
    }

    public int getErrorCode() {
        return errorCode;
    }

    public String getErrorType() {
        return errorType;
    }
}
