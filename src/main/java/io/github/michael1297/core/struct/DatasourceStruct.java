package io.github.michael1297.core.struct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;

@Structure.FieldOrder({"name", "driver"})
public class DatasourceStruct extends Structure {
    public Pointer name;
    public Pointer driver;

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
}
