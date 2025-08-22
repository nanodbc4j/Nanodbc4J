package io.github.michael1297.internal.handler;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import io.github.michael1297.jdbc.SpdLogLevel;
import io.github.michael1297.jdbc.metadata.OdbcDatasource;
import io.github.michael1297.jdbc.metadata.OdbcDriver;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.struct.DatasourceStruct;
import io.github.michael1297.internal.struct.DriverStruct;

import java.util.ArrayList;
import java.util.List;

/**
 * Main native interface. Loads odbc.dll and exposes ODBC functions via JNA.
 */
public final class Handler {
    public static final long POINTER_SIZE = System.getProperty("os.arch").endsWith("64") ? 8 : 4;

    // Static methods only
    private Handler() {
    }

    public static void setLogLevel(SpdLogLevel level) {
        NativeDB.INSTANCE.set_log_level(level.getValue());
    }

    public static List<OdbcDriver> driversList() {
        List<OdbcDriver> drivers = new ArrayList<>();
        IntByReference count = new IntByReference();
        Pointer driversListPtrs = NativeDB.INSTANCE.drivers_list(count);
        try {
            for (int i = 0; i < count.getValue(); i++) {
                DriverStruct ds = new DriverStruct(driversListPtrs.getPointer(POINTER_SIZE * i));
                List<OdbcDriver.Attribute> driverAttributes = new ArrayList<>(ds.attribute_count);
                String name = ds.name.getWideString(0);

                for (int j = 0; j < ds.attribute_count; j++) {
                    DriverStruct.AttributeStruct attr = new DriverStruct.AttributeStruct(ds.attributes.getPointer(POINTER_SIZE * j));
                    String keyword = attr.keyword.getWideString(0);
                    String value = attr.value.getWideString(0);
                    driverAttributes.add(new OdbcDriver.Attribute(keyword, value));
                }

                drivers.add(new OdbcDriver(name, driverAttributes));
            }
        } finally {
            NativeDB.INSTANCE.delete_driver_array(driversListPtrs, count.getValue());
        }
        return drivers;
    }

    public static List<OdbcDatasource> datasourcesList() {
        List<OdbcDatasource> datasources = new ArrayList<>();
        IntByReference count = new IntByReference();
        Pointer datasourcesListPtrs = NativeDB.INSTANCE.datasources_list(count);
        try {
            for (int i = 0; i < count.getValue(); i++) {
                DatasourceStruct ds = new DatasourceStruct(datasourcesListPtrs.getPointer(POINTER_SIZE * i));
                String name = ds.name.getWideString(0);
                String driver = ds.driver.getWideString(0);
                datasources.add(new OdbcDatasource(name, driver));
            }
        } finally {
            NativeDB.INSTANCE.delete_datasource_array(datasourcesListPtrs, count.getValue());
        }

        return datasources;
    }
}
