package io.github.michael1297.internal.pointer;

import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import io.github.michael1297.internal.NativeDB;

public final class ArrayPtr extends PointerType implements AutoCloseable {
    public static final int POINTER_SIZE = System.getProperty("os.arch").endsWith("64") ? 8 : 4;

    private volatile boolean closed = false;

    public ArrayPtr() {
    }

    public ArrayPtr(Pointer p) {
        super(p);
    }

    public Pointer getObjectPointer(long offset){
        return getPointer().getPointer(offset);
    }

    public <T extends PointerType> T getObjectPtr(long index, Class<T> clazz){
        Pointer ptr = getObjectPointer(index * POINTER_SIZE);
        try {
            // Используем конструктор с Pointer
            return clazz.getConstructor(Pointer.class).newInstance(ptr);
        } catch (Exception e) {
            throw new RuntimeException("Не удалось создать экземпляр " + clazz.getName(), e);
        }
    }

    @Override
    public void close() {
        if (closed) return;

        if(getPointer() != null) {
            NativeDB.INSTANCE.std_free(getPointer());
        }

        closed = true;
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            close();
        } finally {
            super.finalize();
        }
    }
}
