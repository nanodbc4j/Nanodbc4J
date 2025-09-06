package io.github.michael1297.internal.cstruct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import lombok.NoArgsConstructor;

@NoArgsConstructor
@Structure.FieldOrder({"name", "driver"})
public final class DatasourceStruct extends Structure {
    public Pointer name;
    public Pointer driver;

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
