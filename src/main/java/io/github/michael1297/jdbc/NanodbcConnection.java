package io.github.michael1297.jdbc;

import io.github.michael1297.exceptions.NanodbcSQLException;
import io.github.michael1297.exceptions.NativeException;
import io.github.michael1297.internal.handler.ConnectionHandler;
import io.github.michael1297.internal.pointer.ConnectionPtr;
import io.github.michael1297.internal.pointer.StatementPtr;
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
    private ConnectionPtr connectionPtr;
    private static final long TIMEOUT = 5;
    private DatabaseMetaData metaData = null;

    NanodbcConnection(String url) throws SQLException {
        try {
            connectionPtr = ConnectionHandler.connect(url, 5);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

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

    @Override
    public CallableStatement prepareCall(String sql) throws SQLException {
        log.log(Level.FINEST, "prepareCall");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public String nativeSQL(String sql) throws SQLException {
        log.log(Level.FINEST, "nativeSQL");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setAutoCommit(boolean autoCommit) throws SQLException {
        log.log(Level.FINEST, "setAutoCommit");
        try {
            ConnectionHandler.setAutoCommitTransaction(connectionPtr, autoCommit);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public boolean getAutoCommit() throws SQLException {
        log.log(Level.FINEST, "getAutoCommit");
        try {
            return ConnectionHandler.getAutoCommitTransaction(connectionPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void commit() throws SQLException {
        log.log(Level.FINEST, "commit");
        try {
            ConnectionHandler.commitTransaction(connectionPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void rollback() throws SQLException {
        log.log(Level.FINEST, "rollback");
        try {
            ConnectionHandler.rollbackTransaction(connectionPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void close() throws SQLException {
        log.log(Level.FINEST, "close");
        try {
            ConnectionHandler.disconnect(connectionPtr);
            connectionPtr = null;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public boolean isClosed() throws SQLException {
        log.log(Level.FINEST, "isClosed");
        try {
            return !ConnectionHandler.isConnected(connectionPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

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

    @Override
    public void setReadOnly(boolean readOnly) throws SQLException {
        log.log(Level.FINEST, "setReadOnly");
        // throw new SQLFeatureNotSupportedException(); TODO
    }

    @Override
    public boolean isReadOnly() throws SQLException {
        log.log(Level.FINEST, "isReadOnly");
        return true;
        //throw new SQLFeatureNotSupportedException(); TODO
    }

    @Override
    public void setCatalog(String catalog) throws SQLException {
        log.log(Level.FINEST, "setCatalog");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public String getCatalog() throws SQLException {
        log.log(Level.FINEST, "getCatalog");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setTransactionIsolation(int level) throws SQLException {
        log.log(Level.FINEST, "setTransactionIsolation");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getTransactionIsolation() throws SQLException {
        log.log(Level.FINEST, "getTransactionIsolation");
        // Используем метаданные — они уже содержат SQL_DEFAULT_TXN_ISOLATION
        int defaultIsolation = getMetaData().getDefaultTransactionIsolation();

        // Проверим, поддерживается ли значение
        return switch (defaultIsolation) {
            case Connection.TRANSACTION_READ_UNCOMMITTED -> Connection.TRANSACTION_READ_UNCOMMITTED;
            case Connection.TRANSACTION_READ_COMMITTED -> Connection.TRANSACTION_READ_COMMITTED;
            case Connection.TRANSACTION_REPEATABLE_READ -> Connection.TRANSACTION_REPEATABLE_READ;
            case Connection.TRANSACTION_SERIALIZABLE -> Connection.TRANSACTION_SERIALIZABLE;
            default -> Connection.TRANSACTION_READ_COMMITTED; // Если драйвер вернул что-то странное — fallback
        };
    }

    @Override
    public SQLWarning getWarnings() throws SQLException {
        log.log(Level.FINEST, "getWarnings");
        throwIfAlreadyClosed();
        return null; // TODO
    }

    @Override
    public void clearWarnings() throws SQLException {
        log.log(Level.FINEST, "clearWarnings");
        throwIfAlreadyClosed();
        // nothing to do
    }

    @Override
    public Statement createStatement(int resultSetType, int resultSetConcurrency) throws SQLException {
        log.log(Level.FINEST, "createStatement");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public PreparedStatement prepareStatement(String sql, int resultSetType, int resultSetConcurrency) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public CallableStatement prepareCall(String sql, int resultSetType, int resultSetConcurrency) throws SQLException {
        log.log(Level.FINEST, "prepareCall");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Map<String, Class<?>> getTypeMap() throws SQLException {
        log.log(Level.FINEST, "getTypeMap");
        return Map.of();
    }

    @Override
    public void setTypeMap(Map<String, Class<?>> map) throws SQLException {
        log.log(Level.FINEST, "setTypeMap");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setHoldability(int holdability) throws SQLException {
        log.log(Level.FINEST, "setHoldability");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getHoldability() throws SQLException {
        log.log(Level.FINEST, "getHoldability");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Savepoint setSavepoint() throws SQLException {
        log.log(Level.FINEST, "setSavepoint");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Savepoint setSavepoint(String name) throws SQLException {
        log.log(Level.FINEST, "setSavepoint");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void rollback(Savepoint savepoint) throws SQLException {
        log.log(Level.FINEST, "rollback");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void releaseSavepoint(Savepoint savepoint) throws SQLException {
        log.log(Level.FINEST, "releaseSavepoint");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Statement createStatement(int resultSetType, int resultSetConcurrency, int resultSetHoldability) throws SQLException {
        log.log(Level.FINEST, "createStatement");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public PreparedStatement prepareStatement(String sql, int resultSetType, int resultSetConcurrency, int resultSetHoldability) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public CallableStatement prepareCall(String sql, int resultSetType, int resultSetConcurrency, int resultSetHoldability) throws SQLException {
        log.log(Level.FINEST, "prepareCall");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public PreparedStatement prepareStatement(String sql, int autoGeneratedKeys) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public PreparedStatement prepareStatement(String sql, int[] columnIndexes) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public PreparedStatement prepareStatement(String sql, String[] columnNames) throws SQLException {
        log.log(Level.FINEST, "prepareStatement");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Clob createClob() throws SQLException {
        log.log(Level.FINEST, "createClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Blob createBlob() throws SQLException {
        log.log(Level.FINEST, "createBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public NClob createNClob() throws SQLException {
        log.log(Level.FINEST, "createNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public SQLXML createSQLXML() throws SQLException {
        log.log(Level.FINEST, "createSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean isValid(int timeout) throws SQLException {
        log.log(Level.FINEST, "isValid");
        return !isClosed();
    }

    @Override
    public void setClientInfo(String name, String value) throws SQLClientInfoException {
        log.log(Level.FINEST, "setClientInfo");
    }

    @Override
    public void setClientInfo(Properties properties) throws SQLClientInfoException {
        log.log(Level.FINEST, "setClientInfo");

    }

    @Override
    public String getClientInfo(String name) throws SQLException {
        log.log(Level.FINEST, "getClientInfo");
        return "";
    }

    @Override
    public Properties getClientInfo() throws SQLException {
        log.log(Level.FINEST, "getClientInfo");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Array createArrayOf(String typeName, Object[] elements) throws SQLException {
        log.log(Level.FINEST, "createArrayOf");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Struct createStruct(String typeName, Object[] attributes) throws SQLException {
        log.log(Level.FINEST, "createStruct");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setSchema(String schema) throws SQLException {
        log.log(Level.FINEST, "setSchema");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public String getSchema() throws SQLException {
        log.log(Level.FINEST, "getSchema");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void abort(Executor executor) throws SQLException {
        log.log(Level.FINEST, "abort");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setNetworkTimeout(Executor executor, int milliseconds) throws SQLException {
        log.log(Level.FINEST, "setNetworkTimeout");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getNetworkTimeout() throws SQLException {
        log.log(Level.FINEST, "getNetworkTimeout");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        log.log(Level.FINEST, "unwrap");
        if (isWrapperFor(iface)) {
            return iface.cast(this);
        }
        throw new NanodbcSQLException("Cannot unwrap to " + iface.getName());
    }

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

    ConnectionPtr getConnectionPtr() {
        return connectionPtr;
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
