package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

/*
 * CResultSetMetaData pointer
 */
public final class OdbcResultSetMetaDataPtr extends PointerType {
    public OdbcResultSetMetaDataPtr() {
    }

    public OdbcResultSetMetaDataPtr(Pointer p) {
        super(p);
    }
}
