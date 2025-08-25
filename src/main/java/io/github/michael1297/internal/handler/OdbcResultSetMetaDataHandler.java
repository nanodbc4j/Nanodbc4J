package io.github.michael1297.internal.handler;

import com.sun.jna.Pointer;
import io.github.michael1297.jdbc.metadata.OdbcResultSetMetadata;
import io.github.michael1297.internal.pointer.OdbcResultSetMetaDataPtr;
import io.github.michael1297.internal.cstruct.ResultSetMetaDataStruct;

import java.util.ArrayList;

import static io.github.michael1297.internal.handler.Handler.POINTER_SIZE;

/**
 * Converts native ODBC metadata (pointer) to Java OdbcResultSetMetadata object.
 */
public final class OdbcResultSetMetaDataHandler {
    // Static methods only
    private OdbcResultSetMetaDataHandler() {
    }

    public static OdbcResultSetMetadata processerMetaData(OdbcResultSetMetaDataPtr metaDataPtr) {
        if (metaDataPtr == null || metaDataPtr.getPointer() == null) {
            throw new IllegalArgumentException("MetaDataPtr is null");
        }

        OdbcResultSetMetadata metaData = new OdbcResultSetMetadata();
        ResultSetMetaDataStruct metaDataStruct = new ResultSetMetaDataStruct(metaDataPtr.getPointer());
        metaData.columnCount = metaDataStruct.columnCount;
        metaData.columnMetaData = new ArrayList<>(metaData.columnCount);

        for (int i = 0; i < metaDataStruct.columnCount; i++) {
            var columnMetaData = new OdbcResultSetMetadata.ColumnMetaData();
            var columnMetaDataStruct = new ResultSetMetaDataStruct.ColumnMetaDataStruct(metaDataStruct.column.getPointer(POINTER_SIZE * i));

            columnMetaData.isAutoIncrement = columnMetaDataStruct.isAutoIncrement != 0;
            columnMetaData.isCaseSensitive = columnMetaDataStruct.isCaseSensitive != 0;
            columnMetaData.isSearchable = columnMetaDataStruct.isSearchable != 0;
            columnMetaData.isCurrency = columnMetaDataStruct.isCurrency != 0;
            columnMetaData.isNullable = columnMetaDataStruct.isNullable;
            columnMetaData.isSigned = columnMetaDataStruct.isSigned != 0;
            columnMetaData.displaySize = columnMetaDataStruct.displaySize;
            columnMetaData.precision = columnMetaDataStruct.precision;
            columnMetaData.scale = columnMetaDataStruct.scale;
            columnMetaData.columnType = columnMetaDataStruct.columnType;
            columnMetaData.isReadOnly = columnMetaDataStruct.isReadOnly != 0;
            columnMetaData.isWritable = columnMetaDataStruct.isWritable != 0;
            columnMetaData.isDefinitelyWritable = columnMetaDataStruct.isDefinitelyWritable != 0;

            columnMetaData.columnLabel = getWideString(columnMetaDataStruct.columnLabel);
            columnMetaData.columnName = getWideString(columnMetaDataStruct.columnName);
            columnMetaData.schemaName = getWideString(columnMetaDataStruct.schemaName);
            columnMetaData.tableName = getWideString(columnMetaDataStruct.tableName);
            columnMetaData.catalogName = getWideString(columnMetaDataStruct.catalogName);
            columnMetaData.columnTypeName = getWideString(columnMetaDataStruct.columnTypeName);
            columnMetaData.columnClassName = getWideString(columnMetaDataStruct.columnClassName);

            metaData.columnMetaData.add(columnMetaData);
        }

        return metaData;
    }

    private static String getWideString(Pointer p) {
        if (p == null || p.equals(Pointer.NULL)) {
            return null;
        }
        return p.getWideString(0);
    }
}
