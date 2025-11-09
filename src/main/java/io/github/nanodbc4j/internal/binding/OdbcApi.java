package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import io.github.nanodbc4j.internal.cstruct.DatasourceStruct;
import io.github.nanodbc4j.internal.cstruct.DriverStruct;
import io.github.nanodbc4j.internal.cstruct.NativeError;

/**
 * JNA interface for ODBC system operations.
 * Maps to native ODBC driver and datasource management functions.
 */
public interface OdbcApi extends Library {
    OdbcApi INSTANCE = initialize();

    private static OdbcApi initialize() {
        try {
            String library = LibraryLoader.load();
            return Native.load(library, OdbcApi.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    /**
     * Gets list of available ODBC drivers.
     *
     * @param count output parameter for number of drivers
     * @return pointer to driver array
     */
    Pointer drivers_list(IntByReference count);

    /**
     * Gets list of available ODBC data sources.
     *
     * @param count output parameter for number of data sources
     * @return pointer to datasource array
     */
    Pointer datasources_list(IntByReference count);

    /**
     * Frees memory allocated by native code.
     *
     * @param ptr pointer to free
     */
    void std_free(Pointer ptr);

    /**
     * Clears native error structure.
     *
     * @param error error structure to clear
     */
    void clear_native_error(NativeError error);

    /**
     * Sets logging level for native library.
     *
     * @param level log level to set
     * @return previous log level
     */
    int set_log_level(int level);

    /**
     * Frees datasource structure.
     *
     * @param datasource datasource to delete
     */
    void delete_datasource(DatasourceStruct datasource);

    /**
     * Frees datasource array.
     *
     * @param datasource_array pointer to datasource array
     * @param size array size
     */
    void delete_datasource_array(Pointer datasource_array, int size);

    /**
     * Frees driver structure.
     *
     * @param driver driver to delete
     */
    void delete_driver(DriverStruct driver);

    /**
     * Frees driver array.
     *
     * @param driver_array pointer to driver array
     * @param size array size
     */
    void delete_driver_array(Pointer driver_array, int size);
}
