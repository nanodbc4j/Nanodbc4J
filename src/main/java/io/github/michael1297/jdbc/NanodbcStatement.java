package io.github.michael1297.jdbc;

import io.github.michael1297.exceptions.NanodbcSQLException;
import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.handler.StatementHandler;
import io.github.michael1297.internal.pointer.ResultSetPtr;
import io.github.michael1297.internal.pointer.StatementPtr;

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
public class NanodbcStatement implements Statement {
    protected StatementPtr statementPtr;
    protected final WeakReference<NanodbcConnection> connection;
    protected NanodbcResultSet resultSet = null;
    protected boolean closed = false;

    NanodbcStatement(NanodbcConnection connection, StatementPtr statementPtr) {
        this.statementPtr = statementPtr;
        this.connection = new WeakReference<>(connection);
    }

    @Override
    public ResultSet executeQuery(String sql) throws SQLException {
        throwIfAlreadyClosed();
        try {
            assert connection.get() != null;
            ResultSetPtr resultSetPtr = StatementHandler.execute(connection.get().getConnectionPtr(), sql);
            return new NanodbcResultSet(this, resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public int executeUpdate(String sql) throws SQLException {
        throwIfAlreadyClosed();
        try {
            assert connection.get() != null;
            return StatementHandler.executeUpdate(connection.get().getConnectionPtr(), sql);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void close() throws SQLException {
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

    @Override
    public int getMaxFieldSize() throws SQLException {
        return 0;   // todo
    }

    @Override
    public void setMaxFieldSize(int max) throws SQLException {
        // throw new SQLFeatureNotSupportedException();todo
    }

    @Override
    public int getMaxRows() throws SQLException {
        return 0;   // todo
    }

    @Override
    public void setMaxRows(int max) throws SQLException {
        //throw new SQLFeatureNotSupportedException(); todo
    }

    @Override
    public void setEscapeProcessing(boolean enable) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getQueryTimeout() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setQueryTimeout(int seconds) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void cancel() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public SQLWarning getWarnings() throws SQLException {
        return null;
    }

    @Override
    public void clearWarnings() throws SQLException {

    }

    @Override
    public void setCursorName(String name) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean execute(String sql) throws SQLException {
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

    @Override
    public ResultSet getResultSet() throws SQLException {
        ResultSet rs = resultSet;
        resultSet = null;
        return rs;
    }

    @Override
    public int getUpdateCount() throws SQLException {
        return -1;
        //throw new SQLFeatureNotSupportedException(); todo
    }

    @Override
    public boolean getMoreResults() throws SQLException {
        return resultSet != null;
    }

    @Override
    public void setFetchDirection(int direction) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getFetchDirection() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setFetchSize(int rows) throws SQLException {
        //throw new SQLFeatureNotSupportedException(); todo
    }

    @Override
    public int getFetchSize() throws SQLException {
        return 0;
        //throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getResultSetConcurrency() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getResultSetType() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void addBatch(String sql) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void clearBatch() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int[] executeBatch() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Connection getConnection() throws SQLException {
        throwIfAlreadyClosed();
        return connection.get();
    }

    @Override
    public boolean getMoreResults(int current) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public ResultSet getGeneratedKeys() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int executeUpdate(String sql, int autoGeneratedKeys) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int executeUpdate(String sql, int[] columnIndexes) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int executeUpdate(String sql, String[] columnNames) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean execute(String sql, int autoGeneratedKeys) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean execute(String sql, int[] columnIndexes) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean execute(String sql, String[] columnNames) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getResultSetHoldability() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean isClosed() throws SQLException {
        return closed;
    }

    @Override
    public void setPoolable(boolean poolable) throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean isPoolable() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void closeOnCompletion() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean isCloseOnCompletion() throws SQLException {
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        if (isWrapperFor(iface)) {
            return iface.cast(this);
        }
        throw new NanodbcSQLException("Cannot unwrap to " + iface.getName());
    }

    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {
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
