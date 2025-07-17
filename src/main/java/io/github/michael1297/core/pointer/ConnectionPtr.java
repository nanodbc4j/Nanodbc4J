package io.github.michael1297.core.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public class ConnectionPtr extends PointerType {
    public ConnectionPtr() {
    }

    public ConnectionPtr(Pointer p) {
        super(p);
    }
}
