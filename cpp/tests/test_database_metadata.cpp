#include <gtest/gtest.h>
#include <string>
#include "api/api.h"
#include "api/connection.h"
#include "api/result.h"
#include "api/database_metadata.h"
#include "struct/error_info.h"

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
}

TEST(DatabaseMetaDataTest, BasicInfo) {
    NativeError error;
    Connection* conn = create_in_memory_db(error);
    ASSERT_NE(conn, nullptr);

    setup_test_table(conn, error);

    CDatabaseMetaData* db_meta = get_database_meta_data(conn, &error);
    ASSERT_NE(db_meta, nullptr);
    assert_no_error(error);

    EXPECT_NE(ApiString(db_meta->driverName), ApiString(NANODBC_TEXT("")));
    EXPECT_NE(ApiString(db_meta->driverVersion), ApiString(NANODBC_TEXT("")));

    delete_database_meta_data(db_meta);

    // Tables
    nanodbc::result* tables = get_database_meta_data_tables(conn, nullptr, nullptr, nullptr, nullptr, &error);    
    ASSERT_NE(tables, nullptr);
    EXPECT_TRUE(next_result(tables, &error));
    close_result(tables, &error);
    assert_no_error(error);

    disconnect(conn, &error);
    assert_no_error(error);
}