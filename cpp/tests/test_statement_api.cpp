#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "api/connection.h"
#include "api/statement.h"
#include "api/result.h"
#include "struct/error_info.h"
#include "struct/nanodbc_c.h"
#include "struct/binary_array.h"
#include <../tests/test_utils.hpp>

// Test: prepare and execute a simple parameterized query
TEST(StatementAPITest, PrepareAndExecuteSimpleQuery) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);
    assert_no_error(error);

    // Create table
    const std::wstring create_sql = L"CREATE TABLE users (id INTEGER, name VARCHAR(50));";
    nanodbc::result* res = execute_request(conn, create_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    res = nullptr;
    assert_no_error(error);

    // Prepare INSERT
    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    assert_no_error(error);

    const std::wstring insert_sql = L"INSERT INTO users (id, name) VALUES (?, ?);";
    prepare_statement(stmt, insert_sql.c_str(), &error);
    assert_no_error(error);

    // Bind parameters
    set_int_value(stmt, 0, 42, &error);
    assert_no_error(error);

    const std::wstring name_val = L"Bob";
    set_string_value(stmt, 1, name_val.c_str(), &error);
    assert_no_error(error);

    // Execute
    res = execute(stmt, 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    res = nullptr;
    assert_no_error(error);

    // Verify insertion
    const std::wstring select_sql = L"SELECT name FROM users LIMIT 1;";
    res = execute_request(conn, select_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    EXPECT_TRUE(res->next());
    EXPECT_EQ(res->get<std::string>(0), "Bob");
    close_result(res, &error);
    res = nullptr;
    assert_no_error(error);

    close_statement(stmt, &error);
    assert_no_error(error);
    disconnect(conn, &error);
    assert_no_error(error);
}

// Test: set various numeric types
TEST(StatementAPITest, SetNumericParameterTypes) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);
    assert_no_error(error);

    const std::wstring create_sql = L"CREATE TABLE nums (i16 SMALLINT, i32 INTEGER, i64 BIGINT, f32 REAL, f64 DOUBLE PRECISION);";
    nanodbc::result* res = execute_request(conn, create_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    const std::wstring insert_sql = L"INSERT INTO nums VALUES (?, ?, ?, ?, ?);";
    prepare_statement(stmt, insert_sql.c_str(), &error);
    assert_no_error(error);

    set_short_value(stmt, 0, -123, &error);
    set_int_value(stmt, 1, 456789, &error);
    set_long_value(stmt, 2, 1234567890123LL, &error);
    set_float_value(stmt, 3, 3.14f, &error);
    set_double_value(stmt, 4, 2.718281828, &error);

    for (int i = 0; i < 5; ++i) assert_no_error(error);

    res = execute(stmt, 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    close_statement(stmt, &error);
    disconnect(conn, &error);
    assert_no_error(error);
}

// Test: set boolean, date, time, timestamp
TEST(StatementAPITest, SetDateTimeAndBool) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);
    assert_no_error(error);

    const std::wstring create_sql = L"CREATE TABLE events (active BOOLEAN, event_date DATE, event_time TIME, event_ts TIMESTAMP);";
    nanodbc::result* res = execute_request(conn, create_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    const std::wstring insert_sql = L"INSERT INTO events VALUES (?, ?, ?, ?);";
    prepare_statement(stmt, insert_sql.c_str(), &error);
    assert_no_error(error);

    set_bool_value(stmt, 0, true, &error);

    CDate date{ 2025, 10, 23 };  // year, month, day
    set_date_value(stmt, 1, &date, &error);

    CTime time{ 14, 30, 45 };  // hour, min, sec
    set_time_value(stmt, 2, &time, &error);

    CTimestamp ts{ 2025, 10, 23, 14, 30, 45, 123456 };  // year, mon, day, h, m, s, microsec
    set_timestamp_value(stmt, 3, &ts, &error);

    for (int i = 0; i < 4; ++i) assert_no_error(error);

    res = execute(stmt, 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    close_statement(stmt, &error);
    disconnect(conn, &error);
    assert_no_error(error);
}

// Test: set binary data
TEST(StatementAPITest, SetBinaryData) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);
    assert_no_error(error);

    const std::wstring create_sql = L"CREATE TABLE blobs (data BLOB);";
    nanodbc::result* res = execute_request(conn, create_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    const std::wstring insert_sql = L"INSERT INTO blobs VALUES (?);";
    prepare_statement(stmt, insert_sql.c_str(), &error);
    assert_no_error(error);

    std::vector<uint8_t> data = { 0xDE, 0xAD, 0xBE, 0xEF };
    BinaryArray bin{ data.data(), static_cast<int>(data.size()) };
    set_binary_array_value(stmt, 0, &bin, &error);
    assert_no_error(error);

    res = execute(stmt, 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    close_statement(stmt, &error);
    disconnect(conn, &error);
    assert_no_error(error);
}

// Test: execute with timeout
TEST(StatementAPITest, ExecuteWithTimeout) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);
    assert_no_error(error);

    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    const std::wstring sql = L"SELECT 1;";
    prepare_statement(stmt, sql.c_str(), &error);
    assert_no_error(error);

    nanodbc::result* res = execute(stmt, 5, &error);  // 5 seconds timeout
    ASSERT_NE(res, nullptr);
    EXPECT_TRUE(res->next());
    EXPECT_EQ(res->get<int>(0), 1);
    close_result(res, &error);
    assert_no_error(error);

    close_statement(stmt, &error);
    disconnect(conn, &error);
    assert_no_error(error);
}