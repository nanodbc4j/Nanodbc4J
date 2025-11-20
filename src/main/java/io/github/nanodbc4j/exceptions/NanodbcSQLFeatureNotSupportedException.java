package io.github.nanodbc4j.exceptions;

import java.sql.SQLFeatureNotSupportedException;

public final class NanodbcSQLFeatureNotSupportedException extends SQLFeatureNotSupportedException {
    private static final String DEFAULT_MSG = "This feature is not supported";
    private static final String MESSAGE_FORMAT = "%s.%s - driver does not support this feature";
    private static final StackWalker STACK_WALKER = StackWalker.getInstance(StackWalker.Option.RETAIN_CLASS_REFERENCE);

    public NanodbcSQLFeatureNotSupportedException() {
        super(generateMessage());
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
                        .map(frame -> {
                            String name = frame.getDeclaringClass().getSimpleName();
                            return name.isEmpty() ? DEFAULT_MSG : String.format(MESSAGE_FORMAT, name, frame.getMethodName());
                        })
                        .orElse(DEFAULT_MSG)
        );
    }
}
