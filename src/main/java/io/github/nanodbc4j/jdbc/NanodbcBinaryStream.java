package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.internal.NativeDB;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.pointer.BinaryStreamPtr;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;
import lombok.AllArgsConstructor;
import lombok.extern.java.Log;

import java.io.IOException;
import java.io.InputStream;
import java.lang.ref.Cleaner;

import static io.github.nanodbc4j.internal.handler.Handler.NUL_CHAR;
import static io.github.nanodbc4j.internal.handler.Handler.throwIfNativeError;

public class NanodbcBinaryStream extends InputStream {
    private final BinaryStreamPtr streamPtr;
    private volatile boolean closed = false;

    // Cleaner for managing resource cleanup
    private static final Cleaner cleaner = Cleaner.create();
    private final Cleaner.Cleanable cleanable;

    public NanodbcBinaryStream(ResultSetPtr resultSetPtr, int columnIndex) {
        NativeError error = new NativeError();
        try {
            streamPtr = NativeDB.INSTANCE.get_binary_stream_by_index(resultSetPtr, columnIndex - 1, error);
            cleanable = cleaner.register(this, new BinaryStreamCleaner(streamPtr));
            throwIfNativeError(error);
        } finally {
            NativeDB.INSTANCE.clear_native_error(error);
        }
    }

    public NanodbcBinaryStream(ResultSetPtr resultSetPtr, String columnName) {
        NativeError error = new NativeError();
        try {
            streamPtr = NativeDB.INSTANCE.get_binary_stream_by_name(resultSetPtr, columnName + NUL_CHAR, error);
            cleanable = cleaner.register(this, new BinaryStreamCleaner(streamPtr));
            throwIfNativeError(error);
        } finally {
            NativeDB.INSTANCE.clear_native_error(error);
        }
    }

    @Override
    public int read() throws IOException {
        byte[] buffer = new byte[1];
        int bytesRead = read(buffer, 0, 1);
        return bytesRead == -1 ? -1 : buffer[0] & 0xFF;
    }

    @Override
    public int read(byte[] buffer, int offset, int length) throws IOException {
        if (closed) throw new IOException("Stream closed");
        if (buffer == null) throw new NullPointerException();

        NativeError error = new NativeError();
        try {
            int bytesRead = NativeDB.INSTANCE.read_binary_stream(streamPtr, buffer, offset, length, error);
            throwIfNativeError(error);
            return bytesRead;
        } catch (Exception e) {
            throw new IOException("Failed to read from binary stream", e);
        } finally {
            NativeDB.INSTANCE.clear_native_error(error);
        }
    }

    @Override
    public void close() throws IOException {
        synchronized (this) {
            if (!closed) {
                cleanable.clean();
                closed = true;
            }
        }
    }

    @Log
    @AllArgsConstructor
    private static class BinaryStreamCleaner implements Runnable {
        private BinaryStreamPtr ptr;

        @Override
        public void run() {
            if (ptr != null) {
                try {
                    NativeDB.INSTANCE.close_binary_stream(ptr);
                } catch (Exception e) {
                    log.warning("Exception while closing binary stream: " + e.getMessage());
                } finally {
                    ptr = null;
                }
            }
        }
    }
}