package io.github.nanodbc4j.exceptions;

import io.github.nanodbc4j.internal.cstruct.NativeError;
import lombok.Getter;

@Getter
public final class NativeException extends RuntimeException {
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

}
