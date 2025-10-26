#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "api/api.h"
#include "api/connection.h"
#include "api/statement.h"
#include "api/result.h"
#include "api/result_set_meta_data.h"
#include "api/database_metadata.h"
#include "struct/error_info.h"
#include "struct/binary_array.h"
#include "core/chunked_binary_stream.hpp"
#include <api/odbc.h>

// Helpers
static void assert_no_error(const NativeError& err) {
    EXPECT_EQ(err.error_type, nullptr);
    EXPECT_EQ(err.error_code, 0);
    EXPECT_EQ(err.error_message, nullptr);

    if(err.error_code && err.error_message) {
        std::cout << "error_message:\t" << err.error_message << std::endl;
    }
}

static void assert_has_error(const NativeError& err) {
    EXPECT_NE(err.error_code, 0);
    EXPECT_NE(err.error_type, nullptr);
    EXPECT_NE(err.error_message, nullptr);

    if (err.error_code && err.error_message) {
        std::cout << "error_message:\t" << err.error_message << std::endl;
    }
}

static void count_check(Connection* conn, NativeError& error) {
    ApiString count_sql = NANODBC_TEXT("SELECT COUNT(*) FROM test_data;");
    nanodbc::result* count_res = execute_request(conn, count_sql.c_str(), 10, &error);
    EXPECT_TRUE(count_res->next());
    int count = count_res->get<int>(0);
    std::cout << "count_res:\t" << count << std::endl;
    EXPECT_EQ(count, 1);
    close_result(count_res, &error);
}

static Connection* create_in_memory_db(NativeError& error) {
    ApiString conn_str = NANODBC_TEXT("DRIVER={SQLite3 ODBC Driver};Database=:memory:;Timeout=1000;");
    return connection_with_timeout(conn_str.c_str(), 10, &error);
}

// Вспомогательная функция: подготовить тестовую таблицу
static void setup_test_table(Connection* conn, NativeError& error) {
    ApiString create = NANODBC_TEXT(
        "CREATE TABLE test_data ("
        "id INTEGER PRIMARY KEY, "
        "name VARCHAR(50), "
        "active BOOLEAN, "
        "score REAL, "
        "balance DOUBLE, "
        "created_date DATE, "
        "created_time TIME, "
        "created_ts TIMESTAMP, "
        "blob_data BLOB"
        ");"
    );
    nanodbc::result* res = execute_request(conn, create.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);
    assert_no_error(error);

    // Вставляем данные
    nanodbc::statement* stmt = create_statement(conn, &error);
    ASSERT_NE(stmt, nullptr);
    ApiString insert = NANODBC_TEXT(
        "INSERT INTO test_data VALUES (1, ?, ?, ?, ?, ?, ?, ?, ?);"
    );
    prepare_statement(stmt, insert.c_str(), &error);
    assert_no_error(error);

    ApiString text = NANODBC_TEXT("Alice");
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

    ApiString select = NANODBC_TEXT("SELECT * FROM test_data;");
    nanodbc::result* res = execute_request(conn, select.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    assert_no_error(error);

    EXPECT_TRUE(next_result(res, &error));
    assert_no_error(error);

    // By index
    EXPECT_EQ(get_long_value_by_index(res, 0, &error), 1);
    assert_no_error(error);
    EXPECT_EQ(ApiString(get_string_value_by_index(res, 1, &error)), ApiString(NANODBC_TEXT("Alice")));
    assert_no_error(error);
    EXPECT_TRUE(get_bool_value_by_index(res, 2, &error));
    assert_no_error(error);

    // find_column
    int idx = find_column_by_name(res, NANODBC_TEXT("score"), &error);
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

    ApiString select = NANODBC_TEXT("SELECT * FROM test_data;");
    nanodbc::result* res = execute_request(conn, select.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    assert_no_error(error);

    EXPECT_TRUE(next_result(res, &error));
    assert_no_error(error);

    // By name
    ApiString name = NANODBC_TEXT("id");
    EXPECT_EQ(get_long_value_by_name(res, name.data(), &error), 1);
    assert_no_error(error);
    name = NANODBC_TEXT("name");
    EXPECT_EQ(ApiString(get_string_value_by_name(res, name.data(), &error)), ApiString(NANODBC_TEXT("Alice")));
    assert_no_error(error);
    name = NANODBC_TEXT("active");
    EXPECT_TRUE(get_bool_value_by_name(res, name.data(), &error));
    assert_no_error(error);

    // find_column
    int idx = find_column_by_name(res, NANODBC_TEXT("score"), &error);
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

    ApiString create = NANODBC_TEXT("CREATE TABLE null_test (val INTEGER);");
    nanodbc::result* res = execute_request(conn, create.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);

    ApiString insert = NANODBC_TEXT("INSERT INTO null_test VALUES (NULL);");
    res = execute_request(conn, insert.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    close_result(res, &error);

    ApiString select = NANODBC_TEXT("SELECT val FROM null_test;");
    res = execute_request(conn, select.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);
    EXPECT_TRUE(next_result(res, &error));

    std::cout << "null_test val:\t" << res->get<int>(0) << std::endl;

    EXPECT_TRUE(was_null_by_index(res, 0, &error));
    EXPECT_TRUE(was_null_by_name(res, NANODBC_TEXT("val"), &error));
    assert_no_error(error);

    close_result(res, &error);
    disconnect(conn, &error);
    assert_no_error(error);
}
