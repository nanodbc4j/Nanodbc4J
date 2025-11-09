package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.internal.dto.DriverProperties;
import org.junit.jupiter.api.*;

import java.sql.*;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.assertNotNull;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
class NanodbcConnectionTest {
    private Connection conn;

    String connectionString;

    @BeforeAll
    void setUpConnectionString() {
        var driver = NanodbcDriver.driversList()
                .stream()
                .map(DriverProperties::name)
                .peek(System.out::println)
                .filter(s -> s.toLowerCase().contains("sqlite"))
                .findFirst().orElse(null);
        assertNotNull(driver, "SQLite driver not found");
        connectionString = "jdbc:nanodbc4j:DRIVER={" + driver + "};Database=:memory:;Timeout=1000;";
        System.out.println("Connection string: " + connectionString);
    }

    @BeforeEach
    void setUp() throws SQLException {
        conn = DriverManager.getConnection(connectionString);
    }

    @AfterEach
    void tearDown() throws SQLException {
        if (conn != null && !conn.isClosed()) {
            conn.close();
        }
    }

    @Test
    void testConnectionNotNull() {
        assertNotNull(conn, "Connection should not be null");
    }

    @Test
    void testConnectionIsValid() throws SQLException {
        assertTrue(conn.isValid(5), "Connection should be valid within 5 seconds");
    }

    @Test
    void testAutoCommitDefault() throws SQLException {
        // JDBC specification requires auto-commit to be true by default
        assertTrue(conn.getAutoCommit(), "Auto-commit should be enabled by default");
    }

    @Test
    void testCloseConnection() throws SQLException {
        conn.close();
        assertTrue(conn.isClosed(), "Connection should be closed after calling close()");
    }
}
