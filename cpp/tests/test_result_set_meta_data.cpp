#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "api/connection.h"
#include "api/statement.h"
#include "api/result.h"
#include "api/result_set_meta_data.h"
#include <../tests/test_utils.hpp>
#include "struct/error_info.h"
#include "struct/binary_array.h"

static void count_check(Connection* conn, NativeError& error) {
    const std::wstring count_sql = L"SELECT COUNT(*) FROM test_data;";
    auto* count_res = execute_request(conn, count_sql.c_str(), 10, &error);
    EXPECT_TRUE(count_res->next());
    int count = count_res->get<int>(0);
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
        balance DOUBLE PRECISION,
        created_date DATE,
        created_time TIME,
        created_ts TIMESTAMP,
        blob_data BLOB
        );)";
    auto* res = execute_request(conn, create.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    // Insert data
    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    const std::wstring insert = L"INSERT INTO test_data VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
    prepare_statement(stmt, insert.c_str(), &error);
    assert_no_error(error);

    set_long_value(stmt, 0, 1, &error);
    set_string_value(stmt, 1, L"Alice", &error);
    set_bool_value(stmt, 2, true, &error);
    set_float_value(stmt, 3, 95.5f, &error);
    set_double_value(stmt, 4, 1234567.89, &error);

    CDate date{ 2025, 10, 26 };
    set_date_value(stmt, 5, &date, &error);
    CTime time{ 14, 30, 0 };
    set_time_value(stmt, 6, &time, &error);
    CTimestamp ts{ 2025, 10, 26, 14, 30, 0, 123456 };
    set_timestamp_value(stmt, 7, &ts, &error);

    std::vector<uint8_t> blob = { 0x01, 0x02, 0x03, 0x04 };
    BinaryArray bin{ blob.data(), static_cast<int>(blob.size()) };
    set_binary_array_value(stmt, 8, &bin, &error);

    assert_no_error(error);

    res = execute(stmt, 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    close_statement(stmt, &error);
    assert_no_error(error);

    count_check(conn, error);
}

TEST(ResultSetMetaDataTest, BasicMetadata) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);
    setup_test_table(conn, error);

    const std::wstring select = L"SELECT id, name FROM test_data;";
    auto* res = execute_request(conn, select.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);

    CResultSetMetaData* meta = get_meta_data(res, &error);
    ASSERT_NE(meta, nullptr);
    assert_no_error(error);

    EXPECT_EQ(meta->columnCount, 2);

    EXPECT_EQ(std::wstring(meta->column[0]->columnName), std::wstring(L"id"));
    EXPECT_EQ(std::wstring(meta->column[1]->columnName), std::wstring(L"name"));

    delete_meta_data(meta);
    close_result(res, &error);
    disconnect(conn, &error);
    assert_no_error(error);
}

