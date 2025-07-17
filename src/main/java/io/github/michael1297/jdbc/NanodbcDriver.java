package io.github.michael1297.jdbc;

import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import io.github.michael1297.core.pointer.ObjectPointer;
import io.github.michael1297.core.pointer.StringPtr;
import io.github.michael1297.dto.DatasourceDto;
import io.github.michael1297.core.pointer.ArrayPtr;
import io.github.michael1297.core.struct.DatasourceStruct;
import io.github.michael1297.core.NativeDB;
import io.github.michael1297.core.PointerTracker;

import java.sql.Connection;
import java.sql.Driver;
import java.sql.DriverManager;
import java.sql.DriverPropertyInfo;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

public class NanodbcDriver implements Driver {
    private static final Logger logger = Logger.getLogger(NanodbcDriver.class.getName());
    public static final String PREFIX = "jdbc:odbc:";

    static {
        try {
            DriverManager.registerDriver(new NanodbcDriver());
        } catch (SQLException e) {
            logger.log(Level.SEVERE, "Could not register driver", e);
        }
    }

    @Override
    public Connection connect(String connection, Properties info) throws SQLException {
        return null;
    }

    /** @see java.sql.Driver#acceptsURL(java.lang.String) */
    public boolean acceptsURL(String url) {
        return isValidURL(url);
    }

    /**
     * Validates a URL
     *
     * @param url url
     * @return true if the URL is valid, false otherwise
     */
    static boolean isValidURL(String url) {
        return url != null && url.toLowerCase().startsWith(PREFIX);
    }

    @Override
    public DriverPropertyInfo[] getPropertyInfo(String connection, Properties info) throws SQLException {
        return new DriverPropertyInfo[0];
    }

    @Override
    public int getMajorVersion() {
        return 0;
    }

    @Override
    public int getMinorVersion() {
        return 0;
    }

    /** @see java.sql.Driver#jdbcCompliant() */
    public boolean jdbcCompliant() {
        return false;
    }

    @Override
    public Logger getParentLogger() {
        return logger;
    }

    /**
     * Gets the location to the database from a given URL.
     *
     * @param url The URL to extract the location from.
     * @return The location to the database.
     */
    static String extractAddress(String url) {
        return url.substring(PREFIX.length());
    }

    /**
     * Creates a new database connection to a given URL.
     *
     * @param url the URL
     * @param prop the properties
     * @return a Connection object that represents a connection to the URL
     * @see java.sql.Driver#connect(java.lang.String, java.util.Properties)
     */
    public static NanodbcConnection createConnection(String url, Properties prop) throws SQLException {
        if (!isValidURL(url)) return null;

        url = url.trim();
        return new NanodbcConnection(extractAddress(url), prop);
    }

    /**
     * Creates a new database connection to a given URL.
     *
     * @param url the URL
     * @return a Connection object that represents a connection to the URL
     * @see java.sql.Driver#connect(java.lang.String, java.util.Properties)
     */
    public static NanodbcConnection createConnection(String url) throws SQLException {
        if (!isValidURL(url)) return null;

        url = url.trim();
        return new NanodbcConnection(extractAddress(url), new Properties());
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

    public static List<DatasourceDto> datasourcesList(){
        try (PointerTracker memory = new PointerTracker()){
            IntByReference count = new IntByReference();
            ArrayPtr list = memory.track(NativeDB.INSTANCE.datasources_list(count));

            List<DatasourceDto> datasources = new ArrayList<>();

            for (int i = 0; i < count.getValue(); i++) {
                ObjectPointer pItem = memory.track(list.getObjectPtr(i, ObjectPointer.class));

                DatasourceStruct ds = new DatasourceStruct(pItem.getPointer());
                String name = memory.track(ds.name).getWideString(0);
                String driver = memory.track(ds.driver).getWideString(0);

                datasources.add(new DatasourceDto(name, driver));
            }

            return datasources;
        }
    }
}
