package io.github.michael1297.core;

import io.github.michael1297.core.metadata.Datasource;
import io.github.michael1297.internal.handler.Handler;

import java.sql.Connection;
import java.sql.Driver;
import java.sql.DriverManager;
import java.sql.DriverPropertyInfo;
import java.sql.SQLException;
import java.sql.SQLFeatureNotSupportedException;
import java.util.List;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

public class NanodbcDriver implements Driver {
    private static final Logger logger = Logger.getLogger(NanodbcDriver.class.getName());
    public static final String PREFIX = "jdbc:odbc:";
    public static final int MAJOR_VERSION = 1;
    public static final int MINOR_VERSION = 7;

    static {
        try {
            DriverManager.registerDriver(new NanodbcDriver());
        } catch (SQLException e) {
            logger.log(Level.SEVERE, "Could not register driver", e);
        }
    }

    @Override
    public Connection connect(String connection, Properties info) throws SQLException {
        return new NanodbcConnection(connection);
    }

    /** @see java.sql.Driver#acceptsURL(java.lang.String) */
    @Override
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
        return MAJOR_VERSION;
    }

    @Override
    public int getMinorVersion() {
        return MINOR_VERSION;
    }

    /** @see java.sql.Driver#jdbcCompliant() */
    public boolean jdbcCompliant() {
        return false;
    }

    @Override
    public Logger getParentLogger() throws SQLFeatureNotSupportedException {
        throw new SQLFeatureNotSupportedException();
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
        return new NanodbcConnection(extractAddress(url));
    }

    /**
     * Creates a new database connection to a given URL.
     *
     * @param url the URL
     * @return a Connection object that represents a connection to the URL
     * @see java.sql.Driver#connect(java.lang.String, java.util.Properties)
     */
    public static NanodbcConnection createConnection(String url) throws SQLException {
        return createConnection(url, null);
    }

    public static List<io.github.michael1297.core.metadata.Driver> driversList(){
        return Handler.driversList();
    }

    public static List<Datasource> datasourcesList(){
        return Handler.datasourcesList();
    }
}
