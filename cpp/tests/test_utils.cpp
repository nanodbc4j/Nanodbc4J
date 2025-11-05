#include <gtest/gtest.h>
#include "api/api.h"
#include "struct/error_info.h"
#include "api/connection.h"
#include "utils/string_utils.hpp"

void assert_no_error(const NativeError& err) {
    EXPECT_EQ(err.error_type, nullptr);
    EXPECT_EQ(err.error_code, 0);
    EXPECT_EQ(err.error_message, nullptr);
}

void assert_has_error(const NativeError& err) {
    EXPECT_NE(err.error_code, 0);
    EXPECT_NE(err.error_type, nullptr);
    EXPECT_NE(err.error_message, nullptr);
}

ApiString get_connection_string() {
    static ApiString result{};
    // HACK: static used to avoid heap corruption
    // Safe only because this is test-only code
    static std::string lower_name;

    if (!result.empty()) {
        return result;
    }

    for (const auto& driver : nanodbc::list_drivers()) {
        lower_name = utils::to_lower(utils::to_string(driver.name));

        if (lower_name.find("sqlite") != std::string::npos) {
            result = NANODBC_TEXT("DRIVER={") + driver.name + NANODBC_TEXT("};Database=:memory:;Timeout=1000;");
        }
    }

    if (result.empty()) {
        ADD_FAILURE() << "SQLite ODBC driver not found among installed drivers";
    }

    return result;
}

// Helper: create in-memory SQLite connection
Connection* create_in_memory_db(NativeError& error) {
    ApiString conn_str = get_connection_string();
    return connection_with_timeout(conn_str.c_str(), 10, &error);
}