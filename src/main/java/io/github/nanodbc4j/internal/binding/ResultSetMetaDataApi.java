package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Library;
import com.sun.jna.Native;
import io.github.nanodbc4j.internal.cstruct.NativeError;
import io.github.nanodbc4j.internal.cstruct.ResultSetMetaDataStruct;
import io.github.nanodbc4j.internal.pointer.ResultSetPtr;

/**
 * JNA interface for ODBC result set metadata operations.
 * Maps to native ODBC result set metadata retrieval functions.
 */
public interface ResultSetMetaDataApi extends Library {
    ResultSetMetaDataApi INSTANCE = initialize();

    private static ResultSetMetaDataApi initialize() {
        try {
            String lib = LibraryLoader.load();
            return Native.load(lib, ResultSetMetaDataApi.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    /**
     * Gets result set metadata information.
     *
     * @param results result set pointer
     * @param error error information output
     * @return result set metadata structure
     */
    ResultSetMetaDataStruct get_meta_data(ResultSetPtr results, NativeError error);

    /**
     * Frees result set metadata resources.
     *
     * @param meta_data metadata structure to delete
     */
    void delete_meta_data(ResultSetMetaDataStruct meta_data);
}
