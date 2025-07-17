package io.github.michael1297.core;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;

@Structure.FieldOrder({"name", "driver"})
public class DatasourcePtr extends Structure {
    public Pointer name;
    public Pointer driver;

    public DatasourcePtr(){
    }

    public DatasourcePtr(Pointer p) {
        super(p);
        read();
    }

    // Публичный метод для установки указателя
    public void setPointer(Pointer p) {
        useMemory(p);
        read();
    }
}
