package io.github.michael1297.core;

import com.sun.jna.Pointer;

import java.util.Collection;
import java.util.HashSet;

public class PointerTracker implements AutoCloseable {
    private final Collection<Pointer> pointers = new HashSet<>();

    public Pointer track(Pointer ptr) {
        if (!isNull(ptr)) {
            pointers.add(ptr);
        }
        return ptr;
    }


    @Override
    public void close() {
        for (Pointer ptr : pointers) {
            NativeDB.INSTANCE.std_free(ptr);
        }
        pointers.clear();
    }

    private static boolean isNull(Pointer ptr) {
        return ptr == null || ptr.equals(Pointer.NULL);
    }
}
