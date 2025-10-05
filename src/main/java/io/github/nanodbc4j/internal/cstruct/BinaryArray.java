package io.github.nanodbc4j.internal.cstruct;

import com.sun.jna.Memory;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;

@Structure.FieldOrder({"data", "length"})
public class BinaryArray extends Structure {
    public Pointer data;    // int8_t* в C -> byte[] в Java
    public int length;      // int32_t в C

    public BinaryArray() {
        super();
    }

    public BinaryArray(Pointer peer) {
        super(peer);
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
        if (data != null && length > 0) {
            byte[] bytes = new byte[length];
            data.read(0, bytes, 0, length);
            return bytes;
        }
        return new byte[0];
    }

    public boolean isNull() {
        return data == null || length == 0;
    }
}
