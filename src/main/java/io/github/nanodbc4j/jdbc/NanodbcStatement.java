package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.exceptions.NanodbcSQLException;
import io.github.nanodbc4j.exceptions.NativeException;
import io.github.nanodbc4j.internal.handler.StatementHandler;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import io.github.nanodbc4j.internal.pointer.StatementPtr;
import lombok.extern.java.Log;

import java.lang.ref.WeakReference;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.SQLFeatureNotSupportedException;
import java.sql.SQLWarning;
import java.sql.Statement;

/**
 * Executes SQL statements. Forward-only, read-only result sets.
 */
@Log
public class NanodbcStatement implements Statement {
    protected StatementPtr statementPtr;
    protected final WeakReference<NanodbcConnection> connection;
    protected NanodbcResultSet resultSet = null;
    protected boolean closed = false;

    NanodbcStatement(NanodbcConnection connection, StatementPtr statementPtr) {
        this.statementPtr = statementPtr;
        this.connection = new WeakReference<>(connection);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet executeQuery(String sql) throws SQLException {
        log.finest("executeQuery");
        throwIfAlreadyClosed();
        try {
            assert connection.get() != null;
            ResultSetPtr resultSetPtr = StatementHandler.execute(connection.get().getConnectionPtr(), sql);
            return new NanodbcResultSet(this, resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int executeUpdate(String sql) throws SQLException {
        log.finest("executeUpdate");
        throwIfAlreadyClosed();
        try {
            assert connection.get() != null;
            return StatementHandler.executeUpdate(connection.get().getConnectionPtr(), sql);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void close() throws SQLException {
        log.finest("close");
        try {
            if (resultSet != null) {
                resultSet.close();
            }
            StatementHandler.close(statementPtr);
            statementPtr = null;
            closed = true;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxFieldSize() throws SQLException {
        log.finest("getMaxFieldSize");
        return 0;   // todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setMaxFieldSize(int max) throws SQLException {
        log.finest("setMaxFieldSize");
        // throw new SQLFeatureNotSupportedException();todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxRows() throws SQLException {
        log.finest("getMaxRows");
        return 0;   // todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setMaxRows(int max) throws SQLException {
        log.finest("setMaxRows");
        //throw new SQLFeatureNotSupportedException(); todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setEscapeProcessing(boolean enable) throws SQLException {
        log.finest("setEscapeProcessing");
        //throw new SQLFeatureNotSupportedException(); todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getQueryTimeout() throws SQLException {
        log.finest("getQueryTimeout");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setQueryTimeout(int seconds) throws SQLException {
        log.finest("setQueryTimeout");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void cancel() throws SQLException {
        log.finest("cancel");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public SQLWarning getWarnings() throws SQLException {
        log.finest("getWarnings");
        return null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void clearWarnings() throws SQLException {
        log.finest("clearWarnings");

    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setCursorName(String name) throws SQLException {
        log.finest("setCursorName");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean execute(String sql) throws SQLException {
        log.finest("execute");
        throwIfAlreadyClosed();
        try {
            assert connection.get() != null;
            ResultSetPtr resultSetPtr = StatementHandler.execute(connection.get().getConnectionPtr(), sql);
            resultSet = new NanodbcResultSet(this, resultSetPtr);
            return true;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getResultSet() throws SQLException {
        log.finest("getResultSet");
        ResultSet rs = resultSet;
        resultSet = null;
        return rs;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getUpdateCount() throws SQLException {
        log.finest("getUpdateCount");
        return -1;
        //throw new SQLFeatureNotSupportedException(); todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean getMoreResults() throws SQLException {
        log.finest("getMoreResults");
        return resultSet != null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setFetchDirection(int direction) throws SQLException {
        log.finest("setFetchDirection");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getFetchDirection() throws SQLException {
        log.finest("getFetchDirection");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setFetchSize(int rows) throws SQLException {
        log.finest("setFetchSize");
        //throw new SQLFeatureNotSupportedException(); todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getFetchSize() throws SQLException {
        log.finest("getFetchSize");
        return 0;
        //throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getResultSetConcurrency() throws SQLException {
        log.finest("getResultSetConcurrency");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getResultSetType() throws SQLException {
        log.finest("getResultSetType");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void addBatch(String sql) throws SQLException {
        log.finest("addBatch");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void clearBatch() throws SQLException {
        log.finest("clearBatch");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int[] executeBatch() throws SQLException {
        log.finest("executeBatch");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Connection getConnection() throws SQLException {
        log.finest("getConnection");
        throwIfAlreadyClosed();
        return connection.get();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean getMoreResults(int current) throws SQLException {
        log.finest("getMoreResults");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getGeneratedKeys() throws SQLException {
        log.finest("getGeneratedKeys");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int executeUpdate(String sql, int autoGeneratedKeys) throws SQLException {
        log.finest("executeUpdate");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int executeUpdate(String sql, int[] columnIndexes) throws SQLException {
        log.finest("executeUpdate");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int executeUpdate(String sql, String[] columnNames) throws SQLException {
        log.finest("executeUpdate");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean execute(String sql, int autoGeneratedKeys) throws SQLException {
        log.finest("execute");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean execute(String sql, int[] columnIndexes) throws SQLException {
        log.finest("execute");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean execute(String sql, String[] columnNames) throws SQLException {
        log.finest("execute");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getResultSetHoldability() throws SQLException {
        log.finest("getResultSetHoldability");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isClosed() throws SQLException {
        log.finest("isClosed");
        return closed;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setPoolable(boolean poolable) throws SQLException {
        log.finest("setPoolable");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isPoolable() throws SQLException {
        log.finest("isPoolable");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void closeOnCompletion() throws SQLException {
        log.finest("closeOnCompletion");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isCloseOnCompletion() throws SQLException {
        log.finest("isCloseOnCompletion");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        log.finest("unwrap");
        if (isWrapperFor(iface)) {
            return iface.cast(this);
        }
        throw new NanodbcSQLException("Cannot unwrap to " + iface.getName());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {
        log.finest("isWrapperFor");
        return iface.isInstance(this) || iface == Statement.class;
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            if (!isClosed()) {
                close();
            }
        } finally {
            super.finalize();
        }
    }

    /**
     * Throws exception if statement is already closed.
     *
     * @throws SQLException if statement is already closed.
     */
    protected void throwIfAlreadyClosed() throws SQLException {
        if (isClosed() || statementPtr == null) {
            throw new NanodbcSQLException("Statement: already closed");
        }
    }
}
