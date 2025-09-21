package io.github.nanodbc4j.exceptions;

import java.sql.SQLException;

public final class NanodbcSQLException extends SQLException {
    public NanodbcSQLException(Exception e) {
        super(e);
    }

    public NanodbcSQLException(String reason) {
        super(reason);
    }
}
