package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import io.github.michael1297.internal.NativeDB;

public final class StringPtr extends PointerType implements AutoCloseable {
    private volatile boolean closed = false;

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

    @Override
    public void close() {
        if (closed) return;

        if(getPointer() != null) {
            NativeDB.INSTANCE.std_free(getPointer());
        }

        closed = true;
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            close();
        } finally {
            super.finalize();
        }
    }
}
