#include <../tests/test_utils.hpp>
#include <gtest/gtest.h>
#include "api/odbc.h"
#include "utils/string_utils.hpp"

void assert_no_error(const NativeError& err) {
    EXPECT_EQ(err.status, EXIT_SUCCESS);
    EXPECT_EQ(err.error_message, nullptr);
}

void assert_has_error(const NativeError& err) {
    EXPECT_NE(err.status, EXIT_SUCCESS);
    EXPECT_NE(err.error_message, nullptr);
}

ApiString get_connection_string() {
    static ApiString result{};

    if (!result.empty()) {
        return result;
    }

    for (const auto&[name, attributes] : nanodbc::list_drivers()) {
        // HACK: duplicate_string used to avoid heap corruption
        auto* name_c_api_str = utils::duplicate_string( static_cast<ApiString>(StringProxy(name)).c_str());
        auto* lower_name_c_str = utils::duplicate_string(utils::to_lower(utils::to_string(name)).c_str());

        std::string lower_name = lower_name_c_str ? std::string(lower_name_c_str) : std::string();
        ApiString name_wstr = name_c_api_str ? ApiString(name_c_api_str) : ApiString();

        if (lower_name.find("sqlite3") != std::string::npos) {
            result = ODBC_TEXT("DRIVER={") + name_wstr + ODBC_TEXT("};Database=:memory:;Timeout=1000;");
        }

        std_free(name_c_api_str);
        std_free(lower_name_c_str);
    }

    if (result.empty()) {
        ADD_FAILURE() << "SQLite ODBC driver not found among installed drivers";
    }

    return result;
}

// Helper: create in-memory SQLite connection
Connection* create_in_memory_db(NativeError& error) {
    const auto conn_str = get_connection_string();
    return connection_with_timeout(conn_str.c_str(), 10, &error);
}