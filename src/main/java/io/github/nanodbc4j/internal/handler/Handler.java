package io.github.nanodbc4j.internal.handler;

import com.sun.jna.Platform;
import com.sun.jna.Pointer;
import io.github.nanodbc4j.exceptions.NativeException;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import lombok.experimental.UtilityClass;

/**
 * Utility handler
 */
@UtilityClass
public class Handler {
    public static final char NUL_CHAR = '\0';
    public static final String NUL_TERMINATOR = "" + NUL_CHAR;

    public static String getWideString(Pointer p) {
        if (p == null || p.equals(Pointer.NULL)) {
            return null;
        }
        return p.getWideString(0);
    }

    public static void throwIfNativeError(NativeError nativeError) {
        if (nativeError.error_code != 0) {
            throw new NativeException(nativeError);
        }
    }

    @FunctionalInterface
    public interface TriFunction<T, U, V, R> {
        R apply(T t, U u, V v);
    }

    @FunctionalInterface
    public interface QuadConsumer<T, U, V, W> {
        void accept(T t, U u, V v, W w);
    }
}
