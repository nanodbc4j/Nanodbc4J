package io.github.michael1297.internal.struct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import io.github.michael1297.internal.NativeDB;

@Structure.FieldOrder({"name", "attributes", "attribute_count"})
public class DriverStruct extends Structure implements AutoCloseable {
    public Pointer name;
    public Pointer attributes;
    public int attribute_count;

    private volatile boolean closed = false;

    public DriverStruct(){
    }

    public DriverStruct(Pointer p) {
        super(p);
        read();
    }

    // Публичный метод для установки указателя
    public void setPointer(Pointer p) {
        useMemory(p);
        read();
    }

    @Override
    public void close() {
        if (closed) return;

        if(name != null) {
            NativeDB.INSTANCE.std_free(name);
        }
        if (attributes != null) {
            NativeDB.INSTANCE.std_free(attributes);
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
