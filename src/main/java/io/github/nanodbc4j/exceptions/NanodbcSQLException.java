package io.github.nanodbc4j.exceptions;

import java.sql.SQLException;

/**
 * General-purpose SQL exception for this driver.
 * Wraps database-specific errors and provides standard {@link SQLException}
 * constructors for flexibility in error reporting.
 */
public final class NanodbcSQLException extends SQLException {
    public NanodbcSQLException(String reason, Throwable cause) {
        super(reason, cause);
    }

    public NanodbcSQLException(Throwable cause) {
        super(cause);
    }

    public NanodbcSQLException(String reason) {
        super(reason);
    }
}
