package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.exceptions.NanodbcSQLException;
import io.github.nanodbc4j.internal.dto.ResultSetMetadataDto;
import lombok.extern.java.Log;

import java.sql.ResultSetMetaData;
import java.sql.SQLException;

/**
 * Metadata about result columns: name, type, size, nullability, etc.
 */
@Log
public class NanodbcResultSetMetaData implements ResultSetMetaData, JdbcWrapper {
    private final ResultSetMetadataDto metaData;

    public NanodbcResultSetMetaData(ResultSetMetadataDto metaData) {
        this.metaData = metaData;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getColumnCount() throws SQLException {
        log.finest("NanodbcResultSetMetaData.getColumnCount");
        return metaData.columnCount;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isAutoIncrement(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.isAutoIncrement");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isAutoIncrement;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isCaseSensitive(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.isCaseSensitive");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isCaseSensitive;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isSearchable(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.isSearchable");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isSearchable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isCurrency(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.isCurrency");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isCurrency;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int isNullable(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.isNullable");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isNullable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isSigned(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.isSigned");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isSigned;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getColumnDisplaySize(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getColumnDisplaySize");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).displaySize;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getColumnLabel(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getColumnLabel");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).columnLabel;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getColumnName(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getColumnName");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).columnName;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getSchemaName(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getSchemaName");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).schemaName;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getPrecision(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getPrecision");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).precision;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getScale(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getScale");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).scale;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getTableName(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getTableName");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).tableName;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getCatalogName(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getCatalogName");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).catalogName;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getColumnType(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getColumnType");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).columnType;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getColumnTypeName(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getColumnTypeName");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).columnTypeName;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isReadOnly(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.isReadOnly");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isReadOnly;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isWritable(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.isWritable");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isWritable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isDefinitelyWritable(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.isDefinitelyWritable");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isDefinitelyWritable;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public String getColumnClassName(int column) throws SQLException {
        log.finest("NanodbcResultSetMetaData.getColumnClassName");
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).columnClassName;
    }

    @Override
    public String toString() {
        log.finest("NanodbcResultSetMetaData.toString");
        return metaData.toString();
    }

    private void checkIndex(int index) throws SQLException {
        if (index <= 0 || index > metaData.columnCount) {
            throw new NanodbcSQLException("Column " + index + " out of range");
        }
    }
}
