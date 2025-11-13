#include <gtest/gtest.h>
#include <string>
#include "api/connection.h"
#include "api/result.h"
#include "api/database_metadata.h"
#include <../tests/test_utils.hpp>
#include "struct/error_info.h"

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

    EXPECT_NE(std::wstring(db_meta->driverName), L"");
    EXPECT_NE(std::wstring(db_meta->driverVersion), L"");

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