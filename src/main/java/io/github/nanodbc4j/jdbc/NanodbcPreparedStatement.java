package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.exceptions.NanodbcSQLException;
import io.github.nanodbc4j.exceptions.NativeException;
import io.github.nanodbc4j.internal.NativeDB;
import io.github.nanodbc4j.internal.handler.StatementHandler;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import io.github.nanodbc4j.internal.pointer.StatementPtr;
import lombok.extern.java.Log;

import java.io.InputStream;
import java.io.Reader;
import java.math.BigDecimal;
import java.net.URL;
import java.sql.Array;
import java.sql.Blob;
import java.sql.Clob;
import java.sql.Date;
import java.sql.NClob;
import java.sql.ParameterMetaData;
import java.sql.PreparedStatement;
import java.sql.Ref;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.RowId;
import java.sql.SQLException;
import java.sql.SQLFeatureNotSupportedException;
import java.sql.SQLXML;
import java.sql.Time;
import java.sql.Timestamp;
import java.sql.Types;
import java.util.Calendar;

/**
 * Prepared SQL statement with parameter support (?). Use setX() methods to bind values.
 */
@Log
public class NanodbcPreparedStatement extends NanodbcStatement implements PreparedStatement {

    public NanodbcPreparedStatement(NanodbcConnection connection, StatementPtr statementPtr) {
        super(connection, statementPtr);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSet executeQuery() throws SQLException {
        log.finest("NanodbcPreparedStatement.executeQuery");
        throwIfAlreadyClosed();
        try {
            ResultSetPtr resultSetPtr = StatementHandler.execute(statementPtr);
            return new NanodbcResultSet(this, resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int executeUpdate() throws SQLException {
        log.finest("NanodbcPreparedStatement.executeUpdate");
        throwIfAlreadyClosed();
        try {
            return StatementHandler.executeUpdate(statementPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setNull(int parameterIndex, int sqlType) throws SQLException {
        log.finest("NanodbcPreparedStatement.setNull");
        setString(parameterIndex, null);    // Используем String метод
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setBoolean(int parameterIndex, boolean x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setBoolean");
        throwIfAlreadyClosed();
        try {
            byte value = (byte) (x ? 1 : 0);
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, value, NativeDB.INSTANCE::set_bool_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setByte(int parameterIndex, byte x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setByte");
        throwIfAlreadyClosed();
        setShort(parameterIndex, (short) x);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setShort(int parameterIndex, short x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setShort");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x, NativeDB.INSTANCE::set_short_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setInt(int parameterIndex, int x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setInt");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x, NativeDB.INSTANCE::set_int_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setLong(int parameterIndex, long x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setLong");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x, NativeDB.INSTANCE::set_long_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setFloat(int parameterIndex, float x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setFloat");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x, NativeDB.INSTANCE::set_float_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setDouble(int parameterIndex, double x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setDouble");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x, NativeDB.INSTANCE::set_double_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setBigDecimal(int parameterIndex, BigDecimal x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setBigDecimal");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setString(int parameterIndex, String x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setString");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x + "\0", NativeDB.INSTANCE::set_string_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setBytes(int parameterIndex, byte[] x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setBytes");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setDate(int parameterIndex, Date x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setDate");
        throwIfAlreadyClosed();
        try {
            var value = StatementHandler.convert(x);
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, value, NativeDB.INSTANCE::set_date_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setTime(int parameterIndex, Time x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setTime");
        throwIfAlreadyClosed();
        try {
            var value = StatementHandler.convert(x);
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, value, NativeDB.INSTANCE::set_time_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setTimestamp(int parameterIndex, Timestamp x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setTimestamp");
        throwIfAlreadyClosed();
        try {
            var value = StatementHandler.convert(x);
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, value, NativeDB.INSTANCE::set_timestamp_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setAsciiStream(int parameterIndex, InputStream x, int length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setAsciiStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setUnicodeStream(int parameterIndex, InputStream x, int length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setUnicodeStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setBinaryStream(int parameterIndex, InputStream x, int length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setBinaryStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void clearParameters() throws SQLException {
        log.finest("NanodbcPreparedStatement.clearParameters");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setObject(int parameterIndex, Object x, int targetSqlType) throws SQLException {
        log.finest("NanodbcPreparedStatement.setObject");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setObject(int parameterIndex, Object x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setObject");
        throwIfAlreadyClosed();
        if (x == null) {
            setNull(parameterIndex, Types.OTHER);
            return;
        }

        if (x instanceof String s) {
            setString(parameterIndex, s);
        } else if (x instanceof Integer i) {
            setInt(parameterIndex, i);
        } else if (x instanceof Long l) {
            setLong(parameterIndex, l);
        } else if (x instanceof Short s) {
            setShort(parameterIndex, s);
        } else if (x instanceof Byte b) {
            setShort(parameterIndex, b);
        } else if (x instanceof Double d) {
            setDouble(parameterIndex, d);
        } else if (x instanceof Float f) {
            setDouble(parameterIndex, f);
        } else if (x instanceof Boolean b) {
            setBoolean(parameterIndex, b);
        } else if (x instanceof Date date) {
            setDate(parameterIndex, date);
        } else if (x instanceof Time time) {
            setTime(parameterIndex, time);
        } else if (x instanceof Timestamp ts) {
            setTimestamp(parameterIndex, ts);
        } else if (x instanceof byte[] bytes) {
            setBytes(parameterIndex, bytes);
        } else {
            // Fallback: toString()
            setString(parameterIndex, x.toString());
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean execute() throws SQLException {
        log.finest("NanodbcPreparedStatement.execute");
        throwIfAlreadyClosed();
        try {
            assert connection.get() != null;
            ResultSetPtr resultSetPtr = StatementHandler.execute(statementPtr);
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
    public void addBatch() throws SQLException {
        log.finest("NanodbcPreparedStatement.addBatch");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setCharacterStream(int parameterIndex, Reader reader, int length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setCharacterStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setRef(int parameterIndex, Ref x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setRef");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setBlob(int parameterIndex, Blob x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setBlob");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setClob(int parameterIndex, Clob x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setClob");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setArray(int parameterIndex, Array x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setArray");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSetMetaData getMetaData() throws SQLException {
        log.finest("NanodbcPreparedStatement.getMetaData");
        throwIfAlreadyClosed();
        if (resultSet == null || resultSet.isClosed()) {
            throw new NanodbcSQLException("ResultSet is null or already closed");
        }
        return resultSet.getMetaData();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setDate(int parameterIndex, Date x, Calendar cal) throws SQLException {
        log.finest("NanodbcPreparedStatement.setDate");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setTime(int parameterIndex, Time x, Calendar cal) throws SQLException {
        log.finest("NanodbcPreparedStatement.setTime");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setTimestamp(int parameterIndex, Timestamp x, Calendar cal) throws SQLException {
        log.finest("NanodbcPreparedStatement.setTimestamp");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setNull(int parameterIndex, int sqlType, String typeName) throws SQLException {
        log.finest("NanodbcPreparedStatement.setNull");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setURL(int parameterIndex, URL x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setURL");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ParameterMetaData getParameterMetaData() throws SQLException {
        log.finest("NanodbcPreparedStatement.getParameterMetaData");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setRowId(int parameterIndex, RowId x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setRowId");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setNString(int parameterIndex, String value) throws SQLException {
        log.finest("NanodbcPreparedStatement.setNString");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setNCharacterStream(int parameterIndex, Reader value, long length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setNCharacterStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setNClob(int parameterIndex, NClob value) throws SQLException {
        log.finest("NanodbcPreparedStatement.setNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setClob(int parameterIndex, Reader reader, long length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setBlob(int parameterIndex, InputStream inputStream, long length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setNClob(int parameterIndex, Reader reader, long length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setSQLXML(int parameterIndex, SQLXML xmlObject) throws SQLException {
        log.finest("NanodbcPreparedStatement.setSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setObject(int parameterIndex, Object x, int targetSqlType, int scaleOrLength) throws SQLException {
        log.finest("NanodbcPreparedStatement.setObject");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setAsciiStream(int parameterIndex, InputStream x, long length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setBinaryStream(int parameterIndex, InputStream x, long length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setCharacterStream(int parameterIndex, Reader reader, long length) throws SQLException {
        log.finest("NanodbcPreparedStatement.setCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setAsciiStream(int parameterIndex, InputStream x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setBinaryStream(int parameterIndex, InputStream x) throws SQLException {
        log.finest("NanodbcPreparedStatement.setBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setCharacterStream(int parameterIndex, Reader reader) throws SQLException {
        log.finest("NanodbcPreparedStatement.setCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setNCharacterStream(int parameterIndex, Reader value) throws SQLException {
        log.finest("NanodbcPreparedStatement.setNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setClob(int parameterIndex, Reader reader) throws SQLException {
        log.finest("NanodbcPreparedStatement.setClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setBlob(int parameterIndex, InputStream inputStream) throws SQLException {
        log.finest("NanodbcPreparedStatement.setBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setNClob(int parameterIndex, Reader reader) throws SQLException {
        log.finest("NanodbcPreparedStatement.setNClob");
        throw new SQLFeatureNotSupportedException();
    }
}
