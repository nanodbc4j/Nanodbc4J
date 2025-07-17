package io.github.michael1297.core.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public class ObjectPointer extends PointerType {
    public ObjectPointer() {
    }

    public ObjectPointer(Pointer p) {
        super(p);
    }
}
