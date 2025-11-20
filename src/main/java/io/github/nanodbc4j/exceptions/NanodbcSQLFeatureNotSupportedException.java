package io.github.nanodbc4j.exceptions;

import java.sql.SQLFeatureNotSupportedException;

/**
 * Exception indicating that a feature is not supported by this driver.
 * Automatically enriches the error message with the caller class and method name
 * to aid in pinpointing the source of the unsupported operation.
 */
public final class NanodbcSQLFeatureNotSupportedException extends SQLFeatureNotSupportedException {
    private static final String MESSAGE = "Driver does not support this feature";
    private static final String FORMAT = "%s.%s";
    private static final StackWalker STACK_WALKER = StackWalker.getInstance(StackWalker.Option.RETAIN_CLASS_REFERENCE);

    public NanodbcSQLFeatureNotSupportedException() {
        super(generateMessage() + " - " + MESSAGE);
    }

    public NanodbcSQLFeatureNotSupportedException(String reason) {
        super(generateMessage() + ": " + reason);
    }

    public NanodbcSQLFeatureNotSupportedException(String reason, Throwable cause) {
        super(generateMessage() + ": " + reason, cause);
    }

    private static String generateMessage() {
        return STACK_WALKER.walk(stackFrameStream ->
                stackFrameStream
                        .filter(frame -> frame.getDeclaringClass() != NanodbcSQLFeatureNotSupportedException.class)
                        .findFirst()
                        .filter(frame -> !frame.getDeclaringClass().getSimpleName().isEmpty())
                        .map(frame -> String.format(FORMAT, frame.getDeclaringClass().getSimpleName(), frame.getMethodName()))
                        .orElse("")
        );
    }
}
