package io.github.nanodbc4j.jdbc;

import io.github.nanodbc4j.internal.dto.DriverProperties;
import org.junit.jupiter.api.*;
import java.sql.*;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Tests for ResultSetMetaData behavior in the custom JDBC driver.
 * Mirrors the logic of the C++ ResultSetMetaDataTest.BasicMetadata test.
 */
@TestInstance(TestInstance.Lifecycle.PER_CLASS)
class NanodbcResultSetMetaDataTest {

    private Connection connection;

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
    }

    @BeforeEach
    void setUp() throws SQLException {
        connection = DriverManager.getConnection(connectionString);

        // Create and populate test table
        createTestTable();
        insertTestData();
    }

    @AfterEach
    void tearDown() throws SQLException {
        if (connection != null && !connection.isClosed()) {
            connection.close();
        }
    }

    /**
     * Creates the test_data table with various SQL types.
     */
    private void createTestTable() throws SQLException {
        String createSql = """
            CREATE TABLE test_data (
                id INTEGER PRIMARY KEY,
                name VARCHAR(50),
                active BOOLEAN,
                score REAL,
                balance DOUBLE PRECISION,
                created_date DATE,
                created_time TIME,
                created_ts TIMESTAMP,
                blob_data BLOB
            )
            """;
        try (Statement stmt = connection.createStatement()) {
            stmt.execute(createSql);
        }
    }

    /**
     * Inserts one row of test data matching the C++ test.
     */
    private void insertTestData() throws SQLException {
        String insertSql = """
            INSERT INTO test_data VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
            """;
        try (PreparedStatement pstmt = connection.prepareStatement(insertSql)) {
            pstmt.setInt(1, 1);
            pstmt.setString(2, "Alice");
            pstmt.setBoolean(3, true);
            pstmt.setFloat(4, 95.5f);
            pstmt.setDouble(5, 1234567.89);
            pstmt.setDate(6, Date.valueOf("2025-10-26"));
            pstmt.setTime(7, Time.valueOf("14:30:00"));
            pstmt.setTimestamp(8, Timestamp.valueOf("2025-10-26 14:30:00.123456"));
            pstmt.setBytes(9, new byte[]{0x01, 0x02, 0x03, 0x04});
            pstmt.executeUpdate();
        }
    }

    /**
     * Validates basic ResultSetMetaData: column count and names.
     */
    @Test
    void testBasicMetadata() throws SQLException {
        String selectSql = "SELECT id, name FROM test_data";
        try (Statement stmt = connection.createStatement();
             ResultSet rs = stmt.executeQuery(selectSql)) {

            // Ensure we have data
            assertTrue(rs.next(), "ResultSet should contain at least one row");

            // Get metadata
            ResultSetMetaData metaData = rs.getMetaData();
            assertNotNull(metaData, "ResultSetMetaData must not be null");

            // Validate column count
            assertEquals(2, metaData.getColumnCount(), "Expected 2 columns in result set");

            // Validate column names (case-insensitive per JDBC spec, but usually preserved)
            assertEquals("id", metaData.getColumnName(1), "First column name should be 'id'");
            assertEquals("name", metaData.getColumnName(2), "Second column name should be 'name'");

            // verify column types
            assertEquals(Types.INTEGER, metaData.getColumnType(1));
        }
    }
}
