#include <gtest/gtest.h>
#include <string>
#include "api/connection.h"
#include "api/result.h"
#include "core/database_metadata.hpp"
#include "core/isolation_level.hpp"
#include <../tests/test_utils.hpp>
#include "utils/logger.hpp"
#include "struct/error_info.h"

// Test: connect using a valid connection string
TEST(ConnectionAPITest, ConnectWithConnectionString) {
    NativeError error;
    const std::wstring conn_str = get_connection_string();
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

    const std::wstring bad_conn = L"DRIVER={NonExistentDriver};Database=foo;";
    Connection* conn = connection_with_timeout(bad_conn.c_str(), 5, &error);

    EXPECT_EQ(conn, nullptr);
    assert_has_error(error);
}

// Test: transaction handling
TEST(ConnectionAPITest, TransactionControl) {
    NativeError error;
    const std::wstring conn_str = get_connection_string();
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

    int current_level = get_transaction_isolation_level(conn, &error);
    assert_no_error(error);
    int supportsTransactionIsolationLevel = metadata.supportsTransactionIsolationLevel();

    std::vector<int> levels;
    levels.push_back(IsolationLevel::READ_COMMITTED);
    levels.push_back(IsolationLevel::READ_UNCOMMITTED);
    levels.push_back(IsolationLevel::REPEATABLE_READ);
    levels.push_back(IsolationLevel::SERIALIZABLE);

    auto find = std::find_if(levels.begin(), levels.end(), [&](int level) {
        return (level != current_level) && (supportsTransactionIsolationLevel & level);
    });

    if (find != levels.end()) {
        // Set isolation level
        set_transaction_isolation_level(conn, *find, &error);
        assert_no_error(error);

        std::cout << "set transaction isolation level " << *find << std::endl;

        int level = get_transaction_isolation_level(conn, &error);
        assert_no_error(error);
        EXPECT_EQ(level, *find);
    } else {
        std::cout << "supports only transaction isolation level " << supportsTransactionIsolationLevel << std::endl;
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

    const std::wstring conn_str = get_connection_string();
    Connection* conn = connection_with_timeout(conn_str.c_str(), 10, &error);
    ASSERT_NE(conn, nullptr);

    // Create table
    const std::wstring create_sql = L"CREATE TABLE test (id INTEGER, name VARCHAR(50));";
    nanodbc::result* res = execute_request(conn, create_sql.c_str(), 10, &error);
    EXPECT_NE(res, nullptr);
    assert_no_error(error);

    close_result(res, &error);
    assert_no_error(error);

    // Insert data
    const std::wstring insert_sql = L"INSERT INTO test VALUES (1, 'Alice');";
    res = execute_request(conn, insert_sql.c_str(), 10, &error);
    EXPECT_NE(res, nullptr);
    assert_no_error(error);

    close_result(res, &error);
    assert_no_error(error);

    // Select data
    const std::wstring select_sql = L"SELECT name FROM test WHERE id = 1;";
    res = execute_request(conn, select_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    assert_no_error(error);

    EXPECT_TRUE(res->next());
    auto name = res->get<std::string>(0);
    EXPECT_EQ(name, "Alice");

    close_result(res, &error);
    assert_no_error(error);
    disconnect(conn, &error);
    assert_no_error(error);
}