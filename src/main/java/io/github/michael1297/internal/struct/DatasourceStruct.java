package io.github.michael1297.internal.struct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import io.github.michael1297.internal.NativeDB;

@Structure.FieldOrder({"name", "driver"})
public final class DatasourceStruct extends Structure implements AutoCloseable {
    public Pointer name;
    public Pointer driver;

    private volatile boolean closed = false;

    public DatasourceStruct(){
    }

    public DatasourceStruct(Pointer p) {
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
        if (driver != null) {
            NativeDB.INSTANCE.std_free(driver);
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
