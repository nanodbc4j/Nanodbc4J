package io.github.nanodbc4j.internal.handler;

import io.github.nanodbc4j.internal.dto.ResultSetMetadataDto;
import io.github.nanodbc4j.internal.cstruct.ResultSetMetaDataStruct;
import lombok.NonNull;
import lombok.experimental.UtilityClass;

import java.util.ArrayList;

import static com.sun.jna.Native.POINTER_SIZE;
import static io.github.nanodbc4j.internal.handler.Handler.getUtf16String;

/**
 * Converts native ODBC metadata (pointer) to Java OdbcResultSetMetadata object.
 */
@UtilityClass
public final class ResultSetMetaDataHandler {

    public static ResultSetMetadataDto processerMetaData(@NonNull ResultSetMetaDataStruct metaDataStruct) {
        ResultSetMetadataDto metaData = new ResultSetMetadataDto();
        metaData.columnCount = metaDataStruct.columnCount;
        metaData.columnMetaData = new ArrayList<>(metaData.columnCount);

        for (int i = 0; i < metaDataStruct.columnCount; i++) {
            var columnMetaData = new ResultSetMetadataDto.ColumnMetaData();
            long offset = (long) POINTER_SIZE * i;
            var columnMetaDataStruct = new ResultSetMetaDataStruct.ColumnMetaDataStruct(metaDataStruct.column.getPointer(offset));

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

            columnMetaData.columnLabel = getUtf16String(columnMetaDataStruct.columnLabel);
            columnMetaData.columnName = getUtf16String(columnMetaDataStruct.columnName);
            columnMetaData.schemaName = getUtf16String(columnMetaDataStruct.schemaName);
            columnMetaData.tableName = getUtf16String(columnMetaDataStruct.tableName);
            columnMetaData.catalogName = getUtf16String(columnMetaDataStruct.catalogName);
            columnMetaData.columnTypeName = getUtf16String(columnMetaDataStruct.columnTypeName);
            columnMetaData.columnClassName = getUtf16String(columnMetaDataStruct.columnClassName);

            metaData.columnMetaData.add(columnMetaData);
        }

        return metaData;
    }
}
