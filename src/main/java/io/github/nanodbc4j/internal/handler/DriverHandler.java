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

import static io.github.nanodbc4j.internal.handler.Handler.*;

/**
 * Native ODBC driver handler
 */
@UtilityClass
public final class DriverHandler {

    public static void setLogLevel(SpdLogLevel level) {
        if (OdbcApi.INSTANCE.set_log_level(level.getValue()) != 0) {
            throw new InternalError("set_log_level failed");
        }
    }

    public static List<DriverProperties> driversList() {
        List<DriverProperties> drivers = new ArrayList<>();
        IntByReference count = new IntByReference();
        Pointer driversListPtrs = OdbcApi.INSTANCE.drivers_list(count);
        try {
            for (int i = 0; i < count.getValue(); i++) {
                DriverStruct ds = new DriverStruct(driversListPtrs.getPointer(POINTER_SIZE * i));
                List<DriverProperties.AttributeProperties> driverAttributes = new ArrayList<>(ds.attribute_count);
                String name = getWideString(ds.name);

                for (int j = 0; j < ds.attribute_count; j++) {
                    DriverStruct.AttributeStruct attr = new DriverStruct.AttributeStruct(ds.attributes.getPointer(POINTER_SIZE * j));
                    String keyword = getWideString(attr.keyword);
                    String value = getWideString(attr.value);
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
                DatasourceStruct ds = new DatasourceStruct(datasourcesListPtrs.getPointer(POINTER_SIZE * i));
                String name = getWideString(ds.name);
                String driver = getWideString(ds.driver);
                datasources.add(new DatasourceProperties(name, driver));
            }
        } finally {
            OdbcApi.INSTANCE.delete_datasource_array(datasourcesListPtrs, count.getValue());
        }

        return datasources;
    }
}
