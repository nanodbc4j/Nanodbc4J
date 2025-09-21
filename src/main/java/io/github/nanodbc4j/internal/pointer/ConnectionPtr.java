package io.github.nanodbc4j.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import lombok.NoArgsConstructor;

/**
 * nanodbc::connection pointer
 */
@NoArgsConstructor
public final class ConnectionPtr extends PointerType {
    public ConnectionPtr(Pointer p) {
        super(p);
    }
}
