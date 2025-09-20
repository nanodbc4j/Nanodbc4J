package io.github.michael1297.internal.handler;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import io.github.michael1297.internal.dto.DatasourceDto;
import io.github.michael1297.internal.dto.DriverDto;
import io.github.michael1297.jdbc.SpdLogLevel;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.cstruct.DatasourceStruct;
import io.github.michael1297.internal.cstruct.DriverStruct;
import lombok.experimental.UtilityClass;

import java.util.ArrayList;
import java.util.List;

import static io.github.michael1297.internal.handler.Handler.*;

/**
 * Native ODBC driver handler
 */
@UtilityClass
public final class DriverHandler {

    public static void setLogLevel(SpdLogLevel level) {
        if (NativeDB.INSTANCE.set_log_level(level.getValue()) != 0) {
            throw new InternalError("set_log_level failed");
        }
    }

    public static List<DriverDto> driversList() {
        List<DriverDto> drivers = new ArrayList<>();
        IntByReference count = new IntByReference();
        Pointer driversListPtrs = NativeDB.INSTANCE.drivers_list(count);
        try {
            for (int i = 0; i < count.getValue(); i++) {
                DriverStruct ds = new DriverStruct(driversListPtrs.getPointer(POINTER_SIZE * i));
                List<DriverDto.Attribute> driverAttributes = new ArrayList<>(ds.attribute_count);
                String name = getWideString(ds.name);

                for (int j = 0; j < ds.attribute_count; j++) {
                    DriverStruct.AttributeStruct attr = new DriverStruct.AttributeStruct(ds.attributes.getPointer(POINTER_SIZE * j));
                    String keyword = getWideString(attr.keyword);
                    String value = getWideString(attr.value);
                    driverAttributes.add(new DriverDto.Attribute(keyword, value));
                }

                drivers.add(new DriverDto(name, driverAttributes));
            }
        } finally {
            NativeDB.INSTANCE.delete_driver_array(driversListPtrs, count.getValue());
        }
        return drivers;
    }

    public static List<DatasourceDto> datasourcesList() {
        List<DatasourceDto> datasources = new ArrayList<>();
        IntByReference count = new IntByReference();
        Pointer datasourcesListPtrs = NativeDB.INSTANCE.datasources_list(count);
        try {
            for (int i = 0; i < count.getValue(); i++) {
                DatasourceStruct ds = new DatasourceStruct(datasourcesListPtrs.getPointer(POINTER_SIZE * i));
                String name = getWideString(ds.name);
                String driver = getWideString(ds.driver);
                datasources.add(new DatasourceDto(name, driver));
            }
        } finally {
            NativeDB.INSTANCE.delete_datasource_array(datasourcesListPtrs, count.getValue());
        }

        return datasources;
    }
}
