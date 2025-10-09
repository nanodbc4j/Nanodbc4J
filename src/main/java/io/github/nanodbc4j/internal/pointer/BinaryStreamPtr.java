package io.github.nanodbc4j.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import lombok.NoArgsConstructor;

/**
 * ChunkedBinaryStream pointer
 */
@NoArgsConstructor
public class BinaryStreamPtr extends PointerType {
    public BinaryStreamPtr(Pointer p) {
        super(p);
    }
}
