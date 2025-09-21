package io.github.nanodbc4j.internal.handler;

import io.github.nanodbc4j.internal.dto.ResultSetMetadataDto;
import io.github.nanodbc4j.internal.cstruct.ResultSetMetaDataStruct;
import lombok.NonNull;
import lombok.experimental.UtilityClass;

import java.util.ArrayList;

import static io.github.nanodbc4j.internal.handler.Handler.*;

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
}
