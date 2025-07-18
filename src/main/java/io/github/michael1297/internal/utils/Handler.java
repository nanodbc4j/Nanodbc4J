package io.github.michael1297.internal.utils;

import com.sun.jna.ptr.IntByReference;
import io.github.michael1297.core.metadata.Datasource;
import io.github.michael1297.internal.NativeDB;
import io.github.michael1297.internal.PointerTracker;
import io.github.michael1297.internal.pointer.ArrayPtr;
import io.github.michael1297.internal.pointer.ObjectPointer;
import io.github.michael1297.internal.pointer.StringPtr;
import io.github.michael1297.internal.struct.DatasourceStruct;

import java.util.ArrayList;
import java.util.List;

public final class Handler {
    private Handler() {
    }

    public static List<String> driversList(){
        try (PointerTracker memory = new PointerTracker()) {
            IntByReference count = new IntByReference();
            ArrayPtr list = memory.track(NativeDB.INSTANCE.drivers_list(count));

            List<String> drivers = new ArrayList<>();

            for (int i = 0; i < count.getValue(); i++) {
                StringPtr pStr = memory.track(list.getObjectPtr(i, StringPtr.class));
                String str = pStr.getWideString(0);
                drivers.add(str);
            }

            return drivers;
        }
    }

    public static List<Datasource> datasourcesList(){
        try (PointerTracker memory = new PointerTracker()){
            IntByReference count = new IntByReference();
            ArrayPtr list = memory.track(NativeDB.INSTANCE.datasources_list(count));

            List<Datasource> datasources = new ArrayList<>();

            for (int i = 0; i < count.getValue(); i++) {
                ObjectPointer pItem = memory.track(list.getObjectPtr(i, ObjectPointer.class));

                DatasourceStruct ds = new DatasourceStruct(pItem.getPointer());
                String name = memory.track(ds.name).getWideString(0);
                String driver = memory.track(ds.driver).getWideString(0);

                datasources.add(new Datasource(name, driver));
            }

            return datasources;
        }
    }
}
