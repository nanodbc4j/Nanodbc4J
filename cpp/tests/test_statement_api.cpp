#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "api/api.h"
#include "api/connection.h"
#include "api/statement.h"
#include "api/result.h"
#include "struct/error_info.h"
#include "struct/nanodbc_c.h"
#include "struct/binary_array.h"

// Helper: verify no error
static void assert_no_error(const NativeError& err) {
    EXPECT_EQ(err.error_type, nullptr);
    EXPECT_EQ(err.error_code, 0);
    EXPECT_EQ(err.error_message, nullptr);
}

// Helper: verify error is set
static void assert_has_error(const NativeError& err) {
    EXPECT_NE(err.error_type, nullptr) << "Error type should not be null";
    EXPECT_NE(err.error_message, nullptr) << "Error message should not be null";
}

// Helper: create in-memory SQLite connection
static Connection* create_in_memory_db(NativeError& error) {
    ApiString conn_str = NANODBC_TEXT("DRIVER={SQLite3 ODBC Driver};Database=:memory:;Timeout=1000;");
    return connection_with_timeout(conn_str.c_str(), 10, &error);
}

// Test: prepare and execute a simple parameterized query
TEST(StatementAPITest, PrepareAndExecuteSimpleQuery) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);
    assert_no_error(error);

    // Create table
    ApiString create_sql = NANODBC_TEXT("CREATE TABLE users (id INTEGER, name VARCHAR(50));");
    nanodbc::result* res = execute_request(conn, create_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    res = nullptr;
    assert_no_error(error);

    // Prepare INSERT
    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    assert_no_error(error);

    ApiString insert_sql = NANODBC_TEXT("INSERT INTO users (id, name) VALUES (?, ?);");
    prepare_statement(stmt, insert_sql.c_str(), &error);
    assert_no_error(error);

    // Bind parameters
    set_int_value(stmt, 0, 42, &error);
    assert_no_error(error);

    ApiString name_val = NANODBC_TEXT("Bob");
    set_string_value(stmt, 1, name_val.c_str(), &error);
    assert_no_error(error);

    // Execute
    res = execute(stmt, 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    res = nullptr;
    assert_no_error(error);

    // Verify insertion
    ApiString select_sql = NANODBC_TEXT("SELECT name FROM users LIMIT 1;");
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

    ApiString create_sql = NANODBC_TEXT("CREATE TABLE nums (i16 SMALLINT, i32 INTEGER, i64 BIGINT, f32 REAL, f64 DOUBLE PRECISION);");
    nanodbc::result* res = execute_request(conn, create_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    ApiString insert_sql = NANODBC_TEXT("INSERT INTO nums VALUES (?, ?, ?, ?, ?);");
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

    ApiString create_sql = NANODBC_TEXT("CREATE TABLE events (active BOOLEAN, event_date DATE, event_time TIME, event_ts TIMESTAMP);");
    nanodbc::result* res = execute_request(conn, create_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    ApiString insert_sql = NANODBC_TEXT("INSERT INTO events VALUES (?, ?, ?, ?);");
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

    ApiString create_sql = NANODBC_TEXT("CREATE TABLE blobs (data BLOB);");
    nanodbc::result* res = execute_request(conn, create_sql.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    ApiString insert_sql = NANODBC_TEXT("INSERT INTO blobs VALUES (?);");
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
    ApiString sql = NANODBC_TEXT("SELECT 1;");
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