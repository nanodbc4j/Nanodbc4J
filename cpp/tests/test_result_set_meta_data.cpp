#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "api/api.h"
#include "api/connection.h"
#include "api/statement.h"
#include "api/result.h"
#include "api/result_set_meta_data.h"
#include "struct/error_info.h"
#include "struct/binary_array.h"

// Helpers
static void assert_no_error(const NativeError& err) {
    EXPECT_EQ(err.error_type, nullptr);
    EXPECT_EQ(err.error_code, 0);
    EXPECT_EQ(err.error_message, nullptr);
}

static void assert_has_error(const NativeError& err) {
    EXPECT_NE(err.error_code, 0);
    EXPECT_NE(err.error_type, nullptr);
    EXPECT_NE(err.error_message, nullptr);
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
        "balance DOUBLE PRECISION, "
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
        "INSERT INTO test_data VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"
    );
    prepare_statement(stmt, insert.c_str(), &error);
    assert_no_error(error);

    set_long_value(stmt, 0, 1, &error);
    set_string_value(stmt, 1, NANODBC_TEXT("Alice"), &error);
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

    ApiString select = NANODBC_TEXT("SELECT id, name FROM test_data;");
    nanodbc::result* res = execute_request(conn, select.c_str(), 10, &error);
    ASSERT_NE(res, nullptr);

    CResultSetMetaData* meta = get_meta_data(res, &error);
    ASSERT_NE(meta, nullptr);
    assert_no_error(error);

    EXPECT_EQ(meta->columnCount, 2);

    EXPECT_EQ(ApiString(meta->column[0]->columnName), ApiString(NANODBC_TEXT("id")));
    EXPECT_EQ(ApiString(meta->column[1]->columnName), ApiString(NANODBC_TEXT("name")));

    delete_meta_data(meta);
    close_result(res, &error);
    disconnect(conn, &error);
    assert_no_error(error);
}

