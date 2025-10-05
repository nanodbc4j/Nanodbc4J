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

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean next() throws SQLException {
        log.finest("NanodbcResultSet.next");
        throwIfAlreadyClosed();
        try {
            return ResultSetHandler.next(resultSetPtr);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void close() throws SQLException {
        log.finest("NanodbcResultSet.close");
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

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean wasNull() throws SQLException {
        log.finest("NanodbcResultSet.wasNull");
        throwIfAlreadyClosed();
        try {
            return ResultSetHandler.wasNull(resultSetPtr, lastColumn);
        } catch (NativeException | InvalidClassException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getString(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getString");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getStringValueByIndex(resultSetPtr, columnIndex);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean getBoolean(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getBoolean");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_bool_value_by_index) != 0;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public byte getByte(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getByte");
        return (byte) getShort(columnIndex);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public short getShort(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getShort");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_short_value_by_index);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getInt(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getInt");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_int_value_by_index);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public long getLong(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getLong");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_long_value_by_index);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public float getFloat(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getFloat");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_float_value_by_index);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public double getDouble(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getDouble");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getValueByIndex(resultSetPtr, columnIndex, NativeDB.INSTANCE::get_double_value_by_index);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public BigDecimal getBigDecimal(int columnIndex, int scale) throws SQLException {
        log.finest("NanodbcResultSet.getBigDecimal");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public byte[] getBytes(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getBytes");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getBytesByIndex(resultSetPtr, columnIndex);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Date getDate(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getDate");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getDateValueByIndex(resultSetPtr, columnIndex);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Time getTime(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getTime");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getTimeValueByIndex(resultSetPtr, columnIndex);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Timestamp getTimestamp(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getTimestamp");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnIndex;
            return ResultSetHandler.getTimestampValueByIndex(resultSetPtr, columnIndex);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public InputStream getAsciiStream(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public InputStream getUnicodeStream(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getUnicodeStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public InputStream getBinaryStream(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getBinaryStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getString(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getString");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getStringValueByName(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean getBoolean(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getBoolean");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_bool_value_by_name) != 0;
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public byte getByte(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getByte");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public short getShort(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getShort");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_short_value_by_name);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getInt(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getInt");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_int_value_by_name);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public long getLong(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getLong");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_long_value_by_name);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public float getFloat(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getFloat");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_float_value_by_name);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public double getDouble(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getDouble");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getValueByName(resultSetPtr, columnLabel, NativeDB.INSTANCE::get_double_value_by_name);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public BigDecimal getBigDecimal(String columnLabel, int scale) throws SQLException {
        log.finest("NanodbcResultSet.getBigDecimal");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public byte[] getBytes(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getBytes");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getBytesByName(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Date getDate(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getDate");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getDateValueByName(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Time getTime(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getTime");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getTimeValueByName(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Timestamp getTimestamp(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getTimestamp");
        throwIfAlreadyClosed();
        try {
            lastColumn = columnLabel;
            return ResultSetHandler.getTimestampValueByName(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public InputStream getAsciiStream(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getAsciiStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public InputStream getUnicodeStream(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getUnicodeStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public InputStream getBinaryStream(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getBinaryStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public SQLWarning getWarnings() throws SQLException {
        log.finest("NanodbcResultSet.getWarnings");
        return null;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void clearWarnings() throws SQLException {
        log.finest("NanodbcResultSet.clearWarnings");
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getCursorName() throws SQLException {
        log.finest("NanodbcResultSet.getCursorName");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public ResultSetMetaData getMetaData() throws SQLException {
        log.finest("NanodbcResultSet.getMetaData");
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

    /**
     * {@inheritDoc}
     */
    @Override
    public Object getObject(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getObject");
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

    /**
     * {@inheritDoc}
     */
    @Override
    public Object getObject(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getObject");
        throwIfAlreadyClosed();
        int index = findColumn(columnLabel);
        lastColumn = columnLabel;
        if (index == -1) {
            throw new SQLException("Column " + columnLabel + " not found");
        }
        return getObject(index);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int findColumn(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.findColumn");
        throwIfAlreadyClosed();
        try {
            return ResultSetHandler.findColumn(resultSetPtr, columnLabel);
        } catch (NativeException e) {
            throw new NanodbcSQLException(e);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Reader getCharacterStream(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getCharacterStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Reader getCharacterStream(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getCharacterStream");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public BigDecimal getBigDecimal(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getBigDecimal");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public BigDecimal getBigDecimal(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getBigDecimal");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isBeforeFirst() throws SQLException {
        log.finest("NanodbcResultSet.isBeforeFirst");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isAfterLast() throws SQLException {
        log.finest("NanodbcResultSet.isAfterLast");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isFirst() throws SQLException {
        log.finest("NanodbcResultSet.isFirst");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isLast() throws SQLException {
        log.finest("NanodbcResultSet.isLast");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void beforeFirst() throws SQLException {
        log.finest("NanodbcResultSet.beforeFirst");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void afterLast() throws SQLException {
        log.finest("NanodbcResultSet.afterLast");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean first() throws SQLException {
        log.finest("NanodbcResultSet.first");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean last() throws SQLException {
        log.finest("NanodbcResultSet.last");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getRow() throws SQLException {
        log.finest("NanodbcResultSet.getRow");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean absolute(int row) throws SQLException {
        log.finest("NanodbcResultSet.absolute");
        log.warning("throw SQLFeatureNotSupportedException");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean relative(int rows) throws SQLException {
        log.finest("NanodbcResultSet.relative");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean previous() throws SQLException {
        log.finest("NanodbcResultSet.previous");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setFetchDirection(int direction) throws SQLException {
        log.finest("NanodbcResultSet.setFetchDirection");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getFetchDirection() throws SQLException {
        log.finest("NanodbcResultSet.getFetchDirection");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void setFetchSize(int rows) throws SQLException {
        log.finest("NanodbcResultSet.setFetchSize");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getFetchSize() throws SQLException {
        log.finest("NanodbcResultSet.getFetchSize");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getType() throws SQLException {
        log.finest("NanodbcResultSet.getType");
        return ResultSet.TYPE_FORWARD_ONLY;
        //throw new SQLFeatureNotSupportedException(); todo
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getConcurrency() throws SQLException {
        log.finest("NanodbcResultSet.getConcurrency");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean rowUpdated() throws SQLException {
        log.finest("NanodbcResultSet.rowUpdated");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean rowInserted() throws SQLException {
        log.finest("NanodbcResultSet.rowInserted");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean rowDeleted() throws SQLException {
        log.finest("NanodbcResultSet.rowDeleted");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNull(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.updateNull");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBoolean(int columnIndex, boolean x) throws SQLException {
        log.finest("NanodbcResultSet.updateBoolean");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateByte(int columnIndex, byte x) throws SQLException {
        log.finest("NanodbcResultSet.updateByte");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateShort(int columnIndex, short x) throws SQLException {
        log.finest("NanodbcResultSet.updateShort");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateInt(int columnIndex, int x) throws SQLException {
        log.finest("NanodbcResultSet.updateInt");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateLong(int columnIndex, long x) throws SQLException {
        log.finest("NanodbcResultSet.updateLong");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateFloat(int columnIndex, float x) throws SQLException {
        log.finest("NanodbcResultSet.updateFloat");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateDouble(int columnIndex, double x) throws SQLException {
        log.finest("NanodbcResultSet.updateDouble");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBigDecimal(int columnIndex, BigDecimal x) throws SQLException {
        log.finest("NanodbcResultSet.updateBigDecimal");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateString(int columnIndex, String x) throws SQLException {
        log.finest("NanodbcResultSet.updateString");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBytes(int columnIndex, byte[] x) throws SQLException {
        log.finest("NanodbcResultSet.updateBytes");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateDate(int columnIndex, Date x) throws SQLException {
        log.finest("NanodbcResultSet.updateDate");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateTime(int columnIndex, Time x) throws SQLException {
        log.finest("NanodbcResultSet.updateTime");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateTimestamp(int columnIndex, Timestamp x) throws SQLException {
        log.finest("NanodbcResultSet.updateTimestamp");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateAsciiStream(int columnIndex, InputStream x, int length) throws SQLException {
        log.finest("NanodbcResultSet.updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBinaryStream(int columnIndex, InputStream x, int length) throws SQLException {
        log.finest("NanodbcResultSet.updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateCharacterStream(int columnIndex, Reader x, int length) throws SQLException {
        log.finest("NanodbcResultSet.updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateObject(int columnIndex, Object x, int scaleOrLength) throws SQLException {
        log.finest("NanodbcResultSet.updateObject");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateObject(int columnIndex, Object x) throws SQLException {
        log.finest("NanodbcResultSet.updateObject");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNull(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.updateNull");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBoolean(String columnLabel, boolean x) throws SQLException {
        log.finest("NanodbcResultSet.updateBoolean");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateByte(String columnLabel, byte x) throws SQLException {
        log.finest("NanodbcResultSet.updateByte");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateShort(String columnLabel, short x) throws SQLException {
        log.finest("NanodbcResultSet.updateShort");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateInt(String columnLabel, int x) throws SQLException {
        log.finest("NanodbcResultSet.updateInt");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateLong(String columnLabel, long x) throws SQLException {
        log.finest("NanodbcResultSet.updateLong");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateFloat(String columnLabel, float x) throws SQLException {
        log.finest("NanodbcResultSet.updateFloat");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateDouble(String columnLabel, double x) throws SQLException {
        log.finest("NanodbcResultSet.updateDouble");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBigDecimal(String columnLabel, BigDecimal x) throws SQLException {
        log.finest("NanodbcResultSet.updateBigDecimal");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateString(String columnLabel, String x) throws SQLException {
        log.finest("NanodbcResultSet.updateString");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBytes(String columnLabel, byte[] x) throws SQLException {
        log.finest("NanodbcResultSet.updateBytes");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateDate(String columnLabel, Date x) throws SQLException {
        log.finest("NanodbcResultSet.updateDate");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateTime(String columnLabel, Time x) throws SQLException {
        log.finest("NanodbcResultSet.updateTime");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateTimestamp(String columnLabel, Timestamp x) throws SQLException {
        log.finest("NanodbcResultSet.updateTimestamp");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateAsciiStream(String columnLabel, InputStream x, int length) throws SQLException {
        log.finest("NanodbcResultSet.updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBinaryStream(String columnLabel, InputStream x, int length) throws SQLException {
        log.finest("NanodbcResultSet.updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateCharacterStream(String columnLabel, Reader reader, int length) throws SQLException {
        log.finest("NanodbcResultSet.updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateObject(String columnLabel, Object x, int scaleOrLength) throws SQLException {
        log.finest("NanodbcResultSet.updateObject");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateObject(String columnLabel, Object x) throws SQLException {
        log.finest("NanodbcResultSet.updateObject");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void insertRow() throws SQLException {
        log.finest("NanodbcResultSet.insertRow");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateRow() throws SQLException {
        log.finest("NanodbcResultSet.updateRow");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void deleteRow() throws SQLException {
        log.finest("NanodbcResultSet.deleteRow");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void refreshRow() throws SQLException {
        log.finest("NanodbcResultSet.refreshRow");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void cancelRowUpdates() throws SQLException {
        log.finest("NanodbcResultSet.cancelRowUpdates");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void moveToInsertRow() throws SQLException {
        log.finest("NanodbcResultSet.moveToInsertRow");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void moveToCurrentRow() throws SQLException {
        log.finest("NanodbcResultSet.moveToCurrentRow");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Statement getStatement() throws SQLException {
        log.finest("NanodbcResultSet.getStatement");
        throwIfAlreadyClosed();
        return statement.get();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Object getObject(int columnIndex, Map<String, Class<?>> map) throws SQLException {
        log.finest("NanodbcResultSet.getObject");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Ref getRef(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getRef");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Blob getBlob(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Clob getClob(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Array getArray(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getArray");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Object getObject(String columnLabel, Map<String, Class<?>> map) throws SQLException {
        log.finest("NanodbcResultSet.getObject");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Ref getRef(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getRef");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Blob getBlob(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Clob getClob(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Array getArray(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getArray");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Date getDate(int columnIndex, Calendar cal) throws SQLException {
        log.finest("NanodbcResultSet.getDate");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Date getDate(String columnLabel, Calendar cal) throws SQLException {
        log.finest("NanodbcResultSet.getDate");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Time getTime(int columnIndex, Calendar cal) throws SQLException {
        log.finest("NanodbcResultSet.getTime");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Time getTime(String columnLabel, Calendar cal) throws SQLException {
        log.finest("NanodbcResultSet.getTime");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Timestamp getTimestamp(int columnIndex, Calendar cal) throws SQLException {
        log.finest("NanodbcResultSet.getTimestamp");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Timestamp getTimestamp(String columnLabel, Calendar cal) throws SQLException {
        log.finest("NanodbcResultSet.getTimestamp");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public URL getURL(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getURL");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public URL getURL(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getURL");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateRef(int columnIndex, Ref x) throws SQLException {
        log.finest("NanodbcResultSet.updateRef");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateRef(String columnLabel, Ref x) throws SQLException {
        log.finest("NanodbcResultSet.updateRef");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBlob(int columnIndex, Blob x) throws SQLException {
        log.finest("NanodbcResultSet.updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBlob(String columnLabel, Blob x) throws SQLException {
        log.finest("NanodbcResultSet.updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateClob(int columnIndex, Clob x) throws SQLException {
        log.finest("NanodbcResultSet.updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateClob(String columnLabel, Clob x) throws SQLException {
        log.finest("NanodbcResultSet.updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateArray(int columnIndex, Array x) throws SQLException {
        log.finest("NanodbcResultSet.updateArray");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateArray(String columnLabel, Array x) throws SQLException {
        log.finest("NanodbcResultSet.updateArray");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public RowId getRowId(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getRowId");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public RowId getRowId(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getRowId");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateRowId(int columnIndex, RowId x) throws SQLException {
        log.finest("NanodbcResultSet.updateRowId");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateRowId(String columnLabel, RowId x) throws SQLException {
        log.finest("NanodbcResultSet.updateRowId");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getHoldability() throws SQLException {
        log.finest("NanodbcResultSet.getHoldability");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isClosed() throws SQLException {
        log.finest("NanodbcResultSet.isClosed");
        return closed;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNString(int columnIndex, String nString) throws SQLException {
        log.finest("NanodbcResultSet.updateNString");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNString(String columnLabel, String nString) throws SQLException {
        log.finest("NanodbcResultSet.updateNString");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNClob(int columnIndex, NClob nClob) throws SQLException {
        log.finest("NanodbcResultSet.updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNClob(String columnLabel, NClob nClob) throws SQLException {
        log.finest("NanodbcResultSet.updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public NClob getNClob(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public NClob getNClob(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public SQLXML getSQLXML(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public SQLXML getSQLXML(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateSQLXML(int columnIndex, SQLXML xmlObject) throws SQLException {
        log.finest("NanodbcResultSet.updateSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateSQLXML(String columnLabel, SQLXML xmlObject) throws SQLException {
        log.finest("NanodbcResultSet.updateSQLXML");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getNString(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getNString");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getNString(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getNString");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Reader getNCharacterStream(int columnIndex) throws SQLException {
        log.finest("NanodbcResultSet.getNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Reader getNCharacterStream(String columnLabel) throws SQLException {
        log.finest("NanodbcResultSet.getNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNCharacterStream(int columnIndex, Reader x, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNCharacterStream(String columnLabel, Reader reader, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateAsciiStream(int columnIndex, InputStream x, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBinaryStream(int columnIndex, InputStream x, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateCharacterStream(int columnIndex, Reader x, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateAsciiStream(String columnLabel, InputStream x, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBinaryStream(String columnLabel, InputStream x, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateCharacterStream(String columnLabel, Reader reader, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBlob(int columnIndex, InputStream inputStream, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBlob(String columnLabel, InputStream inputStream, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateClob(int columnIndex, Reader reader, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateClob(String columnLabel, Reader reader, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNClob(int columnIndex, Reader reader, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNClob(String columnLabel, Reader reader, long length) throws SQLException {
        log.finest("NanodbcResultSet.updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNCharacterStream(int columnIndex, Reader x) throws SQLException {
        log.finest("NanodbcResultSet.updateNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNCharacterStream(String columnLabel, Reader reader) throws SQLException {
        log.finest("NanodbcResultSet.updateNCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateAsciiStream(int columnIndex, InputStream x) throws SQLException {
        log.finest("NanodbcResultSet.updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBinaryStream(int columnIndex, InputStream x) throws SQLException {
        log.finest("NanodbcResultSet.updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateCharacterStream(int columnIndex, Reader x) throws SQLException {
        log.finest("NanodbcResultSet.updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateAsciiStream(String columnLabel, InputStream x) throws SQLException {
        log.finest("NanodbcResultSet.updateAsciiStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBinaryStream(String columnLabel, InputStream x) throws SQLException {
        log.finest("NanodbcResultSet.updateBinaryStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateCharacterStream(String columnLabel, Reader reader) throws SQLException {
        log.finest("NanodbcResultSet.updateCharacterStream");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBlob(int columnIndex, InputStream inputStream) throws SQLException {
        log.finest("NanodbcResultSet.updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateBlob(String columnLabel, InputStream inputStream) throws SQLException {
        log.finest("NanodbcResultSet.updateBlob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateClob(int columnIndex, Reader reader) throws SQLException {
        log.finest("NanodbcResultSet.updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateClob(String columnLabel, Reader reader) throws SQLException {
        log.finest("NanodbcResultSet.updateClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNClob(int columnIndex, Reader reader) throws SQLException {
        log.finest("NanodbcResultSet.updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void updateNClob(String columnLabel, Reader reader) throws SQLException {
        log.finest("NanodbcResultSet.updateNClob");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T getObject(int columnIndex, Class<T> type) throws SQLException {
        log.finest("NanodbcResultSet.getObject");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T getObject(String columnLabel, Class<T> type) throws SQLException {
        log.finest("NanodbcResultSet.getObject");
        throw new SQLFeatureNotSupportedException();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        log.finest("NanodbcResultSet.unwrap");
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
        log.finest("NanodbcResultSet.isWrapperFor");
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
