package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.dto.DriverProperties;
import org.junit.jupiter.api.BeforeAll;

import static org.junit.jupiter.api.Assertions.assertNotNull;

public abstract class BaseTest {
    protected static String connectionString;

    @BeforeAll
    static void setUpOnce() {
        var driver = NanodbcDriver.driversList()
                .stream()
                .map(DriverProperties::name)
                .peek(System.out::println)
                .filter(s -> s.toLowerCase().contains("sqlite3"))
                .findFirst().orElse(null);
        assertNotNull(driver, "SQLite driver not found");
        connectionString = "jdbc:nanodbc4j:DRIVER={" + driver + "};Database=:memory:;Timeout=1000;";
        System.out.println("Connection string: " + connectionString);
    }
}
