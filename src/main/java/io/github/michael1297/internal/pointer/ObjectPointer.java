package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import io.github.michael1297.internal.NativeDB;

public final class ObjectPointer extends PointerType implements AutoCloseable {
    private volatile boolean closed = false;

    public ObjectPointer() {
    }

    public ObjectPointer(Pointer p) {
        super(p);
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
