package io.github.nanodbc4j.jdbc;

import org.junit.jupiter.api.*;

import java.sql.*;

import static org.junit.jupiter.api.Assertions.*;
import static org.junit.jupiter.api.Assertions.assertNotNull;

class NanodbcConnectionTest extends BaseTest {
    private Connection conn;

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
