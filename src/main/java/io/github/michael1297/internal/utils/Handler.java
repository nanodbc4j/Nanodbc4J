package io.github.michael1297.internal.utils;

import com.sun.jna.ptr.IntByReference;
import io.github.michael1297.core.metadata.Datasource;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.pointer.ArrayPtr;
import io.github.michael1297.internal.pointer.ObjectPointer;
import io.github.michael1297.internal.pointer.StringPtr;
import io.github.michael1297.internal.struct.DatasourceStruct;

import java.util.ArrayList;
import java.util.List;

public final class Handler {
    private Handler() {
    }

    public static List<String> driversList() {
        List<String> drivers = new ArrayList<>();
        IntByReference count = new IntByReference();

        try (ArrayPtr list = NativeDB.INSTANCE.drivers_list(count)) {
            for (int i = 0; i < count.getValue(); i++) {
                try (StringPtr pStr = list.getObjectPtr(i, StringPtr.class)) {
                    String str = pStr.getWideString(0);
                    drivers.add(str);
                }
            }
        }

        return drivers;
    }

    public static List<Datasource> datasourcesList() {
        List<Datasource> datasources = new ArrayList<>();
        IntByReference count = new IntByReference();

        try (ArrayPtr list = NativeDB.INSTANCE.datasources_list(count)) {
            for (int i = 0; i < count.getValue(); i++) {
                try (ObjectPointer pItem = list.getObjectPtr(i, ObjectPointer.class);
                     DatasourceStruct ds = new DatasourceStruct(pItem.getPointer())) {
                    String name = ds.name.getWideString(0);
                    String driver = ds.driver.getWideString(0);
                    datasources.add(new Datasource(name, driver));
                }
            }
        }

        return datasources;
    }
}
