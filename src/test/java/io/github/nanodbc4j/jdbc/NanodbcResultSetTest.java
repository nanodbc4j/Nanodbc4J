package io.github.nanodbc4j.jdbc;

import org.junit.jupiter.api.*;

import java.sql.*;

import static org.junit.jupiter.api.Assertions.*;


class NanodbcResultSetTest extends BaseTest {

    private Connection conn;
    private Statement stmt;
    private ResultSet rs;

    @BeforeEach
    void setUp() throws SQLException {
        conn = DriverManager.getConnection(connectionString);
        stmt = conn.createStatement();
        stmt.executeUpdate("CREATE TABLE IF NOT EXISTS test_rs (id INT, name VARCHAR(50))");
        stmt.executeUpdate("INSERT INTO test_rs VALUES (100, 'Alice')");
        rs = stmt.executeQuery("SELECT id, name FROM test_rs");
    }

    @AfterEach
    void tearDown() throws SQLException {
        if (rs != null) rs.close();
        if (stmt != null) stmt.close();
        if (conn != null) conn.close();
    }

    @Test
    void testResultSetNavigation() throws SQLException {
        assertTrue(rs.next(), "First call to next() should return true");
        assertEquals(100, rs.getInt("id"), "Expected id = 100");
        assertEquals("Alice", rs.getString("name"), "Expected name = 'Alice'");
        assertFalse(rs.next(), "Second call to next() should return false (no more rows)");
    }

    @Test
    void testColumnAccessByIndex() throws SQLException {
        rs.next();
        assertEquals(100, rs.getInt(1), "First column (index 1) should be 100");
        assertEquals("Alice", rs.getString(2), "Second column (index 2) should be 'Alice'");
    }

    @Test
    void testWasNullAfterGetString() throws SQLException {
        rs.next();
        rs.getString("name"); // This column is NOT NULL
        assertFalse(rs.wasNull(), "'wasNull()' should return false after reading a non-null value");
    }
}
