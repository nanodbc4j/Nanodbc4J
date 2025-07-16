package io.github.michael1297.core;

import com.sun.jna.Pointer;

import java.util.ArrayList;
import java.util.List;

public class NativeMemory implements AutoCloseable {
    private final List<Pointer> pointers = new ArrayList<>();

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

    public static boolean isNull(Pointer ptr) {
        return ptr == null || ptr.equals(Pointer.NULL);
    }
}
