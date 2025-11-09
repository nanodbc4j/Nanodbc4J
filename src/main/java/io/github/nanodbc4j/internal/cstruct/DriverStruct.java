package io.github.nanodbc4j.internal.cstruct;

import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import lombok.NoArgsConstructor;

@NoArgsConstructor
@Structure.FieldOrder({"name", "attributes", "attribute_count"})
public final class DriverStruct extends Structure {
    public Pointer name;
    public Pointer attributes;
    public int attribute_count;

    public DriverStruct(Pointer p) {
        super(p);
        read();
    }

    public void setPointer(Pointer p) {
        useMemory(p);
        read();
    }

    @NoArgsConstructor
    @Structure.FieldOrder({"keyword", "value"})
    public static final class AttributeStruct extends Structure {
        public Pointer keyword; ///< Driver keyword attribute.
        public Pointer value;   ///< Driver attribute value.

        public AttributeStruct(Pointer p) {
            super(p);
            read();
        }

        public void setPointer(Pointer p) {
            useMemory(p);
            read();
        }
    }
}
