#include <gtest/gtest.h>
#include <string>
#include "api/api.h"
#include "api/connection.h"
#include "api/result.h"
#include "api/database_metadata.h"
#include <../tests/test_utils.hpp>
#include "struct/error_info.h"

// Вспомогательная функция: подготовить тестовую таблицу
static void setup_test_table(Connection* conn, NativeError& error) {
    const ApiString create = NANODBC_TEXT(
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