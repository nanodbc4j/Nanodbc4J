package io.github.michael1297.internal.handler;

import com.sun.jna.Pointer;
import io.github.michael1297.core.metadata.MetaData;
import io.github.michael1297.internal.pointer.MetaDataPtr;
import io.github.michael1297.internal.struct.MetaDataStruct;

import java.util.ArrayList;

import static io.github.michael1297.internal.handler.Handler.POINTER_SIZE;

public class MetaDataHandler {
    // Static methods only
    private MetaDataHandler(){
    }

    public static MetaData processerMetaData(MetaDataPtr metaDataPtr){
        MetaData metaData = new MetaData();
        MetaDataStruct metaDataStruct = new MetaDataStruct(metaDataPtr.getPointer());
        metaData.columnCount = metaDataStruct.columnCount;
        metaData.columnMetaData = new ArrayList<>(metaData.columnCount);

        for(int i = 0; i < metaDataStruct.columnCount; i++){
            var columnMetaData = new MetaData.ColumnMetaData();
            var columnMetaDataStruct = new MetaDataStruct.ColumnMetaDataStruct(metaDataStruct.column.getPointer(POINTER_SIZE * i));

            columnMetaData.isAutoIncrement       = columnMetaDataStruct.isAutoIncrement;
            columnMetaData.isCaseSensitive       = columnMetaDataStruct.isCaseSensitive;
            columnMetaData.isSearchable          = columnMetaDataStruct.isSearchable;
            columnMetaData.isCurrency            = columnMetaDataStruct.isCurrency;
            columnMetaData.isNullable            = columnMetaDataStruct.isNullable;
            columnMetaData.isSigned              = columnMetaDataStruct.isSigned;
            columnMetaData.displaySize           = columnMetaDataStruct.displaySize;
            columnMetaData.precision             = columnMetaDataStruct.precision;
            columnMetaData.scale                 = columnMetaDataStruct.scale;
            columnMetaData.columnType            = columnMetaDataStruct.columnType;
            columnMetaData.isReadOnly            = columnMetaDataStruct.isReadOnly;
            columnMetaData.isWritable            = columnMetaDataStruct.isWritable;
            columnMetaData.isDefinitelyWritable  = columnMetaDataStruct.isDefinitelyWritable;

            columnMetaData.columnLabel           = getWideString(columnMetaDataStruct.columnLabel);
            columnMetaData.columnName            = getWideString(columnMetaDataStruct.columnName);
            columnMetaData.schemaName            = getWideString(columnMetaDataStruct.schemaName);
            //columnMetaData.tableName             = getWideString(columnMetaDataStruct.tableName); TODO
            //columnMetaData.catalogName           = getWideString(columnMetaDataStruct.catalogName);
            //columnMetaData.columnTypeName        = getWideString(columnMetaDataStruct.columnTypeName);

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
