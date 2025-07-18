package io.github.michael1297.exceptions;

import java.sql.SQLException;

public final class NanodbcSQLException extends SQLException {
    public NanodbcSQLException(String reason) {
        super(reason);
    }
}
