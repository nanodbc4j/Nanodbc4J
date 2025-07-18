package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public final class ConnectionPtr extends PointerType {
    public ConnectionPtr() {
    }

    public ConnectionPtr(Pointer p) {
        super(p);
    }
}
