package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public final class MetaDataPtr extends PointerType {
    public MetaDataPtr(){
    }

    public MetaDataPtr(Pointer p){
        super(p);
    }
}
