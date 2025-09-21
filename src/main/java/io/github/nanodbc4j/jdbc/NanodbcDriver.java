package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.internal.dto.DatasourceDto;
import io.github.nanodbc4j.internal.dto.DriverDto;
import io.github.nanodbc4j.internal.handler.DriverHandler;
import lombok.extern.java.Log;

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

/**
 * JDBC Driver implementation over ODBC using nanodbc.
 * <p>
 * Usage: {@code DriverManager.getConnection("jdbc:odbc:Driver={...};...")}
 */
@Log
public class NanodbcDriver implements Driver {
    public static final String PREFIX = "jdbc:odbc:";
    static final int MAJOR_VERSION = 1;
    static final int MINOR_VERSION = 7;

    static {
        try {
            DriverHandler.setLogLevel(SpdLogLevel.DEBUG);
            DriverManager.registerDriver(new NanodbcDriver());
        } catch (SQLException e) {
            log.log(Level.SEVERE, "Could not register driver", e);
            throw new ExceptionInInitializerError(e);
        }
    }

    @Override
    public Connection connect(String url, Properties info) throws SQLException {
        log.finest("connect");
        if (!acceptsURL(url)) {
            return null;
        }

        String connectionString = extractAddress(url).trim(); // ← убираем jdbc:odbc:
        return new NanodbcConnection(connectionString);
    }

    /**
     * @see java.sql.Driver#acceptsURL(java.lang.String)
     */
    @Override
    public boolean acceptsURL(String url) {
        log.finest("acceptsURL");
        return isValidURL(url);
    }

    /**
     * Validates a URL
     *
     * @param url url
     * @return true if the URL is valid, false otherwise
     */
    static boolean isValidURL(String url) {
        log.finest("isValidURL");
        return url != null && url.toLowerCase().startsWith(PREFIX);
    }

    @Override
    public DriverPropertyInfo[] getPropertyInfo(String connection, Properties info) throws SQLException {
        log.finest("getPropertyInfo");
        return new DriverPropertyInfo[0];
    }

    @Override
    public int getMajorVersion() {
        log.finest("getMajorVersion");
        return MAJOR_VERSION;
    }

    @Override
    public int getMinorVersion() {
        log.finest("getMinorVersion");
        return MINOR_VERSION;
    }

    /**
     * @see java.sql.Driver#jdbcCompliant()
     */
    @Override
    public boolean jdbcCompliant() {
        log.finest("jdbcCompliant");
        return false;
    }

    @Override
    public Logger getParentLogger() throws SQLFeatureNotSupportedException {
        log.finest("getParentLogger");
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
     * @param url  the URL
     * @param prop the properties
     * @return a Connection object that represents a connection to the URL
     * @see java.sql.Driver#connect(java.lang.String, java.util.Properties)
     */
    public static NanodbcConnection createConnection(String url, Properties prop) throws SQLException {
        if (!isValidURL(url)) return null;

        String connectionString = extractAddress(url).trim(); // ← убираем jdbc:odbc:
        return new NanodbcConnection(connectionString);
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

    public static void setLogLevel(SpdLogLevel level) {
        DriverHandler.setLogLevel(level);
    }

    public static List<DriverDto> driversList() {
        return DriverHandler.driversList();
    }

    public static List<DatasourceDto> datasourcesList() {
        return DriverHandler.datasourcesList();
    }
}
