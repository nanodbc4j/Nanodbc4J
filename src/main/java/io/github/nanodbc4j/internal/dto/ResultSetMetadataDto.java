package io.github.nanodbc4j.internal.dto;

import lombok.EqualsAndHashCode;
import lombok.ToString;

import java.util.ArrayList;

/**
 * Immutable container for column metadata from ODBC.
 */
@ToString
@EqualsAndHashCode
public final class ResultSetMetadataDto {
    public int columnCount = 0;
    public ArrayList<ColumnMetaData> columnMetaData = null;

    @ToString
    @EqualsAndHashCode
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
    }
}
