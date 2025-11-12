package io.github.nanodbc4j.internal.cstruct;

import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import lombok.NoArgsConstructor;

@NoArgsConstructor
@Structure.FieldOrder({"data", "length"})
public final class BinaryArray extends Structure {
    public Pointer data;    // int8_t* в C -> byte[] в Java
    public int length;      // int32_t в C

    public BinaryArray(Pointer peer) {
        super(peer);
        read();
    }

    public BinaryArray(byte[] bytes) {
        super();
        if (bytes != null) {
            this.data = new Memory(bytes.length);
            this.data.write(0, bytes, 0, bytes.length);
            this.length = bytes.length;
        } else {
            this.data = null;
            this.length = 0;
        }
    }

    public byte[] getBytes() {
        if(isNull()) {
            return null;
        }
        byte[] bytes = new byte[length];
        data.read(0, bytes, 0, length);
        return bytes;
    }

    public boolean isNull() {
        return data == null || data.equals(Pointer.NULL) || length <= 0;
    }
}
