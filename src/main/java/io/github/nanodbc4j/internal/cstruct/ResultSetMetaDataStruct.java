package io.github.nanodbc4j.internal.cstruct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import lombok.NoArgsConstructor;

@NoArgsConstructor
@Structure.FieldOrder({"columnCount", "column"})
public final class ResultSetMetaDataStruct extends Structure {

    public int columnCount;
    public Pointer column;

    public ResultSetMetaDataStruct(Pointer p) {
        super(p);
        read();
    }

    public void setPointer(Pointer p) {
        useMemory(p);
        read();
    }

    @NoArgsConstructor
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
    public static final class ColumnMetaDataStruct extends Structure {
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

        public ColumnMetaDataStruct(Pointer p) {
            super(p);
            read();
        }

        public void setPointer(Pointer p) {
            useMemory(p);
            read();
        }
    }
}
