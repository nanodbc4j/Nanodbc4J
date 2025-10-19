#include <gtest/gtest.h>
#include <string>
#include "api/api.h"
#include "api/connection.h"
#include "api/result.h"
#include "core/database_metadata.hpp"
#include "utils/logger.hpp"
#include "struct/error_info.h"

// Helper function: verify that no error is set
static void assert_no_error(const NativeError& err) {
    EXPECT_EQ(err.error_type, nullptr);
    EXPECT_EQ(err.error_code, 0);
    EXPECT_EQ(err.error_message, nullptr);
}

// Helper function: verify that an error is set
static void assert_has_error(const NativeError& err) {
    EXPECT_NE(err.error_type, nullptr) << "SQL state should not be empty on error";
    // error_code may be 0 in some cases, but message must not be null
    EXPECT_NE(err.error_message, nullptr) << "Error message should not be empty";
}

// Test: connect using a valid connection string
TEST(ConnectionAPITest, ConnectWithConnectionString) {
    NativeError error;
    ApiString conn_str = NANODBC_TEXT("DRIVER={SQLite3 ODBC Driver};Database=:memory:;Timeout=1000;");
    Connection* conn = connection_with_timeout(conn_str.c_str(), 10, &error);
    ASSERT_NE(conn, nullptr);
    assert_no_error(error);

    bool connected = is_connected(conn, nullptr);
    EXPECT_TRUE(connected);
    assert_no_error(error);

    disconnect(conn, nullptr);
    assert_no_error(error);
}

// Test: attempt to connect with an invalid connection string → expect error
TEST(ConnectionAPITest, ConnectWithInvalidConnectionString) {
    NativeError error;

    const ApiChar* bad_conn = NANODBC_TEXT("DRIVER={NonExistentDriver};Database=foo;");
    Connection* conn = connection_with_timeout(bad_conn, 5, &error);

    EXPECT_EQ(conn, nullptr);
    assert_has_error(error);
}

// Test: transaction handling
TEST(ConnectionAPITest, TransactionControl) {
    NativeError error;
    ApiString conn_str = NANODBC_TEXT("DRIVER={SQLite3 ODBC Driver};Database=:memory:;Timeout=1000;");
    Connection* conn = connection_with_timeout(conn_str.c_str(), 10, &error);
    ASSERT_NE(conn, nullptr);
    assert_no_error(error);

    DatabaseMetaData metadata(*conn);

    // Check default auto-commit
    bool autoCommit = get_auto_commit_transaction(conn, &error);
    EXPECT_TRUE(autoCommit);
    assert_no_error(error);

    // Disable auto-commit
    set_auto_commit_transaction(conn, false, &error);
    assert_no_error(error);

    autoCommit = get_auto_commit_transaction(conn, &error);
    EXPECT_FALSE(autoCommit);
    assert_no_error(error);

    if (metadata.supportsTransactionIsolationLevel() & 2) {
        // Set isolation level
        set_transaction_isolation_level(conn, 2, &error); // READ COMMITTED
        assert_no_error(error);

        int level = get_transaction_isolation_level(conn, &error);
        EXPECT_EQ(level, 2);
        assert_no_error(error);
    }

    // Commit (even though we haven't done anything)
    commit_transaction(conn, &error);
    assert_no_error(error);

    disconnect(conn, &error);
    assert_no_error(error);
}

// Test: execute a simple SQL query and verify results
TEST(ConnectionAPITest, ExecuteSimpleQuery) {
    NativeError error;

    ApiString conn_str = NANODBC_TEXT("DRIVER={SQLite3 ODBC Driver};Database=:memory:;Timeout=1000;");
    Connection* conn = connection_with_timeout(conn_str.c_str(), 10, &error);
    ASSERT_NE(conn, nullptr);

    // Create table
    ApiString create_sql = NANODBC_TEXT("CREATE TABLE test (id INTEGER, name VARCHAR(50));");
    nanodbc::result* res = execute_request(conn, create_sql.c_str(), 10, &error);
    EXPECT_NE(res, nullptr);
    assert_no_error(error);

    close_result(res, &error);
    assert_no_error(error);

    // Insert data
    ApiString insert_sql = NANODBC_TEXT("INSERT INTO test VALUES (1, 'Alice');");
    std::cout << 8 << std::endl;
    res = execute_request(conn, insert_sql.c_str(), 10, &error);
    EXPECT_NE(res, nullptr);
    assert_no_error(error);

    close_result(res, &error);
    assert_no_error(error);

    // Select data
    ApiString select_sql = NANODBC_TEXT("SELECT name FROM test WHERE id = 1;");
    res = execute_request(conn, select_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    assert_no_error(error);

    EXPECT_TRUE(res->next());
    std::string name = res->get<std::string>(0);
    EXPECT_EQ(name, "Alice");

    close_result(res, &error);
    assert_no_error(error);
    disconnect(conn, &error);
    assert_no_error(error);
}