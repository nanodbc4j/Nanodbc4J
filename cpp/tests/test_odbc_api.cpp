#include <gtest/gtest.h>
#include "api/odbc.h"

// Test: get drivers list
TEST(OdbcAPITest, DriversList) {
	int count = 0;
	const auto api_drivers = drivers_list(&count);
	auto nanodbc_drivers = nanodbc::list_drivers();
	EXPECT_EQ(count, static_cast<int>(nanodbc_drivers.size()));
	delete_driver_array(api_drivers, count);
}

// Test: get datasource list
TEST(OdbcAPITest, DatasourceList) {
	int count = 0;
	auto api_datasources = datasources_list (&count);
	auto nanodbc_datasources = nanodbc::list_datasources();
	EXPECT_EQ(count, static_cast<int>(nanodbc_datasources.size()));
	delete_datasource_array(api_datasources, count);
}