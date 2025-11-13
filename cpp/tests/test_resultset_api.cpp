#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "api/connection.h"
#include "api/statement.h"
#include "api/result.h"
#include "struct/error_info.h"
#include "struct/binary_array.h"
#include <../tests/test_utils.hpp>

static void count_check(Connection* conn, NativeError& error) {
    const std::wstring count_sql = L"SELECT COUNT(*) FROM test_data;";
    nanodbc::result* count_res = execute_request(conn, count_sql.c_str(), 10, &error);
    EXPECT_TRUE(count_res->next());
    const int count = count_res->get<int>(0);
    std::cout << "count_res:\t" << count << std::endl;
    EXPECT_EQ(count, 1);
    close_result(count_res, &error);
}

// Helper function: prepare a test table
static void setup_test_table(Connection* conn, NativeError& error) {
    const std::wstring create = LR"(
        CREATE TABLE test_data (
        id INTEGER PRIMARY KEY,
        name VARCHAR(50),
        active BOOLEAN,
        score REAL,
        balance DOUBLE,
        created_date DATE,
        created_time TIME,
        created_ts TIMESTAMP,
        blob_data BLOB
        );)";
    nanodbc::result* res = execute_request(conn, create.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    // Insert data
    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    const std::wstring insert = L"INSERT INTO test_data VALUES (1, ?, ?, ?, ?, ?, ?, ?, ?);";
    prepare_statement(stmt, insert.c_str(), &error);
    assert_no_error(error);

    const std::wstring text = L"Alice";
    set_string_value(stmt, 0, text.data(), &error);
    assert_no_error(error);
    set_bool_value(stmt, 1, true, &error);
    assert_no_error(error);
    set_float_value(stmt, 2, 95.5f, &error);
    assert_no_error(error);
    set_double_value(stmt, 3, 1234567.89, &error);
    assert_no_error(error);

    CDate date{ 2025, 10, 26 };
    set_date_value(stmt, 4, &date, &error);
    assert_no_error(error);
    CTime time{ 14, 30, 0 };
    set_time_value(stmt, 5, &time, &error);
    assert_no_error(error);
    CTimestamp ts{ 2025, 10, 26, 14, 30, 0, 123456 };
    set_timestamp_value(stmt, 6, &ts, &error);
    assert_no_error(error);

    std::vector<uint8_t> blob = { 0x01, 0x02, 0x03, 0x04 };
    BinaryArray bin{ blob.data(), static_cast<int>(blob.size()) };
    set_binary_array_value(stmt, 7, &bin, &error);
    assert_no_error(error);

    res = execute(stmt, 10, &error);
    ASSERT_NE(res, nullptr);    
    close_result(res, &error);
    close_statement(stmt, &error);
    assert_no_error(error);

    count_check(conn, error);
}

TEST(ResultSetAPITest, GetValueByIndex) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);
    setup_test_table(conn, error);

    count_check(conn, error);

    const std::wstring select = L"SELECT * FROM test_data;";
    nanodbc::result* res = execute_request(conn, select.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    assert_no_error(error);

    EXPECT_TRUE(next_result(res, &error));
    assert_no_error(error);

    // By index
    EXPECT_EQ(get_long_value_by_index(res, 0, &error), 1);
    assert_no_error(error);
    EXPECT_EQ(std::wstring(get_string_value_by_index(res, 1, &error)), std::wstring(L"Alice"));
    assert_no_error(error);

    // error on unix sqlite odbc
#ifdef _WIN32
    EXPECT_TRUE(get_bool_value_by_index(res, 2, &error));
    assert_no_error(error);
#endif

    // find_column
    int idx = find_column_by_name(res, L"score", &error);
    EXPECT_EQ(idx, 3);
    assert_no_error(error);

    close_result(res, &error);
    disconnect(conn, &error);
    assert_no_error(error);
}

TEST(ResultSetAPITest, GetValueByName) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);
    setup_test_table(conn, error);

    count_check(conn, error);

    const std::wstring select = L"SELECT * FROM test_data;";
    nanodbc::result* res = execute_request(conn, select.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    assert_no_error(error);

    EXPECT_TRUE(next_result(res, &error));
    assert_no_error(error);

    // By name
    std::wstring name = L"id";
    EXPECT_EQ(get_long_value_by_name(res, name.data(), &error), 1);
    assert_no_error(error);
    name = L"name";
    EXPECT_EQ(std::wstring(get_string_value_by_name(res, name.data(), &error)), std::wstring(L"Alice"));
    assert_no_error(error);

    // error on unix sqlite odbc
#ifdef _WIN32
    name = NANODBC_TEXT("active");
    EXPECT_TRUE(get_bool_value_by_name(res, name.data(), &error));
    assert_no_error(error);
#endif

    // find_column
    int idx = find_column_by_name(res, L"score", &error);
    EXPECT_EQ(idx, 3);
    assert_no_error(error);

    close_result(res, &error);
    disconnect(conn, &error);
    assert_no_error(error);
}

TEST(ResultSetAPITest, NullAndBinaryHandling) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);

    const std::wstring create = L"CREATE TABLE null_test (val INTEGER);";
    nanodbc::result* res = execute_request(conn, create.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);

    const std::wstring insert = L"INSERT INTO null_test VALUES (NULL);";
    res = execute_request(conn, insert.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);

    const std::wstring select = L"SELECT val FROM null_test;";
    res = execute_request(conn, select.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    EXPECT_TRUE(next_result(res, &error));

    std::cout << "null_test val:\t" << res->get<int>(0) << std::endl;

    EXPECT_TRUE(was_null_by_index(res, 0, &error));
    EXPECT_TRUE(was_null_by_name(res, L"val", &error));
    assert_no_error(error);

    close_result(res, &error);
    disconnect(conn, &error);
    assert_no_error(error);
}
