package io.github.nanodbc4j.internal.binding.jni;

import io.github.nanodbc4j.internal.binding.LibraryLoader;

public class Native {

    static {
        try {
            LibraryLoader.load();
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    public static native String getUtf16String(long addr);

    /**
     * Frees memory allocated by native code.
     *
     * @param ptr pointer to free
     */
    public static native void std_free(long ptr);
}
