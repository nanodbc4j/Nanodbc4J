package io.github.nanodbc4j.exceptions;

import io.github.nanodbc4j.internal.cstruct.NativeError;
import lombok.Getter;

/**
 * Represents an exception originating from native (e.g., C/C++/ODBC) code.
 * Carries structured error information such as error code and type,
 * facilitating diagnostics and conditional handling of native-layer failures.
 */
@Getter
public final class NativeException extends RuntimeException {
    private final int status;

    public NativeException(String message) {
        super(message);
        this.status = 0;
    }

    public NativeException(NativeError error) {
        super(error.getErrorMessage());
        this.status = error.status;
    }

    public NativeException(int status, String message) {
        super(message);
        this.status = status;
    }
}
