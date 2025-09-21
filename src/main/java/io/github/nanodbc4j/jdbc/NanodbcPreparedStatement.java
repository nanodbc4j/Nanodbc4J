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

    @Override
    public ResultSet executeQuery() throws SQLException {
        log.finest("executeQuery");
        throwIfAlreadyClosed();
        try {
            ResultSetPtr resultSetPtr = StatementHandler.execute(statementPtr);
            return new NanodbcResultSet(this, resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public int executeUpdate() throws SQLException {
        log.finest("executeUpdate");
        throwIfAlreadyClosed();
        try {
            return StatementHandler.executeUpdate(statementPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setNull(int parameterIndex, int sqlType) throws SQLException {
        log.finest("setNull");
        setString(parameterIndex, null);    // Используем String метод
    }

    @Override
    public void setBoolean(int parameterIndex, boolean x) throws SQLException {
        log.finest("setBoolean");
        throwIfAlreadyClosed();
        try {
            byte value = (byte) (x ? 1 : 0);
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, value, NativeDB.INSTANCE::set_bool_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setByte(int parameterIndex, byte x) throws SQLException {
        log.finest("setByte");
        throwIfAlreadyClosed();
        setShort(parameterIndex, (short) x);
    }

    @Override
    public void setShort(int parameterIndex, short x) throws SQLException {
        log.finest("setShort");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x, NativeDB.INSTANCE::set_short_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setInt(int parameterIndex, int x) throws SQLException {
        log.finest("setInt");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x, NativeDB.INSTANCE::set_int_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setLong(int parameterIndex, long x) throws SQLException {
        log.finest("setLong");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x, NativeDB.INSTANCE::set_long_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setFloat(int parameterIndex, float x) throws SQLException {
        log.finest("setFloat");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x, NativeDB.INSTANCE::set_float_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setDouble(int parameterIndex, double x) throws SQLException {
        log.finest("setDouble");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x, NativeDB.INSTANCE::set_double_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setBigDecimal(int parameterIndex, BigDecimal x) throws SQLException {
        log.finest("setBigDecimal");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setString(int parameterIndex, String x) throws SQLException {
        log.finest("setString");
        throwIfAlreadyClosed();
        try {
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, x + "\0", NativeDB.INSTANCE::set_string_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setBytes(int parameterIndex, byte[] x) throws SQLException {
        log.finest("setBytes");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setDate(int parameterIndex, Date x) throws SQLException {
        log.finest("setDate");
        throwIfAlreadyClosed();
        try {
            var value = StatementHandler.convert(x);
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, value, NativeDB.INSTANCE::set_date_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setTime(int parameterIndex, Time x) throws SQLException {
        log.finest("setTime");
        throwIfAlreadyClosed();
        try {
            var value = StatementHandler.convert(x);
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, value, NativeDB.INSTANCE::set_time_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setTimestamp(int parameterIndex, Timestamp x) throws SQLException {
        log.finest("setTimestamp");
        throwIfAlreadyClosed();
        try {
            var value = StatementHandler.convert(x);
            StatementHandler.setValueByIndex(statementPtr, parameterIndex, value, NativeDB.INSTANCE::set_timestamp_value);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void setAsciiStream(int parameterIndex, InputStream x, int length) throws SQLException {
        log.finest("setAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setUnicodeStream(int parameterIndex, InputStream x, int length) throws SQLException {
        log.finest("setUnicodeStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setBinaryStream(int parameterIndex, InputStream x, int length) throws SQLException {
        log.finest("setBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void clearParameters() throws SQLException {
        log.finest("clearParameters");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setObject(int parameterIndex, Object x, int targetSqlType) throws SQLException {
        log.finest("setObject");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setObject(int parameterIndex, Object x) throws SQLException {
        log.finest("setObject");
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

    @Override
    public boolean execute() throws SQLException {
        log.finest("execute");
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

    @Override
    public void addBatch() throws SQLException {
        log.finest("addBatch");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setCharacterStream(int parameterIndex, Reader reader, int length) throws SQLException {
        log.finest("setCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setRef(int parameterIndex, Ref x) throws SQLException {
        log.finest("setRef");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setBlob(int parameterIndex, Blob x) throws SQLException {
        log.finest("setBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setClob(int parameterIndex, Clob x) throws SQLException {
        log.finest("setClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setArray(int parameterIndex, Array x) throws SQLException {
        log.finest("setArray");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public ResultSetMetaData getMetaData() throws SQLException {
        log.finest("getMetaData");
        throwIfAlreadyClosed();
        if (resultSet == null || resultSet.isClosed()) {
            throw new NanodbcSQLException("ResultSet is null or already closed");
        }
        return resultSet.getMetaData();
    }

    @Override
    public void setDate(int parameterIndex, Date x, Calendar cal) throws SQLException {
        log.finest("setDate");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setTime(int parameterIndex, Time x, Calendar cal) throws SQLException {
        log.finest("setTime");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setTimestamp(int parameterIndex, Timestamp x, Calendar cal) throws SQLException {
        log.finest("setTimestamp");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setNull(int parameterIndex, int sqlType, String typeName) throws SQLException {
        log.finest("setNull");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setURL(int parameterIndex, URL x) throws SQLException {
        log.finest("setURL");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public ParameterMetaData getParameterMetaData() throws SQLException {
        log.finest("getParameterMetaData");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setRowId(int parameterIndex, RowId x) throws SQLException {
        log.finest("setRowId");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setNString(int parameterIndex, String value) throws SQLException {
        log.finest("setNString");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setNCharacterStream(int parameterIndex, Reader value, long length) throws SQLException {
        log.finest("setNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setNClob(int parameterIndex, NClob value) throws SQLException {
        log.finest("setNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setClob(int parameterIndex, Reader reader, long length) throws SQLException {
        log.finest("setClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setBlob(int parameterIndex, InputStream inputStream, long length) throws SQLException {
        log.finest("setBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setNClob(int parameterIndex, Reader reader, long length) throws SQLException {
        log.finest("setNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setSQLXML(int parameterIndex, SQLXML xmlObject) throws SQLException {
        log.finest("setSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setObject(int parameterIndex, Object x, int targetSqlType, int scaleOrLength) throws SQLException {
        log.finest("setObject");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setAsciiStream(int parameterIndex, InputStream x, long length) throws SQLException {
        log.finest("setAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setBinaryStream(int parameterIndex, InputStream x, long length) throws SQLException {
        log.finest("setBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setCharacterStream(int parameterIndex, Reader reader, long length) throws SQLException {
        log.finest("setCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setAsciiStream(int parameterIndex, InputStream x) throws SQLException {
        log.finest("setAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setBinaryStream(int parameterIndex, InputStream x) throws SQLException {
        log.finest("setBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setCharacterStream(int parameterIndex, Reader reader) throws SQLException {
        log.finest("setCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setNCharacterStream(int parameterIndex, Reader value) throws SQLException {
        log.finest("setNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setClob(int parameterIndex, Reader reader) throws SQLException {
        log.finest("setClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setBlob(int parameterIndex, InputStream inputStream) throws SQLException {
        log.finest("setBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setNClob(int parameterIndex, Reader reader) throws SQLException {
        log.finest("setNClob");
        throw new SQLFeatureNotSupportedException();
    }
}
