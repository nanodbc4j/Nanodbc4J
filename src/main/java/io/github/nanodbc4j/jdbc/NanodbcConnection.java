package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.exceptions.NanodbcSQLException;
import io.github.nanodbc4j.exceptions.NativeException;
import io.github.nanodbc4j.internal.handler.ConnectionHandler;
import io.github.nanodbc4j.internal.pointer.ConnectionPtr;
import io.github.nanodbc4j.internal.pointer.StatementPtr;
import lombok.AccessLevel;
import lombok.Getter;
import lombok.extern.java.Log;

import java.sql.Array;
import java.sql.Blob;
import java.sql.CallableStatement;
import java.sql.Clob;
import java.sql.Connection;
import java.sql.DatabaseMetaData;
import java.sql.NClob;
import java.sql.PreparedStatement;
import java.sql.SQLClientInfoException;
import java.sql.SQLException;
import java.sql.SQLFeatureNotSupportedException;
import java.sql.SQLWarning;
import java.sql.SQLXML;
import java.sql.Savepoint;
import java.sql.Statement;
import java.sql.Struct;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.Executor;
import java.util.logging.Level;

/**
 * Connection to ODBC data source. Manages native connection handle.
 * Closed via close() or garbage collection.
 */
@Log
public class NanodbcConnection implements Connection {

    @Getter(AccessLevel.PACKAGE)
    private ConnectionPtr connectionPtr;

    private static final long TIMEOUT = 5;

    private DatabaseMetaData metaData = null;

    @Getter(AccessLevel.PACKAGE)
    private String url = null;

