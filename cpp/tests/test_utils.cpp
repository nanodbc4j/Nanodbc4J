#include <gtest/gtest.h>
#include "api/odbc.h"
#include "struct/error_info.h"
#include "api/connection.h"
#include "spdlog/fmt/bundled/xchar.h"
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

nanodbc::string get_connection_string() {
    static nanodbc::string result{};

    if (!result.empty()) {
        return result;
    }

    for (const auto&[name, attributes] : nanodbc::list_drivers()) {
        // HACK: duplicate_string used to avoid heap corruption
        auto* lower_name_c_str = utils::duplicate_string(utils::to_lower(utils::to_string(name)).c_str());
        std::string lower_name = lower_name_c_str ? std::string(lower_name_c_str) : std::string();

        if (lower_name.find("sqlite") != std::string::npos) {
            result = NANODBC_TEXT("DRIVER={") + name + NANODBC_TEXT("};Database=:memory:;Timeout=1000;");
        }

        std_free(lower_name_c_str);
    }

    if (result.empty()) {
        ADD_FAILURE() << "SQLite ODBC driver not found among installed drivers";
    }

    return result;
}

// Helper: create in-memory SQLite connection
Connection* create_in_memory_db(NativeError& error) {
    const auto conn_str = utils::to_wstring(get_connection_string()) ;
    return connection_with_timeout(conn_str.c_str(), 10, &error);
}