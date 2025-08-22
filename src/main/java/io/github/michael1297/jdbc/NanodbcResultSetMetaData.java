package io.github.michael1297.jdbc;

import io.github.michael1297.jdbc.metadata.OdbcResultSetMetadata;

import java.sql.ResultSetMetaData;
import java.sql.SQLException;

/**
 * Metadata about result columns: name, type, size, nullability, etc.
 */
public class NanodbcResultSetMetaData implements ResultSetMetaData {
    private final OdbcResultSetMetadata metaData;
    public NanodbcResultSetMetaData(OdbcResultSetMetadata metaData){
        this.metaData = metaData;
    }

    @Override
    public int getColumnCount() throws SQLException {
        return metaData.columnCount;
    }

    @Override
    public boolean isAutoIncrement(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isAutoIncrement;
    }

    @Override
    public boolean isCaseSensitive(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isCaseSensitive;
    }

    @Override
    public boolean isSearchable(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isSearchable;
    }

    @Override
    public boolean isCurrency(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isCurrency;
    }

    @Override
    public int isNullable(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isNullable;
    }

    @Override
    public boolean isSigned(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isSigned;
    }

    @Override
    public int getColumnDisplaySize(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).displaySize;
    }

    @Override
    public String getColumnLabel(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).columnLabel;
    }

    @Override
    public String getColumnName(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).columnName;
    }

    @Override
    public String getSchemaName(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).schemaName;
    }

    @Override
    public int getPrecision(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).precision;
    }

    @Override
    public int getScale(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).scale;
    }

    @Override
    public String getTableName(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).tableName;
    }

    @Override
    public String getCatalogName(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).catalogName;
    }

    @Override
    public int getColumnType(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).columnType;
    }

    @Override
    public String getColumnTypeName(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).columnTypeName;
    }

    @Override
    public boolean isReadOnly(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isReadOnly;
    }

    @Override
    public boolean isWritable(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isWritable;
    }

    @Override
    public boolean isDefinitelyWritable(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).isDefinitelyWritable;
    }

    @Override
    public String getColumnClassName(int column) throws SQLException {
        checkIndex(column);
        return metaData.columnMetaData.get(column - 1).columnClassName;
    }

    @Override
    public <T> T unwrap(Class<T> iface) throws SQLException {
        return null;
    }

    @Override
    public boolean isWrapperFor(Class<?> iface) throws SQLException {
        return false;
    }

    @Override
    public String toString() {
        return metaData.toString();
    }

    private void checkIndex (int index) throws SQLException {
        index -= 1;
        if (index < 0 || index > metaData.columnCount) {
            throw new SQLException("column out of range");
        }
    }
}