    NanodbcConnection(String url) throws SQLException {
        try {
            connectionPtr = ConnectionHandler.connect(url, 5);
            this.url = url;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Statement createStatement() throws SQLException {
        log.log(Level.FINEST, "createStatement");
        try {
            StatementPtr statementPtr = ConnectionHandler.create(connectionPtr);
            return new NanodbcStatement(this, statementPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public PreparedStatement prepareStatement(String sql) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        try {
            StatementPtr statementPtr = ConnectionHandler.create(connectionPtr);
            ConnectionHandler.prepared(statementPtr, sql, TIMEOUT);
            return new NanodbcPreparedStatement(this, statementPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public CallableStatement prepareCall(String sql) throws SQLException {
        log.log(Level.FINEST, "prepareCall");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String nativeSQL(String sql) throws SQLException {
        log.log(Level.FINEST, "nativeSQL");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setAutoCommit(boolean autoCommit) throws SQLException {
        log.log(Level.FINEST, "setAutoCommit");
        try {
            ConnectionHandler.setAutoCommitTransaction(connectionPtr, autoCommit);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean getAutoCommit() throws SQLException {
        log.log(Level.FINEST, "getAutoCommit");
        try {
            return ConnectionHandler.getAutoCommitTransaction(connectionPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void commit() throws SQLException {
        log.log(Level.FINEST, "commit");
        try {
            ConnectionHandler.commitTransaction(connectionPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void rollback() throws SQLException {
        log.log(Level.FINEST, "rollback");
        try {
            ConnectionHandler.rollbackTransaction(connectionPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void close() throws SQLException {
        log.log(Level.FINEST, "close");
        try {
            ConnectionHandler.disconnect(connectionPtr);
            connectionPtr = null;
            url = null;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isClosed() throws SQLException {
        log.log(Level.FINEST, "isClosed");
        try {
            return !ConnectionHandler.isConnected(connectionPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public DatabaseMetaData getMetaData() throws SQLException {
        log.log(Level.FINEST, "getMetaData");
        try {
            if (metaData == null) {
                metaData = ConnectionHandler.getDatabaseSetMetaData(this, connectionPtr);
            }
            return metaData;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setReadOnly(boolean readOnly) throws SQLException {
        log.log(Level.FINEST, "setReadOnly");
        // throw new SQLFeatureNotSupportedException(); TODO
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isReadOnly() throws SQLException {
        log.log(Level.FINEST, "isReadOnly");
        return true;
        //throw new SQLFeatureNotSupportedException(); TODO
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setCatalog(String catalog) throws SQLException {
        log.log(Level.FINEST, "setCatalog");
        try {
            ConnectionHandler.setCatalog(connectionPtr, catalog);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getCatalog() throws SQLException {
        log.log(Level.FINEST, "getCatalog");
        try {
            return ConnectionHandler.getCatalog(connectionPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setTransactionIsolation(int level) throws SQLException {
        log.log(Level.FINEST, "setTransactionIsolation");
        try {
            ConnectionHandler.setTransactionIsolation(connectionPtr, level);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getTransactionIsolation() throws SQLException {
        log.log(Level.FINEST, "getTransactionIsolation");
        try {
            int level = ConnectionHandler.getTransactionIsolation(connectionPtr);
            // Проверим, поддерживается ли значение
            return switch (level) {
                case Connection.TRANSACTION_READ_UNCOMMITTED -> Connection.TRANSACTION_READ_UNCOMMITTED;
                case Connection.TRANSACTION_READ_COMMITTED -> Connection.TRANSACTION_READ_COMMITTED;
                case Connection.TRANSACTION_REPEATABLE_READ -> Connection.TRANSACTION_REPEATABLE_READ;
                case Connection.TRANSACTION_SERIALIZABLE -> Connection.TRANSACTION_SERIALIZABLE;
                default -> Connection.TRANSACTION_READ_COMMITTED; // Если драйвер вернул что-то странное — fallback
            };
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public SQLWarning getWarnings() throws SQLException {
        log.log(Level.FINEST, "getWarnings");
        throwIfAlreadyClosed();
        return null; // TODO
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void clearWarnings() throws SQLException {
        log.log(Level.FINEST, "clearWarnings");
        throwIfAlreadyClosed();
        // nothing to do
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Statement createStatement(int resultSetType, int resultSetConcurrency) throws SQLException {
        log.log(Level.FINEST, "createStatement");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public PreparedStatement prepareStatement(String sql, int resultSetType, int resultSetConcurrency) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public CallableStatement prepareCall(String sql, int resultSetType, int resultSetConcurrency) throws SQLException {
        log.log(Level.FINEST, "prepareCall");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Map<String, Class<?>> getTypeMap() throws SQLException {
        log.log(Level.FINEST, "getTypeMap");
        return Map.of();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setTypeMap(Map<String, Class<?>> map) throws SQLException {
        log.log(Level.FINEST, "setTypeMap");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setHoldability(int holdability) throws SQLException {
        log.log(Level.FINEST, "setHoldability");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getHoldability() throws SQLException {
        log.log(Level.FINEST, "getHoldability");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Savepoint setSavepoint() throws SQLException {
        log.log(Level.FINEST, "setSavepoint");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Savepoint setSavepoint(String name) throws SQLException {
        log.log(Level.FINEST, "setSavepoint");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void rollback(Savepoint savepoint) throws SQLException {
        log.log(Level.FINEST, "rollback");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void releaseSavepoint(Savepoint savepoint) throws SQLException {
        log.log(Level.FINEST, "releaseSavepoint");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Statement createStatement(int resultSetType, int resultSetConcurrency, int resultSetHoldability) throws SQLException {
        log.log(Level.FINEST, "createStatement");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public PreparedStatement prepareStatement(String sql, int resultSetType, int resultSetConcurrency, int resultSetHoldability) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public CallableStatement prepareCall(String sql, int resultSetType, int resultSetConcurrency, int resultSetHoldability) throws SQLException {
        log.log(Level.FINEST, "prepareCall");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public PreparedStatement prepareStatement(String sql, int autoGeneratedKeys) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public PreparedStatement prepareStatement(String sql, int[] columnIndexes) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public PreparedStatement prepareStatement(String sql, String[] columnNames) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Clob createClob() throws SQLException {
        log.log(Level.FINEST, "createClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Blob createBlob() throws SQLException {
        log.log(Level.FINEST, "createBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public NClob createNClob() throws SQLException {
        log.log(Level.FINEST, "createNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public SQLXML createSQLXML() throws SQLException {
        log.log(Level.FINEST, "createSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isValid(int timeout) throws SQLException {
        log.log(Level.FINEST, "isValid");
        return !isClosed();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setClientInfo(String name, String value) throws SQLClientInfoException {
        log.log(Level.FINEST, "setClientInfo");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setClientInfo(Properties properties) throws SQLClientInfoException {
        log.log(Level.FINEST, "setClientInfo");

    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getClientInfo(String name) throws SQLException {
        log.log(Level.FINEST, "getClientInfo");
        return "";
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Properties getClientInfo() throws SQLException {
        log.log(Level.FINEST, "getClientInfo");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Array createArrayOf(String typeName, Object[] elements) throws SQLException {
        log.log(Level.FINEST, "createArrayOf");
        return null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Struct createStruct(String typeName, Object[] attributes) throws SQLException {
        log.log(Level.FINEST, "createStruct");
        return null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setSchema(String schema) throws SQLException {
        log.log(Level.FINEST, "setSchema");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSchema() throws SQLException {
        log.log(Level.FINEST, "getSchema");
        return null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void abort(Executor executor) throws SQLException {
        log.log(Level.FINEST, "abort");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setNetworkTimeout(Executor executor, int milliseconds) throws SQLException {
        log.log(Level.FINEST, "setNetworkTimeout");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getNetworkTimeout() throws SQLException {
        log.log(Level.FINEST, "getNetworkTimeout");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        log.log(Level.FINEST, "unwrap");
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
        log.log(Level.FINEST, "isWrapperFor");
        return iface.isInstance(this) || iface == Connection.class;
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            if (connectionPtr != null) {
                close();
            }
        } finally {
            super.finalize();
        }
    }

    /**
     * Throws exception if Connection is already closed.
     *
     * @throws SQLException if Connection is already closed.
     */
    protected void throwIfAlreadyClosed() throws SQLException {
        if (isClosed() || connectionPtr == null) {
            throw new NanodbcSQLException("Connection: already closed");
        }
    }
}
