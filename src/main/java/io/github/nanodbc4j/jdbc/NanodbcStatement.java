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
        log.finest("NanodbcStatement.executeQuery");
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
        log.finest("NanodbcStatement.executeUpdate");
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
        log.finest("NanodbcStatement.close");
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
        log.finest("NanodbcStatement.getMaxFieldSize");
        return 0;   // todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setMaxFieldSize(int max) throws SQLException {
        log.finest("NanodbcStatement.setMaxFieldSize");
        // throw new SQLFeatureNotSupportedException();todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getMaxRows() throws SQLException {
        log.finest("NanodbcStatement.getMaxRows");
        return 0;   // todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setMaxRows(int max) throws SQLException {
        log.finest("NanodbcStatement.setMaxRows");
        //throw new SQLFeatureNotSupportedException(); todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setEscapeProcessing(boolean enable) throws SQLException {
        log.finest("NanodbcStatement.setEscapeProcessing");
        //throw new SQLFeatureNotSupportedException(); todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getQueryTimeout() throws SQLException {
        log.finest("NanodbcStatement.getQueryTimeout");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setQueryTimeout(int seconds) throws SQLException {
        log.finest("NanodbcStatement.setQueryTimeout");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void cancel() throws SQLException {
        log.finest("NanodbcStatement.cancel");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public SQLWarning getWarnings() throws SQLException {
        log.finest("NanodbcStatement.getWarnings");
        return null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void clearWarnings() throws SQLException {
        log.finest("NanodbcStatement.clearWarnings");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setCursorName(String name) throws SQLException {
        log.finest("NanodbcStatement.setCursorName");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean execute(String sql) throws SQLException {
        log.finest("NanodbcStatement.execute");
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
        log.finest("NanodbcStatement.getResultSet");
        ResultSet rs = resultSet;
        resultSet = null;
        return rs;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getUpdateCount() throws SQLException {
        log.finest("NanodbcStatement.getUpdateCount");
        return -1;
        //throw new SQLFeatureNotSupportedException(); todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean getMoreResults() throws SQLException {
        log.finest("NanodbcStatement.getMoreResults");
        return resultSet != null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setFetchDirection(int direction) throws SQLException {
        log.finest("NanodbcStatement.setFetchDirection");
        throwIfAlreadyClosed();
        // TODO
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getFetchDirection() throws SQLException {
        log.finest("NanodbcStatement.getFetchDirection");
        throwIfAlreadyClosed();
        return ResultSet.FETCH_FORWARD; // TODO
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setFetchSize(int rows) throws SQLException {
        log.finest("NanodbcStatement.setFetchSize");
        throwIfAlreadyClosed();
        //throw new SQLFeatureNotSupportedException(); todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getFetchSize() throws SQLException {
        log.finest("NanodbcStatement.getFetchSize");
        return 0;
        //throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getResultSetConcurrency() throws SQLException {
        log.finest("NanodbcStatement.getResultSetConcurrency");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getResultSetType() throws SQLException {
        log.finest("NanodbcStatement.getResultSetType");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void addBatch(String sql) throws SQLException {
        log.finest("NanodbcStatement.addBatch");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void clearBatch() throws SQLException {
        log.finest("NanodbcStatement.clearBatch");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int[] executeBatch() throws SQLException {
        log.finest("NanodbcStatement.executeBatch");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Connection getConnection() throws SQLException {
        log.finest("NanodbcStatement.getConnection");
        throwIfAlreadyClosed();
        return connection.get();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean getMoreResults(int current) throws SQLException {
        log.finest("NanodbcStatement.getMoreResults");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet getGeneratedKeys() throws SQLException {
        log.finest("NanodbcStatement.getGeneratedKeys");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int executeUpdate(String sql, int autoGeneratedKeys) throws SQLException {
        log.finest("NanodbcStatement.executeUpdate");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int executeUpdate(String sql, int[] columnIndexes) throws SQLException {
        log.finest("NanodbcStatement.executeUpdate");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int executeUpdate(String sql, String[] columnNames) throws SQLException {
        log.finest("NanodbcStatement.executeUpdate");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean execute(String sql, int autoGeneratedKeys) throws SQLException {
        log.finest("NanodbcStatement.execute");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean execute(String sql, int[] columnIndexes) throws SQLException {
        log.finest("NanodbcStatement.execute");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean execute(String sql, String[] columnNames) throws SQLException {
        log.finest("NanodbcStatement.execute");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getResultSetHoldability() throws SQLException {
        log.finest("NanodbcStatement.getResultSetHoldability");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isClosed() throws SQLException {
        log.finest("NanodbcStatement.isClosed");
        return closed;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setPoolable(boolean poolable) throws SQLException {
        log.finest("NanodbcStatement.setPoolable");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isPoolable() throws SQLException {
        log.finest("NanodbcStatement.isPoolable");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void closeOnCompletion() throws SQLException {
        log.finest("NanodbcStatement.closeOnCompletion");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isCloseOnCompletion() throws SQLException {
        log.finest("NanodbcStatement.isCloseOnCompletion");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        log.finest("NanodbcStatement.unwrap");
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
        log.finest("NanodbcStatement.isWrapperFor");
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
