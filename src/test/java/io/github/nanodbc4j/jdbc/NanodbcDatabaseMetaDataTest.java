package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.internal.dto.DriverProperties;
import org.junit.jupiter.api.*;
import java.sql.*;

import static org.junit.jupiter.api.Assertions.*;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
class NanodbcDatabaseMetaDataTest {

    private Connection conn;
    private DatabaseMetaData meta;

    String connectionString;

    @BeforeAll
    void setUpConnectionString() {
        var driver = NanodbcDriver.driversList().stream().map(DriverProperties::name).filter(s -> s.toLowerCase().contains("sqlite")).findFirst().orElse(null);
        assertNotNull(driver, "SQLite driver not found");
        connectionString = "jdbc:nanodbc4j:DRIVER={" + driver + "};Database=:memory:;Timeout=1000;";
    }

    @BeforeEach
    void setUp() throws SQLException {
        conn = DriverManager.getConnection(connectionString);
        meta = conn.getMetaData();
    }

    @AfterEach
    void tearDown() throws SQLException {
        if (conn != null) conn.close();
    }

    @Test
    void testDriverNameAndVersion() throws SQLException {
        assertNotNull(meta.getDriverName(), "Driver name must not be null");
        assertNotNull(meta.getDriverVersion(), "Driver version must not be null");
    }

    @Test
    void testDatabaseProductName() throws SQLException {
        String productName = meta.getDatabaseProductName();
        assertNotNull(productName, "Database product name must not be null");
        assertFalse(productName.isEmpty(), "Database product name must not be empty");
    }

    @Test
    void testSupportsTransactions() throws SQLException {
        // Most databases support transactions; adjust expectation if yours doesn't
        assertTrue(meta.supportsTransactions(), "Database should support transactions");
    }

    @Test
    void testGetTablesContainsOurTable() throws SQLException {
        conn.createStatement().executeUpdate("CREATE TABLE IF NOT EXISTS meta_test (x INT)");
        try (ResultSet tables = meta.getTables(null, null, "meta_test", new String[]{"TABLE"})) {
            assertTrue(tables.next(), "Table 'meta_test' should be listed in metadata");
            assertEquals("meta_test", tables.getString("TABLE_NAME"), "Table name in metadata must match");
        }
    }
}
