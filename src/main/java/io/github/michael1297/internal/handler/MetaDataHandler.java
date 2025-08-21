package io.github.michael1297.internal.handler;

import com.sun.jna.Pointer;
import io.github.michael1297.jdbc.metadata.MetaData;
import io.github.michael1297.internal.pointer.MetaDataPtr;
import io.github.michael1297.internal.struct.MetaDataStruct;

import java.util.ArrayList;

import static io.github.michael1297.internal.handler.Handler.POINTER_SIZE;

/**
 * Converts native ODBC metadata (pointer) to Java MetaData object.
 */
public final class MetaDataHandler {
    // Static methods only
    private MetaDataHandler(){
    }

    public static MetaData processerMetaData(MetaDataPtr metaDataPtr){
        if (metaDataPtr == null || metaDataPtr.getPointer() == null) {
            throw new IllegalArgumentException("MetaDataPtr is null");
        }

        MetaData metaData = new MetaData();
        MetaDataStruct metaDataStruct = new MetaDataStruct(metaDataPtr.getPointer());
        metaData.columnCount = metaDataStruct.columnCount;
        metaData.columnMetaData = new ArrayList<>(metaData.columnCount);

        for(int i = 0; i < metaDataStruct.columnCount; i++){
            var columnMetaData = new MetaData.ColumnMetaData();
            var columnMetaDataStruct = new MetaDataStruct.ColumnMetaDataStruct(metaDataStruct.column.getPointer(POINTER_SIZE * i));

            columnMetaData.isAutoIncrement       = columnMetaDataStruct.isAutoIncrement != 0;
            columnMetaData.isCaseSensitive       = columnMetaDataStruct.isCaseSensitive != 0;
            columnMetaData.isSearchable          = columnMetaDataStruct.isSearchable != 0;
            columnMetaData.isCurrency            = columnMetaDataStruct.isCurrency != 0;
            columnMetaData.isNullable            = columnMetaDataStruct.isNullable;
            columnMetaData.isSigned              = columnMetaDataStruct.isSigned != 0;
            columnMetaData.displaySize           = columnMetaDataStruct.displaySize;
            columnMetaData.precision             = columnMetaDataStruct.precision;
            columnMetaData.scale                 = columnMetaDataStruct.scale;
            columnMetaData.columnType            = columnMetaDataStruct.columnType;
            columnMetaData.isReadOnly            = columnMetaDataStruct.isReadOnly != 0;
            columnMetaData.isWritable            = columnMetaDataStruct.isWritable != 0;
            columnMetaData.isDefinitelyWritable  = columnMetaDataStruct.isDefinitelyWritable != 0;

            columnMetaData.columnLabel           = getWideString(columnMetaDataStruct.columnLabel);
            columnMetaData.columnName            = getWideString(columnMetaDataStruct.columnName);
            columnMetaData.schemaName            = getWideString(columnMetaDataStruct.schemaName);
            columnMetaData.tableName             = getWideString(columnMetaDataStruct.tableName);
            columnMetaData.catalogName           = getWideString(columnMetaDataStruct.catalogName);
            columnMetaData.columnTypeName        = getWideString(columnMetaDataStruct.columnTypeName);
            columnMetaData.columnClassName       = getWideString(columnMetaDataStruct.columnClassName);

            metaData.columnMetaData.add(columnMetaData);
        }

        return metaData;
    }

    private static String getWideString(Pointer p){
        if(p == null || p.equals(Pointer.NULL)) {
            return null;
        }
        return p.getWideString(0);
    }
}
