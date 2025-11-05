package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.internal.dto.DriverProperties;
import org.junit.jupiter.api.*;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import static org.junit.jupiter.api.Assertions.*;

@TestInstance(TestInstance.Lifecycle.PER_CLASS)
class NanodbcStatementTest {

    private Connection conn;
    private Statement stmt;

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
        stmt = conn.createStatement();
    }

    @AfterEach
    void tearDown() throws SQLException {
        if (stmt != null) stmt.close();
        if (conn != null) conn.close();
    }

    @Test
    void testExecuteQueryReturnsResultSet() throws SQLException {
        ResultSet rs = stmt.executeQuery("SELECT 1 AS id");
        assertNotNull(rs, "executeQuery() should return a non-null ResultSet");
        assertTrue(rs.next(), "ResultSet should contain at least one row");
        assertEquals(1, rs.getInt("id"), "Column 'id' should equal 1");
        rs.close();
    }

    @Test
    void testExecuteUpdateReturnsRowCount() throws SQLException {
        // DDL statements typically return 0 rows affected
        stmt.executeUpdate("CREATE TABLE IF NOT EXISTS test_table (id INT)");
        // DML: verify no exception is thrown
        assertDoesNotThrow(() -> stmt.executeUpdate("INSERT INTO test_table VALUES (42)"));
    }

    @Test
    void testClosedStatementThrowsException() throws SQLException {
        stmt.close();
        SQLException ex = assertThrows(SQLException.class, () -> stmt.executeQuery("SELECT 1"));
        assertTrue(ex.getMessage().toLowerCase().contains("closed"),
                "Using a closed Statement should throw an SQLException indicating it is closed");
    }
}
