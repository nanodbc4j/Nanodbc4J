package io.github.nanodbc4j.jdbc;

import java.sql.SQLException;
import java.sql.Wrapper;

interface JdbcWrapper extends Wrapper {
    /**
     * {@inheritDoc}
     */
    @Override
    default boolean isWrapperFor(Class<?> iface) throws SQLException {
        return iface != null && iface.isInstance(this);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    default <T> T unwrap(Class<T> iface) throws SQLException {
        try {
            return iface.cast(this);
        } catch (ClassCastException e) {
            throw new SQLException("Unable to unwrap to " + iface.getName(), e);
        }
    }
}
