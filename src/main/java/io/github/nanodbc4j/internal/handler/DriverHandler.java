package io.github.nanodbc4j.internal.handler;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import io.github.nanodbc4j.internal.binding.OdbcApi;
import io.github.nanodbc4j.internal.dto.DatasourceProperties;
import io.github.nanodbc4j.internal.dto.DriverProperties;
import io.github.nanodbc4j.jdbc.SpdLogLevel;
import io.github.nanodbc4j.internal.cstruct.DatasourceStruct;
import io.github.nanodbc4j.internal.cstruct.DriverStruct;
import lombok.experimental.UtilityClass;

import java.util.ArrayList;
import java.util.List;

import static com.sun.jna.Native.POINTER_SIZE;
import static io.github.nanodbc4j.internal.handler.Handler.*;

/**
 * Native ODBC driver handler
 */
@UtilityClass
public final class DriverHandler {

    public static void setLogLevel(SpdLogLevel level) {
        OdbcApi.INSTANCE.set_log_level(level.getValue());
    }

    public static List<DriverProperties> driversList() {
        List<DriverProperties> drivers = new ArrayList<>();
        IntByReference count = new IntByReference();
        Pointer driversListPtrs = OdbcApi.INSTANCE.drivers_list(count);
        try {
            for (int i = 0; i < count.getValue(); i++) {
                long driversListOffset = (long) POINTER_SIZE * i;
                DriverStruct ds = new DriverStruct(driversListPtrs.getPointer(driversListOffset));
                List<DriverProperties.AttributeProperties> driverAttributes = new ArrayList<>(ds.attribute_count);
                String name = getUtf16String(ds.name);

                for (int j = 0; j < ds.attribute_count; j++) {
                    long attributeListOffset =  (long) POINTER_SIZE * j;
                    DriverStruct.AttributeStruct attr = new DriverStruct.AttributeStruct(ds.attributes.getPointer(attributeListOffset));
                    String keyword = getUtf16String(attr.keyword);
                    String value = getUtf16String(attr.value);
                    driverAttributes.add(new DriverProperties.AttributeProperties(keyword, value));
                }

                drivers.add(new DriverProperties(name, driverAttributes));
            }
        } finally {
            OdbcApi.INSTANCE.delete_driver_array(driversListPtrs, count.getValue());
        }
        return drivers;
    }

    public static List<DatasourceProperties> datasourcesList() {
        List<DatasourceProperties> datasources = new ArrayList<>();
        IntByReference count = new IntByReference();
        Pointer datasourcesListPtrs = OdbcApi.INSTANCE.datasources_list(count);
        try {
            for (int i = 0; i < count.getValue(); i++) {
                long datasourceListOffset = (long) POINTER_SIZE * i;
                DatasourceStruct ds = new DatasourceStruct(datasourcesListPtrs.getPointer(datasourceListOffset));
                String name = getUtf16String(ds.name);
                String driver = getUtf16String(ds.driver);
                datasources.add(new DatasourceProperties(name, driver));
            }
        } finally {
            OdbcApi.INSTANCE.delete_datasource_array(datasourcesListPtrs, count.getValue());
        }

        return datasources;
    }
}
