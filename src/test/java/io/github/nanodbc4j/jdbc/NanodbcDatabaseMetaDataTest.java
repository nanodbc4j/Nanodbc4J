package io.github.nanodbc4j.jdbc;

import org.junit.jupiter.api.*;
import java.sql.*;

import static org.junit.jupiter.api.Assertions.*;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
public class NanodbcDatabaseMetaDataTest {

    private Connection conn;
    private DatabaseMetaData meta;

    @BeforeAll
    void setUp() throws SQLException {
        conn = DriverManager.getConnection("jdbc:nanodbc4j:DRIVER={SQLite3 ODBC Driver};Database=:memory:;Timeout=1000;");
        meta = conn.getMetaData();
    }

    @AfterAll
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
