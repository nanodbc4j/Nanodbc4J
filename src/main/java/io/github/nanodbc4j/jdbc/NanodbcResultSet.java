package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.exceptions.NanodbcSQLException;
import io.github.nanodbc4j.exceptions.NativeException;
import io.github.nanodbc4j.internal.NativeDB;
import io.github.nanodbc4j.internal.handler.ResultSetHandler;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import lombok.extern.java.Log;

import java.io.InputStream;
import java.io.InvalidClassException;
import java.io.Reader;
import java.lang.ref.WeakReference;
import java.math.BigDecimal;
import java.net.URL;
import java.sql.Array;
import java.sql.Blob;
import java.sql.Clob;
import java.sql.Date;
import java.sql.NClob;
import java.sql.Ref;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.RowId;
import java.sql.SQLException;
import java.sql.SQLFeatureNotSupportedException;
import java.sql.SQLWarning;
import java.sql.SQLXML;
import java.sql.Statement;
import java.sql.Time;
import java.sql.Timestamp;
import java.util.Calendar;
import java.util.Map;
import java.util.UUID;

/**
 * Result set from a query. Iterate with next(), read data with getX() methods.
 */
@Log
public class NanodbcResultSet implements ResultSet {
    protected ResultSetPtr resultSetPtr;
    private final WeakReference<NanodbcStatement> statement;
    private ResultSetMetaData metaData = null;
    private boolean closed = false;
    private Object lastColumn = null;

    NanodbcResultSet(ResultSetPtr resultSetPtr) {
        this.resultSetPtr = resultSetPtr;
        this.statement = new WeakReference<>(null);
    }

    NanodbcResultSet(NanodbcStatement statement, ResultSetPtr resultSetPtr) {
        this.resultSetPtr = resultSetPtr;
        this.statement = new WeakReference<>(statement);
    }

