package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import io.github.nanodbc4j.internal.cstruct.DatasourceStruct;
import io.github.nanodbc4j.internal.cstruct.DriverStruct;
import io.github.nanodbc4j.internal.cstruct.NativeError;

public interface OdbcApi extends Library {
    OdbcApi INSTANCE = initialize();

    private static OdbcApi initialize() {
        try {
            LibraryLoader.load();
            return Native.load(OdbcApi.class, LibraryLoader.getNativeLibraryOptions());
        } catch (Throwable t) {
            throw new ExceptionInInitializerError(t);
        }
    }

    Pointer drivers_list(IntByReference count);

    Pointer datasources_list(IntByReference count);

    void std_free(Pointer ptr);

    void clear_native_error(NativeError error);

    int set_log_level(int level);

    void delete_datasource(DatasourceStruct datasource);

    void delete_datasource_array(Pointer datasource_array, int size);

    void delete_driver(DriverStruct driver);

    void delete_driver_array(Pointer driver_array, int size);
}
