package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public final class ObjectPointer extends PointerType {
    public ObjectPointer() {
    }

    public ObjectPointer(Pointer p) {
        super(p);
    }
}
