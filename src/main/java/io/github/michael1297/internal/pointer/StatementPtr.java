package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import lombok.NoArgsConstructor;

/**
 * nanodbc::statement pointer
 */
@NoArgsConstructor
public final class StatementPtr extends PointerType {
    public StatementPtr(Pointer p) {
        super(p);
    }
}
