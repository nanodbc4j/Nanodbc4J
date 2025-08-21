package io.github.michael1297.internal.struct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;

@Structure.FieldOrder ({"columnCount", "column"})
public final class MetaDataStruct extends Structure {

    public int columnCount;
    public Pointer column;

    public MetaDataStruct(){
    }

    public MetaDataStruct(Pointer p){
        super(p);
        read();
    }

    public void setPointer(Pointer p) {
        useMemory(p);
        read();
    }

    @Structure.FieldOrder({
            "isAutoIncrement",
            "isCaseSensitive",
            "isSearchable",
            "isCurrency",
            "isNullable",
            "isSigned",
            "displaySize",
            "precision",
            "scale",
            "columnType",
            "isReadOnly",
            "isWritable",
            "isDefinitelyWritable",
            "columnLabel",
            "columnName",
            "schemaName",
            "tableName",
            "catalogName",
            "columnTypeName",
            "columnClassName"
    })
    public static class ColumnMetaDataStruct extends Structure {
        public byte isAutoIncrement;
        public byte isCaseSensitive;
        public byte isSearchable;
        public byte isCurrency;
        public int isNullable;
        public byte isSigned;
        public int displaySize;
        public int precision;
        public int scale;
        public int columnType;
        public byte isReadOnly;
        public byte isWritable;
        public byte isDefinitelyWritable;
        public Pointer columnLabel;
        public Pointer columnName;
        public Pointer schemaName;
        public Pointer tableName;
        public Pointer catalogName;
        public Pointer columnTypeName;
        public Pointer columnClassName;

        public ColumnMetaDataStruct(){
        }

        public ColumnMetaDataStruct(Pointer p){
            super(p);
            read();
        }

        public void setPointer(Pointer p) {
            useMemory(p);
            read();
        }
    }
}
