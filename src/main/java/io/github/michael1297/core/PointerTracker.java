package io.github.michael1297.core;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;

import java.util.LinkedHashSet;
import java.util.Set;

public class PointerTracker implements AutoCloseable {
    private final Set<Pointer> pointers = new LinkedHashSet<>();

    public <T extends Pointer> T track(T ptr) {
        if (!isNull(ptr)) {
            pointers.add(ptr);
        }
        return ptr;
    }

    public <T extends PointerType> T track(T ptr) {
        track(ptr.getPointer());
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
