package io.github.michael1297.jdbc.metadata;

import java.util.ArrayList;
import java.util.Objects;

/**
 * Immutable container for column metadata from ODBC.
 */
public final class OdbcResultSetMetadata {
    public int columnCount = 0;
    public ArrayList<ColumnMetaData> columnMetaData = null;

    public static final class ColumnMetaData {
        public boolean isAutoIncrement = false;
        public boolean isCaseSensitive = false;
        public boolean isSearchable = false;
        public boolean isCurrency = false;
        public int isNullable = 0;
        public boolean isSigned = false;
        public int displaySize = 0;
        public int precision = 0;
        public int scale = 0;
        public int columnType = 0;
        public boolean isReadOnly = false;
        public boolean isWritable = false;
        public boolean isDefinitelyWritable = false;
        public String columnLabel = null;
        public String columnName = null;
        public String schemaName = null;
        public String tableName = null;
        public String catalogName = null;
        public String columnTypeName = null;
        public String columnClassName = null;

        @Override
        public boolean equals(Object o) {
            if (o == null || getClass() != o.getClass()) return false;
            ColumnMetaData that = (ColumnMetaData) o;
            return isAutoIncrement == that.isAutoIncrement && isCaseSensitive == that.isCaseSensitive && isSearchable == that.isSearchable && isCurrency == that.isCurrency && isNullable == that.isNullable && isSigned == that.isSigned && displaySize == that.displaySize && precision == that.precision && scale == that.scale && columnType == that.columnType && isReadOnly == that.isReadOnly && isWritable == that.isWritable && isDefinitelyWritable == that.isDefinitelyWritable && Objects.equals(columnLabel, that.columnLabel) && Objects.equals(columnName, that.columnName) && Objects.equals(schemaName, that.schemaName) && Objects.equals(tableName, that.tableName) && Objects.equals(catalogName, that.catalogName) && Objects.equals(columnTypeName, that.columnTypeName);
        }

        @Override
        public int hashCode() {
            return Objects.hash(isAutoIncrement, isCaseSensitive, isSearchable, isCurrency, isNullable, isSigned, displaySize, precision, scale, columnType, isReadOnly, isWritable, isDefinitelyWritable, columnLabel, columnName, schemaName, tableName, catalogName, columnTypeName);
        }

        @Override
        public String toString() {
            return "ColumnMetaData{" +
                    "isAutoIncrement=" + isAutoIncrement +
                    ", isCaseSensitive=" + isCaseSensitive +
                    ", isSearchable=" + isSearchable +
                    ", isCurrency=" + isCurrency +
                    ", isNullable=" + isNullable +
                    ", isSigned=" + isSigned +
                    ", displaySize=" + displaySize +
                    ", precision=" + precision +
                    ", scale=" + scale +
                    ", columnType=" + columnType +
                    ", isReadOnly=" + isReadOnly +
                    ", isWritable=" + isWritable +
                    ", isDefinitelyWritable=" + isDefinitelyWritable +
                    ", columnLabel='" + columnLabel + '\'' +
                    ", columnName='" + columnName + '\'' +
                    ", schemaName='" + schemaName + '\'' +
                    ", tableName='" + tableName + '\'' +
                    ", catalogName='" + catalogName + '\'' +
                    ", columnTypeName='" + columnTypeName + '\'' +
                    ", columnClassName='" + columnClassName + '\'' +
                    '}';
        }
    }

    @Override
    public boolean equals(Object o) {
        if (o == null || getClass() != o.getClass()) return false;
        OdbcResultSetMetadata metaData = (OdbcResultSetMetadata) o;
        return columnCount == metaData.columnCount && Objects.equals(columnMetaData, metaData.columnMetaData);
    }

    @Override
    public int hashCode() {
        return Objects.hash(columnCount, columnMetaData);
    }

    @Override
    public String toString() {
        return "OdbcResultSetMetadata{" +
                "columnCount=" + columnCount +
                ", columnMetaData=" + columnMetaData +
                '}';
    }
}
