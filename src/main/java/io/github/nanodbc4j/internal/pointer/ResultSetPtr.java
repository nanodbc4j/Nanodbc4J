package io.github.nanodbc4j.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import lombok.NoArgsConstructor;

/**
 * nanodbc::result pointer
 */
@NoArgsConstructor
public final class ResultSetPtr extends PointerType {
    public ResultSetPtr(Pointer p) {
        super(p);
    }
}
