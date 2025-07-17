package io.github.michael1297.core.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public class StatementPtr extends PointerType {
    public StatementPtr() {
    }

    public StatementPtr(Pointer p) {
        super(p);
    }
}
