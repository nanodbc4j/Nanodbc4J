package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public class OdbcDatabaseMetaDataPrt  extends PointerType {
    public OdbcDatabaseMetaDataPrt(){
    }

    public OdbcDatabaseMetaDataPrt(Pointer p){
        super(p);
    }
}
