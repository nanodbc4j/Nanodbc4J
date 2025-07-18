package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public final class ResultSetPtr extends PointerType {
    public ResultSetPtr() {
    }

    public ResultSetPtr(Pointer p) {
        super(p);
    }
}
