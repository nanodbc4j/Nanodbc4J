package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public final class StatementPtr extends PointerType {
    public StatementPtr() {
    }

    public StatementPtr(Pointer p) {
        super(p);
    }
}
