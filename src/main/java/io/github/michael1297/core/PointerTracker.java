package io.github.michael1297.core;

import com.sun.jna.Pointer;

import java.util.LinkedHashSet;
import java.util.Set;

public class PointerTracker implements AutoCloseable {
    private final Set<Pointer> pointers = new LinkedHashSet<>();

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
