package io.github.michael1297.core.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public class ResultSetPtr extends PointerType {
    public ResultSetPtr() {
    }

    public ResultSetPtr(Pointer p) {
        super(p);
    }
}
