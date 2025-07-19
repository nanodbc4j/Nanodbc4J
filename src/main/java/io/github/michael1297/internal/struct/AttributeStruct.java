package io.github.michael1297.internal.struct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import io.github.michael1297.internal.NativeDB;

@Structure.FieldOrder({"keyword", "value"})
public class AttributeStruct extends Structure implements AutoCloseable {
    public Pointer keyword; ///< Driver keyword attribute.
    public Pointer value;   ///< Driver attribute value.

    private volatile boolean closed = false;

    public AttributeStruct(){
    }

    public AttributeStruct(Pointer p) {
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

        if(keyword != null) {
            NativeDB.INSTANCE.std_free(keyword);
        }
        if (value != null) {
            NativeDB.INSTANCE.std_free(value);
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
