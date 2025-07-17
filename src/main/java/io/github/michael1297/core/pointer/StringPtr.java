package io.github.michael1297.core.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

public class StringPtr extends PointerType {
    public StringPtr(Pointer p) {
        super(p);
    }

    public StringPtr() {
    }

    public String getWideString(long offset) {
        return getPointer().getWideString(offset);
    }

    public String getString(long offset) {
        return getPointer().getString(offset);
    }

    public String getString(long offset, String encoding) {
        return getPointer().getString(offset, encoding);
    }
}
