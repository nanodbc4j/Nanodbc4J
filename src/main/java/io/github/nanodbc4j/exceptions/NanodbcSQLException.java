package io.github.nanodbc4j.exceptions;

import java.sql.SQLException;

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