    @Override
    public boolean next() throws SQLException {
        log.finest("next");
        throwIfAlreadyClosed();
        try {
            return ResultSetHandler.next(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public void close() throws SQLException {
        log.finest("close");
        try {
            ResultSetHandler.close(resultSetPtr);
            resultSetPtr = null;
            metaData = null;
            lastColumn = null;
            closed = true;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public boolean wasNull() throws SQLException {
        log.finest("wasNull");
        throwIfAlreadyClosed();
        try {
            return ResultSetHandler.wasNull(resultSetPtr, lastColumn);
        } catch (NativeException | InvalidClassException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public String getString(int columnIndex) throws SQLException {
        log.finest("getString");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getStringValueByIndex(resultSetPtr, columnIndex);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public boolean getBoolean(int columnIndex) throws SQLException {
        log.finest("getBoolean");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_bool_value_by_index) != 0;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public byte getByte(int columnIndex) throws SQLException {
        log.finest("getByte");
        return (byte) getShort(columnIndex);
    }

    @Override
    public short getShort(int columnIndex) throws SQLException {
        log.finest("getShort");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_short_value_by_index);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public int getInt(int columnIndex) throws SQLException {
        log.finest("getInt");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_int_value_by_index);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public long getLong(int columnIndex) throws SQLException {
        log.finest("getLong");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_long_value_by_index);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public float getFloat(int columnIndex) throws SQLException {
        log.finest("getFloat");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_float_value_by_index);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public double getDouble(int columnIndex) throws SQLException {
        log.finest("getDouble");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_double_value_by_index);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public BigDecimal getBigDecimal(int columnIndex, int scale) throws SQLException {
        log.finest("getBigDecimal");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public byte[] getBytes(int columnIndex) throws SQLException {
        log.finest("getBytes");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Date getDate(int columnIndex) throws SQLException {
        log.finest("getDate");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getDateValueByIndex(resultSetPtr, columnIndex);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public Time getTime(int columnIndex) throws SQLException {
        log.finest("getTime");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getTimeValueByIndex(resultSetPtr, columnIndex);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public Timestamp getTimestamp(int columnIndex) throws SQLException {
        log.finest("getTimestamp");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getTimestampValueByIndex(resultSetPtr, columnIndex);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public InputStream getAsciiStream(int columnIndex) throws SQLException {
        log.finest("getAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public InputStream getUnicodeStream(int columnIndex) throws SQLException {
        log.finest("getUnicodeStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public InputStream getBinaryStream(int columnIndex) throws SQLException {
        log.finest("getBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public String getString(String columnLabel) throws SQLException {
        log.finest("getString");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getStringValueByName(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public boolean getBoolean(String columnLabel) throws SQLException {
        log.finest("getBoolean");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_bool_value_by_name) != 0;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public byte getByte(String columnLabel) throws SQLException {
        log.finest("getByte");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public short getShort(String columnLabel) throws SQLException {
        log.finest("getShort");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_short_value_by_name);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public int getInt(String columnLabel) throws SQLException {
        log.finest("getInt");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_int_value_by_name);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public long getLong(String columnLabel) throws SQLException {
        log.finest("getLong");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_long_value_by_name);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public float getFloat(String columnLabel) throws SQLException {
        log.finest("getFloat");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_float_value_by_name);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public double getDouble(String columnLabel) throws SQLException {
        log.finest("getDouble");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_double_value_by_name);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public BigDecimal getBigDecimal(String columnLabel, int scale) throws SQLException {
        log.finest("getBigDecimal");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public byte[] getBytes(String columnLabel) throws SQLException {
        log.finest("getBytes");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Date getDate(String columnLabel) throws SQLException {
        log.finest("getDate");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getDateValueByName(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public Time getTime(String columnLabel) throws SQLException {
        log.finest("getTime");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getTimeValueByName(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public Timestamp getTimestamp(String columnLabel) throws SQLException {
        log.finest("getTimestamp");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getTimestampValueByName(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public InputStream getAsciiStream(String columnLabel) throws SQLException {
        log.finest("getAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public InputStream getUnicodeStream(String columnLabel) throws SQLException {
        log.finest("getUnicodeStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public InputStream getBinaryStream(String columnLabel) throws SQLException {
        log.finest("getBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public SQLWarning getWarnings() throws SQLException {
        log.finest("getWarnings");
        return null;
    }

    @Override
    public void clearWarnings() throws SQLException {
        log.finest("clearWarnings");

    }

    @Override
    public String getCursorName() throws SQLException {
        log.finest("getCursorName");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public ResultSetMetaData getMetaData() throws SQLException {
        log.finest("getMetaData");
        throwIfAlreadyClosed();
        try {
            if (metaData == null && !isClosed()) {
                metaData = ResultSetHandler.getResultSetMetaData(resultSetPtr);
            }
            return metaData;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public Object getObject(int columnIndex) throws SQLException {
        log.finest("getObject");
        throwIfAlreadyClosed();
        lastColumn = columnIndex;
        ResultSetMetaData metaData = getMetaData();
        String className = metaData.getColumnClassName(columnIndex);

        try {
            return switch (className) {
                case "java.lang.Boolean" -> getBoolean(columnIndex);
                case "java.lang.Byte" -> getByte(columnIndex);
                case "java.lang.Short" -> getShort(columnIndex);
                case "java.lang.Integer" -> getInt(columnIndex);
                case "java.lang.Long" -> getLong(columnIndex);
                case "java.lang.Float" -> getFloat(columnIndex);
                case "java.lang.Double" -> getDouble(columnIndex);
                case "java.math.BigDecimal" -> getBigDecimal(columnIndex);
                case "java.lang.String" -> getString(columnIndex);
                case "java.sql.Date" -> getDate(columnIndex);
                case "java.sql.Time" -> getTime(columnIndex);
                case "java.sql.Timestamp" -> getTimestamp(columnIndex);
                case "[B" -> // byte array
                        getBytes(columnIndex);
                case "java.util.UUID" -> {
                    String uuidString = getString(columnIndex);
                    try {
                        yield uuidString != null ? UUID.fromString(uuidString) : null;
                    } catch (IllegalArgumentException e) {
                        throw new SQLException("Invalid UUID format: " + uuidString, e);
                    }
                }
                default -> getString(columnIndex); // Fallback
            };
        } catch (SQLException e) {
            throw new SQLException("Error getting object for column " + columnIndex + " (class: " + className + ")", e);
        }
    }

    @Override
    public Object getObject(String columnLabel) throws SQLException {
        log.finest("getObject");
        throwIfAlreadyClosed();
        int index = findColumn(columnLabel);
        lastColumn = columnLabel;
        if (index == -1) {
            throw new SQLException("Column " + columnLabel + " not found");
        }
        return getObject(index);
    }

    @Override
    public int findColumn(String columnLabel) throws SQLException {
        log.finest("findColumn");
        throwIfAlreadyClosed();
        try {
            return ResultSetHandler.findColumn(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    @Override
    public Reader getCharacterStream(int columnIndex) throws SQLException {
        log.finest("getCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Reader getCharacterStream(String columnLabel) throws SQLException {
        log.finest("getCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public BigDecimal getBigDecimal(int columnIndex) throws SQLException {
        log.finest("getBigDecimal");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public BigDecimal getBigDecimal(String columnLabel) throws SQLException {
        log.finest("getBigDecimal");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean isBeforeFirst() throws SQLException {
        log.finest("isBeforeFirst");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean isAfterLast() throws SQLException {
        log.finest("isAfterLast");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean isFirst() throws SQLException {
        log.finest("isFirst");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean isLast() throws SQLException {
        log.finest("isLast");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void beforeFirst() throws SQLException {
        log.finest("beforeFirst");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void afterLast() throws SQLException {
        log.finest("afterLast");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean first() throws SQLException {
        log.finest("first");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean last() throws SQLException {
        log.finest("last");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getRow() throws SQLException {
        log.finest("getRow");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean absolute(int row) throws SQLException {
        log.finest("absolute");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean relative(int rows) throws SQLException {
        log.finest("relative");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean previous() throws SQLException {
        log.finest("previous");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setFetchDirection(int direction) throws SQLException {
        log.finest("setFetchDirection");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getFetchDirection() throws SQLException {
        log.finest("getFetchDirection");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void setFetchSize(int rows) throws SQLException {
        log.finest("setFetchSize");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getFetchSize() throws SQLException {
        log.finest("getFetchSize");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getType() throws SQLException {
        log.finest("getType");
        return ResultSet.TYPE_FORWARD_ONLY;
        //throw new SQLFeatureNotSupportedException(); todo
    }

    @Override
    public int getConcurrency() throws SQLException {
        log.finest("getConcurrency");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean rowUpdated() throws SQLException {
        log.finest("rowUpdated");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean rowInserted() throws SQLException {
        log.finest("rowInserted");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean rowDeleted() throws SQLException {
        log.finest("rowDeleted");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNull(int columnIndex) throws SQLException {
        log.finest("updateNull");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBoolean(int columnIndex, boolean x) throws SQLException {
        log.finest("updateBoolean");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateByte(int columnIndex, byte x) throws SQLException {
        log.finest("updateByte");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateShort(int columnIndex, short x) throws SQLException {
        log.finest("updateShort");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateInt(int columnIndex, int x) throws SQLException {
        log.finest("updateInt");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateLong(int columnIndex, long x) throws SQLException {
        log.finest("updateLong");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateFloat(int columnIndex, float x) throws SQLException {
        log.finest("updateFloat");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateDouble(int columnIndex, double x) throws SQLException {
        log.finest("updateDouble");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBigDecimal(int columnIndex, BigDecimal x) throws SQLException {
        log.finest("updateBigDecimal");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateString(int columnIndex, String x) throws SQLException {
        log.finest("updateString");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBytes(int columnIndex, byte[] x) throws SQLException {
        log.finest("updateBytes");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateDate(int columnIndex, Date x) throws SQLException {
        log.finest("updateDate");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateTime(int columnIndex, Time x) throws SQLException {
        log.finest("updateTime");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateTimestamp(int columnIndex, Timestamp x) throws SQLException {
        log.finest("updateTimestamp");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateAsciiStream(int columnIndex, InputStream x, int length) throws SQLException {
        log.finest("updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBinaryStream(int columnIndex, InputStream x, int length) throws SQLException {
        log.finest("updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateCharacterStream(int columnIndex, Reader x, int length) throws SQLException {
        log.finest("updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateObject(int columnIndex, Object x, int scaleOrLength) throws SQLException {
        log.finest("updateObject");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateObject(int columnIndex, Object x) throws SQLException {
        log.finest("updateObject");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNull(String columnLabel) throws SQLException {
        log.finest("updateNull");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBoolean(String columnLabel, boolean x) throws SQLException {
        log.finest("updateBoolean");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateByte(String columnLabel, byte x) throws SQLException {
        log.finest("updateByte");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateShort(String columnLabel, short x) throws SQLException {
        log.finest("updateShort");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateInt(String columnLabel, int x) throws SQLException {
        log.finest("updateInt");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateLong(String columnLabel, long x) throws SQLException {
        log.finest("updateLong");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateFloat(String columnLabel, float x) throws SQLException {
        log.finest("updateFloat");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateDouble(String columnLabel, double x) throws SQLException {
        log.finest("updateDouble");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBigDecimal(String columnLabel, BigDecimal x) throws SQLException {
        log.finest("updateBigDecimal");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateString(String columnLabel, String x) throws SQLException {
        log.finest("updateString");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBytes(String columnLabel, byte[] x) throws SQLException {
        log.finest("updateBytes");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateDate(String columnLabel, Date x) throws SQLException {
        log.finest("updateDate");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateTime(String columnLabel, Time x) throws SQLException {
        log.finest("updateTime");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateTimestamp(String columnLabel, Timestamp x) throws SQLException {
        log.finest("updateTimestamp");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateAsciiStream(String columnLabel, InputStream x, int length) throws SQLException {
        log.finest("updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBinaryStream(String columnLabel, InputStream x, int length) throws SQLException {
        log.finest("updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateCharacterStream(String columnLabel, Reader reader, int length) throws SQLException {
        log.finest("updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateObject(String columnLabel, Object x, int scaleOrLength) throws SQLException {
        log.finest("updateObject");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateObject(String columnLabel, Object x) throws SQLException {
        log.finest("updateObject");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void insertRow() throws SQLException {
        log.finest("insertRow");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateRow() throws SQLException {
        log.finest("updateRow");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void deleteRow() throws SQLException {
        log.finest("deleteRow");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void refreshRow() throws SQLException {
        log.finest("refreshRow");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void cancelRowUpdates() throws SQLException {
        log.finest("cancelRowUpdates");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void moveToInsertRow() throws SQLException {
        log.finest("moveToInsertRow");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void moveToCurrentRow() throws SQLException {
        log.finest("moveToCurrentRow");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Statement getStatement() throws SQLException {
        log.finest("getStatement");
        throwIfAlreadyClosed();
        return statement.get();
    }

    @Override
    public Object getObject(int columnIndex, Map<String, Class<?>> map) throws SQLException {
        log.finest("getObject");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Ref getRef(int columnIndex) throws SQLException {
        log.finest("getRef");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Blob getBlob(int columnIndex) throws SQLException {
        log.finest("getBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Clob getClob(int columnIndex) throws SQLException {
        log.finest("getClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Array getArray(int columnIndex) throws SQLException {
        log.finest("getArray");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Object getObject(String columnLabel, Map<String, Class<?>> map) throws SQLException {
        log.finest("getObject");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Ref getRef(String columnLabel) throws SQLException {
        log.finest("getRef");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Blob getBlob(String columnLabel) throws SQLException {
        log.finest("getBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Clob getClob(String columnLabel) throws SQLException {
        log.finest("getClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Array getArray(String columnLabel) throws SQLException {
        log.finest("getArray");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Date getDate(int columnIndex, Calendar cal) throws SQLException {
        log.finest("getDate");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Date getDate(String columnLabel, Calendar cal) throws SQLException {
        log.finest("getDate");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Time getTime(int columnIndex, Calendar cal) throws SQLException {
        log.finest("getTime");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Time getTime(String columnLabel, Calendar cal) throws SQLException {
        log.finest("getTime");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Timestamp getTimestamp(int columnIndex, Calendar cal) throws SQLException {
        log.finest("getTimestamp");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Timestamp getTimestamp(String columnLabel, Calendar cal) throws SQLException {
        log.finest("getTimestamp");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public URL getURL(int columnIndex) throws SQLException {
        log.finest("getURL");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public URL getURL(String columnLabel) throws SQLException {
        log.finest("getURL");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateRef(int columnIndex, Ref x) throws SQLException {
        log.finest("updateRef");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateRef(String columnLabel, Ref x) throws SQLException {
        log.finest("updateRef");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBlob(int columnIndex, Blob x) throws SQLException {
        log.finest("updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBlob(String columnLabel, Blob x) throws SQLException {
        log.finest("updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateClob(int columnIndex, Clob x) throws SQLException {
        log.finest("updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateClob(String columnLabel, Clob x) throws SQLException {
        log.finest("updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateArray(int columnIndex, Array x) throws SQLException {
        log.finest("updateArray");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateArray(String columnLabel, Array x) throws SQLException {
        log.finest("updateArray");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public RowId getRowId(int columnIndex) throws SQLException {
        log.finest("getRowId");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public RowId getRowId(String columnLabel) throws SQLException {
        log.finest("getRowId");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateRowId(int columnIndex, RowId x) throws SQLException {
        log.finest("updateRowId");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateRowId(String columnLabel, RowId x) throws SQLException {
        log.finest("updateRowId");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public int getHoldability() throws SQLException {
        log.finest("getHoldability");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public boolean isClosed() throws SQLException {
        log.finest("isClosed");
        return closed;
    }

    @Override
    public void updateNString(int columnIndex, String nString) throws SQLException {
        log.finest("updateNString");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNString(String columnLabel, String nString) throws SQLException {
        log.finest("updateNString");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNClob(int columnIndex, NClob nClob) throws SQLException {
        log.finest("updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNClob(String columnLabel, NClob nClob) throws SQLException {
        log.finest("updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public NClob getNClob(int columnIndex) throws SQLException {
        log.finest("getNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public NClob getNClob(String columnLabel) throws SQLException {
        log.finest("getNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public SQLXML getSQLXML(int columnIndex) throws SQLException {
        log.finest("getSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public SQLXML getSQLXML(String columnLabel) throws SQLException {
        log.finest("getSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateSQLXML(int columnIndex, SQLXML xmlObject) throws SQLException {
        log.finest("updateSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateSQLXML(String columnLabel, SQLXML xmlObject) throws SQLException {
        log.finest("updateSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public String getNString(int columnIndex) throws SQLException {
        log.finest("getNString");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public String getNString(String columnLabel) throws SQLException {
        log.finest("getNString");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Reader getNCharacterStream(int columnIndex) throws SQLException {
        log.finest("getNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public Reader getNCharacterStream(String columnLabel) throws SQLException {
        log.finest("getNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNCharacterStream(int columnIndex, Reader x, long length) throws SQLException {
        log.finest("updateNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNCharacterStream(String columnLabel, Reader reader, long length) throws SQLException {
        log.finest("updateNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateAsciiStream(int columnIndex, InputStream x, long length) throws SQLException {
        log.finest("updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBinaryStream(int columnIndex, InputStream x, long length) throws SQLException {
        log.finest("updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateCharacterStream(int columnIndex, Reader x, long length) throws SQLException {
        log.finest("updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateAsciiStream(String columnLabel, InputStream x, long length) throws SQLException {
        log.finest("updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBinaryStream(String columnLabel, InputStream x, long length) throws SQLException {
        log.finest("updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateCharacterStream(String columnLabel, Reader reader, long length) throws SQLException {
        log.finest("updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBlob(int columnIndex, InputStream inputStream, long length) throws SQLException {
        log.finest("updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBlob(String columnLabel, InputStream inputStream, long length) throws SQLException {
        log.finest("updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateClob(int columnIndex, Reader reader, long length) throws SQLException {
        log.finest("updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateClob(String columnLabel, Reader reader, long length) throws SQLException {
        log.finest("updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNClob(int columnIndex, Reader reader, long length) throws SQLException {
        log.finest("updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNClob(String columnLabel, Reader reader, long length) throws SQLException {
        log.finest("updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNCharacterStream(int columnIndex, Reader x) throws SQLException {
        log.finest("updateNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNCharacterStream(String columnLabel, Reader reader) throws SQLException {
        log.finest("updateNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateAsciiStream(int columnIndex, InputStream x) throws SQLException {
        log.finest("updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBinaryStream(int columnIndex, InputStream x) throws SQLException {
        log.finest("updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateCharacterStream(int columnIndex, Reader x) throws SQLException {
        log.finest("updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateAsciiStream(String columnLabel, InputStream x) throws SQLException {
        log.finest("updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBinaryStream(String columnLabel, InputStream x) throws SQLException {
        log.finest("updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateCharacterStream(String columnLabel, Reader reader) throws SQLException {
        log.finest("updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBlob(int columnIndex, InputStream inputStream) throws SQLException {
        log.finest("updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateBlob(String columnLabel, InputStream inputStream) throws SQLException {
        log.finest("updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateClob(int columnIndex, Reader reader) throws SQLException {
        log.finest("updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateClob(String columnLabel, Reader reader) throws SQLException {
        log.finest("updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNClob(int columnIndex, Reader reader) throws SQLException {
        log.finest("updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public void updateNClob(String columnLabel, Reader reader) throws SQLException {
        log.finest("updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public <T> T getObject(int columnIndex, Class<T> type) throws SQLException {
        log.finest("getObject");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public <T> T getObject(String columnLabel, Class<T> type) throws SQLException {
        log.finest("getObject");
        throw new SQLFeatureNotSupportedException();
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        log.finest("unwrap");
        if (isWrapperFor(iface)) {
            return iface.cast(this);
        }
        throw new NanodbcSQLException("Cannot unwrap to " + iface.getName());
    }

    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {
        log.finest("isWrapperFor");
        return iface.isInstance(this) || iface == ResultSet.class;
    }

    /**
     * Throws exception if ResultSet is already closed.
     *
     * @throws SQLException if ResultSet is already closed.
     */
    protected void throwIfAlreadyClosed() throws SQLException {
        if (isClosed() || resultSetPtr == null) {
            throw new NanodbcSQLException("ResultSet: already closed");
        }
    }
}
