#include <../tests/test_utils.hpp>
#include <gtest/gtest.h>
#include "api/odbc.h"
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

std::wstring get_connection_string() {
    static std::wstring result{};

    if (!result.empty()) {
        return result;
    }

    for (const auto&[name, attributes] : nanodbc::list_drivers()) {
        // HACK: duplicate_string used to avoid heap corruption
        auto* name_c_wstr = utils::duplicate_string(utils::to_wstring(name).c_str());
        auto* lower_name_c_str = utils::duplicate_string(utils::to_lower(utils::to_string(name)).c_str());

        std::string lower_name = lower_name_c_str ? std::string(lower_name_c_str) : std::string();
        std::wstring name_wstr = name_c_wstr ? std::wstring(name_c_wstr) : std::wstring();

        if (lower_name.find("sqlite") != std::string::npos) {
            result = L"DRIVER={" + name_wstr + L"};Database=:memory:;Timeout=1000;";
        }

        std_free(name_c_wstr);
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