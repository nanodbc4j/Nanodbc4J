package io.github.michael1297.internal.cstruct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;

@Structure.FieldOrder({"name", "attributes", "attribute_count"})
public final class DriverStruct extends Structure {
    public Pointer name;
    public Pointer attributes;
    public int attribute_count;

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

    @Structure.FieldOrder({"keyword", "value"})
    public static class AttributeStruct extends Structure {
        public Pointer keyword; ///< Driver keyword attribute.
        public Pointer value;   ///< Driver attribute value.

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
    }
}
