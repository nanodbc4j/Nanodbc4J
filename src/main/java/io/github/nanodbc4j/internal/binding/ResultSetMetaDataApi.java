package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Library;
import com.sun.jna.Native;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.cstruct.ResultSetMetaDataStruct;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;

public interface ResultSetMetaDataApi extends Library {
    ResultSetMetaDataApi INSTANCE = initialize();

    private static ResultSetMetaDataApi initialize() {
        try {
            LibraryLoader.load();
            return Native.load(ResultSetMetaDataApi.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    ResultSetMetaDataStruct get_meta_data(ResultSetPtr results, NativeError error);

    void delete_meta_data(ResultSetMetaDataStruct meta_data);
}
